
#include "chunk_loader.h"
#include "state.h"
#include "constants.h"

#include <chrono>
#include <thread>
#include <boost/thread/thread.hpp>

#include <boost/unordered_set.hpp>
using boost::unordered_set;

#include <mutex>
#include <iostream>
using namespace std;

std::mutex chunk_loader_lock;

void process_chunk_action_ress(){
  Chunk_Action_Res* res;
  while (state.chunk_action_ress.pop(res)){
    if (res->type() == Chunk_Action_Ress::Unload_Chunk_Res) {
      Unload_Chunk_Action_Res* u_res = static_cast<Unload_Chunk_Action_Res*>(res);

      if (render_state.chunks.find(u_res->xyz) != render_state.chunks.end()){
        Chunk& c = render_state.chunks.find(u_res->xyz)->second;
        render_state.chunks.erase(u_res->xyz);
        unload_chunk(c);
      }
    } else if (res->type() == Chunk_Action_Ress::Load_Chunk_Res) {
      Load_Chunk_Action_Res* l_res = static_cast<Load_Chunk_Action_Res*>(res);

      int x, y, z;
      tie(x, y, z) = l_res->xyz;

      if (render_state.chunks.find(l_res->xyz) != render_state.chunks.end()){
        Chunk& c = render_state.chunks.find(l_res->xyz)->second;
        render_state.chunks.erase(l_res->xyz);
        unload_chunk(c);
      }

      Chunk c = l_res->chunk;

      if (!init_chunk_gl(c, render_state.program, x, y, z, state.world)){
        throw runtime_error("could not init chunk");
      }

      render_state.chunks.insert({l_res->xyz, c});
    }
    delete res;
  }
}

void chunk_loader(void){
  while (true){
    chunk_loader_lock.lock();
    chunk_loader_lock.lock();
    Chunk_Action_Req* req;
    while (state.chunk_action_reqs.pop(req)){
      if (req->type() == Chunk_Action_Reqs::Unload_Chunk_Req) {
        Unload_Chunk_Action_Req* u_req = static_cast<Unload_Chunk_Action_Req*>(req);

        state.chunk_action_ress.push(new Unload_Chunk_Action_Res(u_req->xyz));

      // ==============================================================
      } else if (req->type() == Chunk_Action_Reqs::Load_Chunk_Req) {
        Load_Chunk_Action_Req* l_req = static_cast<Load_Chunk_Action_Req*>(req);
        int x, y, z;
        tie(x, y, z) = l_req->xyz;

        Chunk chunk;
        init_chunk_cubes(chunk, x, y, z, state.world);
        state.chunk_action_ress.push(new Load_Chunk_Action_Res(l_req->xyz, chunk));
      }

      delete req;
    }
    chunk_loader_lock.unlock();
  }
}


void start_chunk_loader_thread(){
  boost::thread thread(chunk_loader);
}

void load_all_chunks(){
  int pcx = -snap_to_chunk(render_state.player_x);
  int pcy = -snap_to_chunk(render_state.player_y);
  int pcz = -snap_to_chunk(render_state.player_x);

  unordered_set<XYZ> xyzs_to_load;
  for (int x = -render_state.render_dist; x <= render_state.render_dist; x++){
    for (int y = -render_state.render_dist; y <= render_state.render_dist; y++){
      for (int z = -render_state.render_dist; z <= render_state.render_dist; z++){
        xyzs_to_load.insert(XYZ(-pcx + x*CHUNK_SIZE, -pcy + y*CHUNK_SIZE, -pcz + z*CHUNK_SIZE));
      }
    }
  }

  for (unordered_set<XYZ>::iterator it=xyzs_to_load.begin(); it != xyzs_to_load.end(); it++){
    state.chunk_action_reqs.push(new Load_Chunk_Action_Req(*it));
  }
  chunk_loader_lock.unlock();
}

void set_chunks(bool player_moved_chunks, unordered_set<XYZ> changed_chunks){
  int pcx = -snap_to_chunk(render_state.player_x);
  int pcy = -snap_to_chunk(render_state.player_y);
  int pcz = -snap_to_chunk(render_state.player_z);

  unordered_set<XYZ> xyzs_to_unload;
  unordered_set<XYZ> xyzs_to_load;

  if (player_moved_chunks){
    int lcx = -snap_to_chunk(state.last_player_x);
    int lcy = -snap_to_chunk(state.last_player_y);
    int lcz = -snap_to_chunk(state.last_player_z);

    if (pcx != lcx){
      int load_cx = pcx + (pcx - lcx)*render_state.render_dist;
      int unload_cx = pcx + (lcx - pcx)*(render_state.render_dist+1);
      for (int y = -render_state.render_dist; y <= render_state.render_dist; y++){
        for (int z = -render_state.render_dist; z <= render_state.render_dist; z++){
          int cy = pcy + y*CHUNK_SIZE;
          int cz = pcz + z*CHUNK_SIZE;
          xyzs_to_load.insert(XYZ(load_cx, cy, cz));
          xyzs_to_unload.insert(XYZ(unload_cx, cy, cz));
        }
      }
    }
    if (pcy != lcy){
      int load_cy = pcy + (pcy - lcy)*render_state.render_dist;
      int unload_cy = pcy + (lcy - pcy)*(render_state.render_dist+1);
      for (int x = -render_state.render_dist; x <= render_state.render_dist; x++){
        for (int z = -render_state.render_dist; z <= render_state.render_dist; z++){
          int cx = pcx + x*CHUNK_SIZE;
          int cz = pcz + z*CHUNK_SIZE;
          xyzs_to_load.insert(XYZ(cx, load_cy, cz));
          xyzs_to_unload.insert(XYZ(cx, unload_cy, cz));
        }
      }
    }
    if (pcz != lcz){
      int load_cz = pcz + (pcz - lcz)*render_state.render_dist;
      int unload_cz = pcz + (lcz - pcz)*(render_state.render_dist+1);
      for (int y = -render_state.render_dist; y <= render_state.render_dist; y++){
        for (int x = -render_state.render_dist; x <= render_state.render_dist; x++){
          int cy = pcy + y*CHUNK_SIZE;
          int cx = pcx + x*CHUNK_SIZE;
          xyzs_to_load.insert(XYZ(cx, cy, load_cz));
          xyzs_to_unload.insert(XYZ(cx, cy, unload_cz));
        }
      }
    }
  }

  for (unordered_set<XYZ>::iterator it=changed_chunks.begin(); it != changed_chunks.end(); it++){
    int x, y, z;
    tie(x, y, z) = *it;

    int horizon = CHUNK_SIZE*render_state.render_dist;
    if (   pcx - horizon < x && pcx + horizon > x
        && pcy - horizon < y && pcy + horizon > y
        && pcz - horizon < z && pcz + horizon > z){

      xyzs_to_load.insert(XYZ(x, y, z));
    }
  }

  bool has_change = false;

  for (unordered_set<XYZ>::iterator it=xyzs_to_unload.begin(); it != xyzs_to_unload.end(); it++){
    state.chunk_action_reqs.push(new Unload_Chunk_Action_Req(*it));
    has_change = true;
  }

  for (unordered_set<XYZ>::iterator it=xyzs_to_load.begin(); it != xyzs_to_load.end(); it++){
    state.chunk_action_reqs.push(new Load_Chunk_Action_Req(*it));
    has_change = true;
  }

  if (has_change){
    chunk_loader_lock.unlock();
  }
}
