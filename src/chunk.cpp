#include "chunk.h"
#include <vector>
#include <map>
#include <tuple>
#include "state.h"
#include "constants.h"
#include "utils.h"

#include <iostream>
using namespace std;


double vec_3d_angle(double fwd_x, double fwd_y, double fwd_z,
                    double chunk_camera_x, double chunk_camera_y, double chunk_camera_z){

  double fwd_norm = sqrt(fwd_x*fwd_x + fwd_y*fwd_y + fwd_z*fwd_z);
  double chunk_camera_norm = sqrt(chunk_camera_x*chunk_camera_x + chunk_camera_y*chunk_camera_y + chunk_camera_z*chunk_camera_z);

  double chunk_camera_angle = acos((fwd_x*chunk_camera_x + 
                                    fwd_y*chunk_camera_y + 
                                    fwd_z*chunk_camera_z)/(fwd_norm*chunk_camera_norm))*180/M_PI;
  return chunk_camera_angle;
}

int draw_faces(Chunk& chunk, Faces& faces){
  if (faces.empty){
    return 0;
  }
  glEnableVertexAttribArray(chunk.attribute_coord3d);

  int size;

  glBindBuffer(GL_ARRAY_BUFFER, faces.vbo_vertices);
  glVertexAttribPointer(
    chunk.attribute_coord3d, // attribute
    3,                 // number of elements per vertex, here (x,y,z)
    GL_FLOAT,          // the type of each element
    GL_FALSE,          // take our values as-is
    0,                 // no extra data between each position
    0                  // offset of first element
  );

  glEnableVertexAttribArray(chunk.attribute_v_tex_coord);
  glBindBuffer(GL_ARRAY_BUFFER, faces.vbo_tex_coords);
  glVertexAttribPointer(
    chunk.attribute_v_tex_coord, // attribute
    2,                 // number of elements per vertex, here (R,G,B)
    GL_FLOAT,          // the type of each element
    GL_FALSE,          // take our values as-is
    0,                 // no extra data between each position
    0                  // offset of first element
  );


  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faces.ibo_elements);
  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
  glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

  glDisableVertexAttribArray(chunk.attribute_coord3d);
  glDisableVertexAttribArray(chunk.attribute_v_tex_coord);

  return size;
}

int draw_chunk(glm::mat4 projection_view, Chunk& chunk, double x, double y, double z, double rx, double ry, double rz, float fov){

  if (chunk.empty){
    return 0;
  }

  double cos_ang = cos(ry/180*M_PI);
  double sin_ang = sin(ry/180*M_PI);

  double sin_vert_ang = sin(rx/180*M_PI);
  double cos_vert_ang = cos(rx/180*M_PI);

  double fwd_x = cos_vert_ang*sin_ang;
  double fwd_z = -cos_vert_ang*cos_ang;
  double fwd_y = -sin_vert_ang;

  bool chunk_in_fov = false;

  for (int cx = 0; cx <= 1; cx++){
    for (int cy = 0; cy <= 1; cy++){
      for (int cz = 0; cz <= 1; cz++){

        double chunk_camera_x = chunk.x + cx*CHUNK_SIZE + x; 
        double chunk_camera_y = chunk.y + cy*CHUNK_SIZE + y; 
        double chunk_camera_z = chunk.z + cz*CHUNK_SIZE + z; 

        double chunk_camera_angle = vec_3d_angle(fwd_x, fwd_y, fwd_z, chunk_camera_x, chunk_camera_y, chunk_camera_z);
        bool in_view = isnan(chunk_camera_angle) || chunk_camera_angle < fov;

        chunk_in_fov = chunk_in_fov || in_view;

        if (chunk_in_fov)
          break;
      }
      if (chunk_in_fov)
        break;
    }
    if (chunk_in_fov)
      break;
  }

  if (!chunk_in_fov)
    return 0;

  bool draw_x_pos = -x > chunk.x;
  bool draw_x_neg = -x < chunk.x + CHUNK_SIZE;
  bool draw_y_pos = -y > chunk.y;
  bool draw_y_neg = -y < chunk.y + CHUNK_SIZE;
  bool draw_z_pos = -z > chunk.z;
  bool draw_z_neg = -z < chunk.z + CHUNK_SIZE;

  int drawn = 0;

  glm::vec3 axis_y(1, 0, 0);
  glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(chunk.x, chunk.y, chunk.z));

  glm::mat4 mvp = projection_view * model;
  glUniformMatrix4fv(render_state.uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));

  if (draw_x_pos){
    drawn += draw_faces(chunk, chunk.x_pos);
  }

  if (draw_x_neg){
    drawn += draw_faces(chunk, chunk.x_neg);
  }

  if (draw_y_pos){
    drawn += draw_faces(chunk, chunk.y_pos);
  }

  if (draw_y_neg){
    drawn += draw_faces(chunk, chunk.y_neg);
  }

  if (draw_z_pos){
    drawn += draw_faces(chunk, chunk.z_pos);
  }

  if (draw_z_neg){
    drawn += draw_faces(chunk, chunk.z_neg);
  }

  return drawn;
}

