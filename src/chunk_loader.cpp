
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

void process_chunk_action_ress(){
  vector<Chunk_Action_Res*> v = state.chunk_action_stream.multi_nowait_pop_res();

  for (int i = 0; i < v.size(); i++){
    Chunk_Action_Res* res = v[i];
    if (res->type() == Chunk_Action_Ress::Unload_Chunk_Res) {
      Unload_Chunk_Action_Res* u_res = static_cast<Unload_Chunk_Action_Res*>(res);

      if (render_state.chunks.find(u_res->xyz) != render_state.chunks.end()){
        Chunk& c = *render_state.chunks.find(u_res->xyz)->second;
        render_state.chunks.erase(u_res->xyz);
        unload_chunk(&c);
      }
    } else if (res->type() == Chunk_Action_Ress::Load_Chunk_Res) {
      Load_Chunk_Action_Res* l_res = static_cast<Load_Chunk_Action_Res*>(res);

      int x, y, z;
      tie(x, y, z) = l_res->xyz;

      if (render_state.chunks.find(l_res->xyz) != render_state.chunks.end()){
        Chunk& c = *render_state.chunks.find(l_res->xyz)->second;
        render_state.chunks.erase(l_res->xyz);
        unload_chunk(&c);
      }

      Chunk& c = l_res->chunk;

      if (!init_chunk_gl(c, render_state.program, x, y, z, state.world)){
        throw runtime_error("could not init chunk");
      }

      render_state.chunks.insert({l_res->xyz, &c});
    }
    delete res;
  }
}

void chunk_loader(void){
  while (true){
    pair<Chunk_Action_Req*, unsigned int> req = state.chunk_action_stream.pop_req();

    if (req.first->type() == Chunk_Action_Reqs::Unload_Chunk_Req) {
      Unload_Chunk_Action_Req* u_req = static_cast<Unload_Chunk_Action_Req*>(req.first);

      state.chunk_action_stream.push_res(new Unload_Chunk_Action_Res(u_req->xyz), req.second);

    // ==============================================================
    } else if (req.first->type() == Chunk_Action_Reqs::Load_Chunk_Req) {
      Load_Chunk_Action_Req* l_req = static_cast<Load_Chunk_Action_Req*>(req.first);
      int x, y, z;
      tie(x, y, z) = l_req->xyz;

      Chunk* chunk = new Chunk;
      init_chunk_cubes(*chunk, x, y, z, state.world);

      state.chunk_action_stream.push_res(new Load_Chunk_Action_Res(l_req->xyz, *chunk), req.second);
    }

    delete req.first;
  }
}


void start_chunk_loader_thread(){
  boost::thread thread(chunk_loader);
}

void load_all_chunks(){
  static int pcx = -snap_to_chunk(render_state.player_x);
  static int pcy = -snap_to_chunk(render_state.player_y);
  static int pcz = -snap_to_chunk(render_state.player_z);

  static int px = render_state.player_x;
  static int py = render_state.player_y;
  static int pz = render_state.player_z;

  struct dist_compare {
    bool operator() (const XYZ& lhs, const XYZ& rhs) const{
      int lx, ly, lz;
      int rx, ry, rz;
      tie(lx, ly, lz) = lhs;
      tie(rx, ry, rz) = rhs;

      double rd = sqrt(pow(rx - px, 2) + pow(ry - py, 2) + pow(rz - pz, 2));
      double ld = sqrt(pow(lx - px, 2) + pow(ly - py, 2) + pow(lz - pz, 2));
      if (rd != ld){
        return ld < rd;
      } else if (rx != lx){
        return rx < lx;
      } else if (ry != ly){
        return ry < ly;
      } else if (rz != lz){
        return rz < lz;
      } else {
        return true;

      }
    }
  };

  set<XYZ, dist_compare> xyzs_to_load;
  for (int x = -render_state.render_dist; x <= render_state.render_dist; x++){
    for (int y = -render_state.render_dist; y <= render_state.render_dist; y++){
      for (int z = -render_state.render_dist; z <= render_state.render_dist; z++){
        xyzs_to_load.insert(XYZ(-pcx + x*CHUNK_SIZE, -pcy + y*CHUNK_SIZE, -pcz + z*CHUNK_SIZE));
      }
    }
  }

  for (set<XYZ>::iterator it=xyzs_to_load.begin(); it != xyzs_to_load.end(); it++){
    state.chunk_action_stream.push_req(new Load_Chunk_Action_Req(*it));
  }
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
    if (   pcx - horizon <= x && pcx + horizon >= x
        && pcy - horizon <= y && pcy + horizon >= y
        && pcz - horizon <= z && pcz + horizon >= z){

      xyzs_to_load.insert(XYZ(x, y, z));
    }
  }

  for (unordered_set<XYZ>::iterator it=xyzs_to_unload.begin(); it != xyzs_to_unload.end(); it++){
    state.chunk_action_stream.push_req(new Unload_Chunk_Action_Req(*it));
  }

  for (unordered_set<XYZ>::iterator it=xyzs_to_load.begin(); it != xyzs_to_load.end(); it++){
    state.chunk_action_stream.push_req(new Load_Chunk_Action_Req(*it));
  }
}
