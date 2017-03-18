#ifndef CHUNK_LOADER_
#define CHUNK_LOADER_

#include "utils.h"
#include "chunk.h"

#include <boost/unordered_set.hpp>
using boost::unordered_set;

enum Chunk_Action_Reqs { CA_Req_Base, Load_Chunk_Req, Unload_Chunk_Req };
enum Chunk_Action_Ress { CA_Res_Base, Load_Chunk_Res, Unload_Chunk_Res };

class Chunk_Action_Req {
  public:
    virtual Chunk_Action_Reqs type() { return Chunk_Action_Reqs::CA_Req_Base; }
};

class Chunk_Action_Res {
  public:
    virtual Chunk_Action_Ress type() { return Chunk_Action_Ress::CA_Res_Base; }
};

class Load_Chunk_Action_Req : public Chunk_Action_Req {
  public:
    Chunk_Action_Reqs type() { return Chunk_Action_Reqs::Load_Chunk_Req; }
    XYZ xyz;
    Load_Chunk_Action_Req(XYZ _xyz) : 
          xyz(_xyz){ }
};

class Unload_Chunk_Action_Req : public Chunk_Action_Req {
  public:
    Chunk_Action_Reqs type() { return Chunk_Action_Reqs::Unload_Chunk_Req; }
    XYZ xyz;
    Unload_Chunk_Action_Req(XYZ _xyz) : 
          xyz(_xyz){ }
};

class Unload_Chunk_Action_Res : public Chunk_Action_Res {
  public:
    Chunk_Action_Ress type() { return Chunk_Action_Ress::Unload_Chunk_Res; }
    XYZ xyz;
    Unload_Chunk_Action_Res(XYZ _xyz) : 
          xyz(_xyz){ }
};

class Load_Chunk_Action_Res : public Chunk_Action_Res {
  public:
    Chunk_Action_Ress type() { return Chunk_Action_Ress::Load_Chunk_Res; }
    XYZ xyz;
    Chunk& chunk;
    Load_Chunk_Action_Res(XYZ _xyz, Chunk& _chunk) : 
          xyz(_xyz),
          chunk(_chunk){ }
};

void set_chunks(bool player_moved_chunks, unordered_set<XYZ> changed_chunks);
void load_all_chunks();
void process_chunk_action_ress();
void start_chunk_loader_thread();

#endif