void unload_faces(Faces& faces){
  glDeleteBuffers(1, &faces.vbo_vertices);
  glDeleteBuffers(1, &faces.vbo_tex_coords);
  glDeleteBuffers(1, &faces.ibo_elements);
}


void unload_chunk(Chunk* chunk){
  unload_faces(chunk->x_pos);
  unload_faces(chunk->x_neg);
  unload_faces(chunk->y_pos);
  unload_faces(chunk->y_neg);
  unload_faces(chunk->z_pos);
  unload_faces(chunk->z_neg);
  delete chunk;
}

void init_faces_elems(Faces& faces, int cx, int cy, int cz, World& world){
  faces.elements.clear();
  faces.vertices.clear();
  faces.tex_coords.clear();

  faces.index = 0;
}

void add_face(Faces& faces, int axis, int axis_val, int x, int y, int z){
  vector<GLfloat> face_tex_coords({
    0.0, 0.0,
    0.0, 1.0,
    1.0, 0.0,
    1.0, 1.0,
  });

  vector<GLushort> face_elements({
    (short unsigned int)(faces.index+2), (short unsigned int)(faces.index+1), (short unsigned int)(faces.index+0),
    (short unsigned int)(faces.index+3), (short unsigned int)(faces.index+1), (short unsigned int)(faces.index+2),
  });

  vector<GLfloat> face_vertices;
  if (axis == 0 && axis_val == 1){
    face_vertices = {
      x+1.f, y+0.f, z+0.f,
      x+1.f, y+0.f, z+1.f,            
      x+1.f, y+1.f, z+0.f,
      x+1.f, y+1.f, z+1.f,
    };
  } else if (axis == 0 && axis_val == 0){
    face_vertices = {
      x+0.f, y+0.f, z+0.f,
      x+0.f, y+1.f, z+0.f,
      x+0.f, y+0.f, z+1.f,            
      x+0.f, y+1.f, z+1.f,
    };
  } else if (axis == 1 && axis_val == 1){
    face_vertices = {
      x+0.f, y+1.f, z+0.f,
      x+1.f, y+1.f, z+0.f,
      x+0.f, y+1.f, z+1.f,            
      x+1.f, y+1.f, z+1.f,
    };
  } else if (axis == 1 && axis_val == 0){
    face_vertices = {
      x+0.f, y+0.f, z+0.f,
      x+0.f, y+0.f, z+1.f,            
      x+1.f, y+0.f, z+0.f,
      x+1.f, y+0.f, z+1.f,
    };
  } else if (axis == 2 && axis_val == 1){
    face_vertices = {
      x+0.f, y+0.f, z+1.f,
      x+0.f, y+1.f, z+1.f,            
      x+1.f, y+0.f, z+1.f,
      x+1.f, y+1.f, z+1.f,
    };
  } else if (axis == 2 && axis_val == 0){
    face_vertices = {
      x+0.f, y+0.f, z+0.f,
      x+1.f, y+0.f, z+0.f,
      x+0.f, y+1.f, z+0.f,            
      x+1.f, y+1.f, z+0.f,
    };
  }
  faces.index += 4;
  faces.vertices.insert(faces.vertices.begin(), face_vertices.begin(), face_vertices.end());
  faces.tex_coords.insert(faces.tex_coords.begin(), face_tex_coords.begin(), face_tex_coords.end());
  faces.elements.insert(faces.elements.begin(), face_elements.begin(), face_elements.end());
}

