

#include "async_chunk_action_stream.h"

void Async_Chunk_Action_Stream::push_req(Chunk_Action_Req* req) {
  chunk_action_reqs.push({ req, next_req });
  next_req += 1;
}

void Async_Chunk_Action_Stream::push_res(Chunk_Action_Res* res, unsigned int i){
  chunk_action_ress.push({ res, i });
}

vector<Chunk_Action_Res*> Async_Chunk_Action_Stream::multi_nowait_pop_res() {
  vector<pair<Chunk_Action_Res*, unsigned int>> v_unordered = chunk_action_ress.multi_pop_nowait();

  for (int i = 0; i < v_unordered.size(); i++){
    chunk_action_ress_pq.push(v_unordered[i]);
  }

  vector<Chunk_Action_Res*> ress;

  while (chunk_action_ress_pq.size() > 0 && chunk_action_ress_pq.top().second == next_res){
    ress.push_back(chunk_action_ress_pq.top().first);
    chunk_action_ress_pq.pop();
    next_res += 1;
  }
  return ress;
}

pair<Chunk_Action_Req*, unsigned int> Async_Chunk_Action_Stream::pop_req() {
  return chunk_action_reqs.pop();
}