void init_chunk_cubes(Chunk& chunk, int cx, int cy, int cz, World& world){

  chunk.x = cx;
  chunk.y = cy;
  chunk.z = cz;

  init_faces_elems(chunk.x_pos, cx, cy, cz, world);
  init_faces_elems(chunk.x_neg, cx, cy, cz, world);
  init_faces_elems(chunk.y_pos, cx, cy, cz, world);
  init_faces_elems(chunk.y_neg, cx, cy, cz, world);
  init_faces_elems(chunk.z_pos, cx, cy, cz, world);
  init_faces_elems(chunk.z_neg, cx, cy, cz, world);

  int X = CHUNK_SIZE;
  int Y = CHUNK_SIZE;
  int Z = CHUNK_SIZE;

  for (int x = 0; x < X; x++){
    for (int y = 0; y < Y; y++){
      for (int z = 0; z < Z; z++){
        if (!get_block(world, cx+x, cy+y, cz+z))
          continue;
        bool x_pos_block = get_block(world, cx+x+1, cy+y, cz+z);
        bool x_neg_block = get_block(world, cx+x-1, cy+y, cz+z);
        bool y_pos_block = get_block(world, cx+x, cy+y+1, cz+z);
        bool y_neg_block = get_block(world, cx+x, cy+y-1, cz+z);
        bool z_pos_block = get_block(world, cx+x, cy+y, cz+z+1);
        bool z_neg_block = get_block(world, cx+x, cy+y, cz+z-1);

        if (!x_pos_block){
          add_face(chunk.x_pos, 0, 1, x, y, z);
        }

        if (!x_neg_block){
          add_face(chunk.x_neg, 0, 0, x, y, z);
        }

        if (!y_pos_block){
          add_face(chunk.y_pos, 1, 1, x, y, z);
        }

        if (!y_neg_block){
          add_face(chunk.y_neg, 1, 0, x, y, z);
        }

        if (!z_pos_block){
          add_face(chunk.z_pos, 2, 1, x, y, z);
        }

        if (!z_neg_block){
          add_face(chunk.z_neg, 2, 0, x, y, z);
        }
      }
    }
  }

  chunk.x_pos.empty = chunk.x_pos.elements.size() == 0;
  chunk.x_neg.empty = chunk.x_neg.elements.size() == 0;
  chunk.y_pos.empty = chunk.y_pos.elements.size() == 0;
  chunk.y_neg.empty = chunk.y_neg.elements.size() == 0;
  chunk.z_pos.empty = chunk.z_pos.elements.size() == 0;
  chunk.z_neg.empty = chunk.z_neg.elements.size() == 0;

  chunk.empty = chunk.x_pos.empty && 
                chunk.x_neg.empty && 
                chunk.y_pos.empty && 
                chunk.y_neg.empty && 
                chunk.z_pos.empty && 
                chunk.z_neg.empty;
}

void init_faces_gl(Faces& faces){
  glGenBuffers(1, &faces.vbo_vertices);
  glBindBuffer(GL_ARRAY_BUFFER, faces.vbo_vertices);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*faces.vertices.size(), &faces.vertices[0], GL_STATIC_DRAW);

  glGenBuffers(1, &faces.vbo_tex_coords);
  glBindBuffer(GL_ARRAY_BUFFER, faces.vbo_tex_coords);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*faces.tex_coords.size(), &faces.tex_coords[0], GL_STATIC_DRAW);

  glGenBuffers(1, &faces.ibo_elements);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faces.ibo_elements);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*faces.elements.size(), &faces.elements[0], GL_STATIC_DRAW);
}

bool init_chunk_gl(Chunk& chunk, GLuint program, int cx, int cy, int cz, World& world){

  init_faces_gl(chunk.x_pos);
  init_faces_gl(chunk.x_neg);
  init_faces_gl(chunk.y_pos);
  init_faces_gl(chunk.y_neg);
  init_faces_gl(chunk.z_pos);
  init_faces_gl(chunk.z_neg);

  const char* attribute_name;
  attribute_name = "coord3d";
  chunk.attribute_coord3d = glGetAttribLocation(program, attribute_name);
  if (chunk.attribute_coord3d == -1) {
    cerr << "Could not bind attribute " << attribute_name << endl;
    return false;
  }
  attribute_name = "v_tex_coord";
  chunk.attribute_v_tex_coord = glGetAttribLocation(program, attribute_name);
  if (chunk.attribute_v_tex_coord == -1) {
    cerr << "Could not bind attribute " << attribute_name << endl;
    return false;
  }
  return true;
}

bool init_chunk(Chunk& chunk, GLuint program, int cx, int cy, int cz, World& world){
  init_chunk_cubes(chunk, cx, cy, cz, world);
  return init_chunk_gl(chunk, program, cx, cy, cz, world);

}
