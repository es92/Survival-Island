#include "chunk.h"
#include <vector>
#include <map>
#include <tuple>
#include "state.h"
#include "utils.h"

#include <iostream>
using namespace std;

vector<GLfloat> chunk_vertices;
GLuint vbo_chunk_vertices;
map< XYZ, GLushort > vertex_indices;

GLuint vbo_chunk_colors;
vector<GLfloat> chunk_colors;

int D = 16;

void init_chunk_vertices(){

  int X = D;
  int Y = D;
  int Z = D;


  unsigned short index = 0;
  for (int x = 0; x <= X; x++){
    for (int y = 0; y <= Y; y++){
      for (int z = 0; z <= Z; z++){
        chunk_vertices.push_back(x);
        chunk_vertices.push_back(y);
        chunk_vertices.push_back(z);

        XYZ coords(x, y, z);

        vertex_indices.insert(pair< XYZ, GLushort >(coords, index));

        index += 1;
      }
    }
  }

  vector<GLfloat> cube_colors({
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 1.0, 1.0,
    1.0, 1.0, 1.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
  });


  for (int i = 0; i < X*Y*Z; i++){
    chunk_colors.insert(chunk_colors.end(), cube_colors.begin(), cube_colors.end());
  }

}

double vec_3d_angle(double fwd_x, double fwd_y, double fwd_z,
                    double chunk_camera_x, double chunk_camera_y, double chunk_camera_z){

  double fwd_norm = sqrt(fwd_x*fwd_x + fwd_y*fwd_y + fwd_z*fwd_z);
  double chunk_camera_norm = sqrt(chunk_camera_x*chunk_camera_x + chunk_camera_y*chunk_camera_y + chunk_camera_z*chunk_camera_z);

  double chunk_camera_angle = acos((fwd_x*chunk_camera_x + 
                                    fwd_y*chunk_camera_y + 
                                    fwd_z*chunk_camera_z)/(fwd_norm*chunk_camera_norm))*180/M_PI;
  return chunk_camera_angle;
}

int draw_chunk(glm::mat4 projection_view, Chunk& chunk, double x, double y, double z, double rx, double ry, double rz, float fov){

  if (chunk.empty){
    return 0;
  }

  int size;

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

        double chunk_camera_x = chunk.x + cx*D + x; 
        double chunk_camera_y = chunk.y + cy*D + y; 
        double chunk_camera_z = chunk.z + cz*D + z; 

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
  bool draw_x_neg = -x < chunk.x + D;
  bool draw_y_pos = -y > chunk.y;
  bool draw_y_neg = -y < chunk.y + D;
  bool draw_z_pos = -z > chunk.z;
  bool draw_z_neg = -z < chunk.z + D;

  int drawn = 0;

  glEnableVertexAttribArray(chunk.attribute_coord3d);
  // Describe our vertices array to OpenGL (it can't guess its format automatically)
  glBindBuffer(GL_ARRAY_BUFFER, chunk.vbo_chunk_vertices);
  glVertexAttribPointer(
    chunk.attribute_coord3d, // attribute
    3,                 // number of elements per vertex, here (x,y,z)
    GL_FLOAT,          // the type of each element
    GL_FALSE,          // take our values as-is
    0,                 // no extra data between each position
    0                  // offset of first element
  );

  glEnableVertexAttribArray(chunk.attribute_v_color);
  glBindBuffer(GL_ARRAY_BUFFER, chunk.vbo_chunk_colors);
  glVertexAttribPointer(
    chunk.attribute_v_color, // attribute
    3,                 // number of elements per vertex, here (R,G,B)
    GL_FLOAT,          // the type of each element
    GL_FALSE,          // take our values as-is
    0,                 // no extra data between each position
    0                  // offset of first element
  );

  glm::vec3 axis_y(1, 0, 0);
  glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(chunk.x, chunk.y, chunk.z));

  glm::mat4 mvp = projection_view * model;
  glUniformMatrix4fv(render_state.uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));

  if (draw_x_pos){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk.ibo_chunk_x_pos_elements);
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
    drawn += size;
  }

  if (draw_x_neg){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk.ibo_chunk_x_neg_elements);
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
    drawn += size;
  }

  if (draw_y_pos){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk.ibo_chunk_y_pos_elements);
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
    drawn += size;
  }

  if (draw_y_neg){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk.ibo_chunk_y_neg_elements);
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
    drawn += size;
  }

  if (draw_z_pos){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk.ibo_chunk_z_pos_elements);
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
    drawn += size;
  }

  if (draw_z_neg){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk.ibo_chunk_z_neg_elements);
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
    drawn += size;
  }

  glDisableVertexAttribArray(chunk.attribute_coord3d);
  glDisableVertexAttribArray(chunk.attribute_v_color);

  return drawn;
}

void unload_chunk(Chunk& chunk){
  //glDeleteBuffers(1, &vbo_chunk_vertices);
  //glDeleteBuffers(1, &vbo_chunk_colors);
  glDeleteBuffers(1, &chunk.ibo_chunk_x_pos_elements);
  glDeleteBuffers(1, &chunk.ibo_chunk_x_neg_elements);
  glDeleteBuffers(1, &chunk.ibo_chunk_y_pos_elements);
  glDeleteBuffers(1, &chunk.ibo_chunk_y_neg_elements);
  glDeleteBuffers(1, &chunk.ibo_chunk_z_pos_elements);
  glDeleteBuffers(1, &chunk.ibo_chunk_z_neg_elements);
}

bool init_chunk(Chunk& chunk, GLuint program, int cx, int cy, int cz, World& world){

  chunk.x = cx;
  chunk.y = cy;
  chunk.z = cz;

  int X = D;
  int Y = D;
  int Z = D;

  if (chunk_vertices.size() == 0) {
    init_chunk_vertices();

    glGenBuffers(1, &vbo_chunk_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_chunk_vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*chunk_vertices.size(), &chunk_vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &vbo_chunk_colors);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_chunk_colors);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*chunk_colors.size(), &chunk_colors[0], GL_STATIC_DRAW);
  }

  chunk.vbo_chunk_vertices = vbo_chunk_vertices;
  chunk.vbo_chunk_colors = vbo_chunk_colors;

  vector<GLushort> chunk_x_pos_elements;
  vector<GLushort> chunk_x_neg_elements;
  vector<GLushort> chunk_y_pos_elements;
  vector<GLushort> chunk_y_neg_elements;
  vector<GLushort> chunk_z_pos_elements;
  vector<GLushort> chunk_z_neg_elements;

  for (int x = 0; x < X; x++){
    for (int y = 0; y < Y; y++){
      for (int z = 0; z < Z; z++){
        if (!get_block(world, cx+x, cy+y, cz+z))
          continue;
        unsigned short e000 = vertex_indices[XYZ(x, y, z)];
        unsigned short e001 = vertex_indices[XYZ(x, y, z+1)];
        unsigned short e010 = vertex_indices[XYZ(x, y+1, z)];
        unsigned short e011 = vertex_indices[XYZ(x, y+1, z+1)];
        unsigned short e100 = vertex_indices[XYZ(x+1, y, z)];
        unsigned short e101 = vertex_indices[XYZ(x+1, y, z+1)];
        unsigned short e110 = vertex_indices[XYZ(x+1, y+1, z)];
        unsigned short e111 = vertex_indices[XYZ(x+1, y+1, z+1)];

        bool x_pos_block = get_block(world, cx+x+1, cy+y, cz+z);
        bool x_neg_block = get_block(world, cx+x-1, cy+y, cz+z);
        bool y_pos_block = get_block(world, cx+x, cy+y+1, cz+z);
        bool y_neg_block = get_block(world, cx+x, cy+y-1, cz+z);
        bool z_pos_block = get_block(world, cx+x, cy+y, cz+z+1);
        bool z_neg_block = get_block(world, cx+x, cy+y, cz+z-1);

        if (!x_pos_block){
          vector<GLushort> cube_x_pos_elements({
            e101, e100, e110,
            e110, e111, e101,
          });
          chunk_x_pos_elements.insert(chunk_x_pos_elements.begin(), cube_x_pos_elements.begin(), cube_x_pos_elements.end());
        }

        if (!x_neg_block){
          vector<GLushort> cube_x_neg_elements({
            e000, e001, e011,
            e011, e010, e000,
          });
          chunk_x_neg_elements.insert(chunk_x_neg_elements.begin(), cube_x_neg_elements.begin(), cube_x_neg_elements.end());
        }

        if (!y_pos_block){
          vector<GLushort> cube_y_pos_elements({
            e011, e111, e110,
            e110, e010, e011,
          });
          chunk_y_pos_elements.insert(chunk_y_pos_elements.begin(), cube_y_pos_elements.begin(), cube_y_pos_elements.end());
        }

        if (!y_neg_block){
          vector<GLushort> cube_y_neg_elements({
            e000, e100, e101,
            e101, e001, e000,
          });
          chunk_y_neg_elements.insert(chunk_y_neg_elements.begin(), cube_y_neg_elements.begin(), cube_y_neg_elements.end());
        }

        if (!z_pos_block){
          vector<GLushort> cube_z_pos_elements({
            e001, e101, e111,
            e111, e011, e001,
          });
          chunk_z_pos_elements.insert(chunk_z_pos_elements.begin(), cube_z_pos_elements.begin(), cube_z_pos_elements.end());
        }

        if (!z_neg_block){
          vector<GLushort> cube_z_neg_elements({
            e010, e110, e100,
            e100, e000, e010,
          });
          chunk_z_neg_elements.insert(chunk_z_neg_elements.begin(), cube_z_neg_elements.begin(), cube_z_neg_elements.end());
        }
      }
    }
  }

  bool empty =  chunk_x_pos_elements.size() + 
                chunk_x_neg_elements.size() + 
                chunk_y_pos_elements.size() + 
                chunk_y_neg_elements.size() + 
                chunk_z_pos_elements.size() + 
                chunk_z_neg_elements.size() == 0;

  chunk.empty = empty;


  glGenBuffers(1, &chunk.ibo_chunk_x_pos_elements);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk.ibo_chunk_x_pos_elements);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*chunk_x_pos_elements.size(), &chunk_x_pos_elements[0], GL_STATIC_DRAW);

  glGenBuffers(1, &chunk.ibo_chunk_x_neg_elements);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk.ibo_chunk_x_neg_elements);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*chunk_x_neg_elements.size(), &chunk_x_neg_elements[0], GL_STATIC_DRAW);

  glGenBuffers(1, &chunk.ibo_chunk_y_pos_elements);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk.ibo_chunk_y_pos_elements);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*chunk_y_pos_elements.size(), &chunk_y_pos_elements[0], GL_STATIC_DRAW);

  glGenBuffers(1, &chunk.ibo_chunk_y_neg_elements);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk.ibo_chunk_y_neg_elements);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*chunk_y_neg_elements.size(), &chunk_y_neg_elements[0], GL_STATIC_DRAW);

  glGenBuffers(1, &chunk.ibo_chunk_z_pos_elements);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk.ibo_chunk_z_pos_elements);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*chunk_z_pos_elements.size(), &chunk_z_pos_elements[0], GL_STATIC_DRAW);

  glGenBuffers(1, &chunk.ibo_chunk_z_neg_elements);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk.ibo_chunk_z_neg_elements);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*chunk_z_neg_elements.size(), &chunk_z_neg_elements[0], GL_STATIC_DRAW);

  const char* attribute_name;
  attribute_name = "coord3d";
  chunk.attribute_coord3d = glGetAttribLocation(program, attribute_name);
  if (chunk.attribute_coord3d == -1) {
    cerr << "Could not bind attribute " << attribute_name << endl;
    return false;
  }
  attribute_name = "v_color";
  chunk.attribute_v_color = glGetAttribLocation(program, attribute_name);
  if (chunk.attribute_v_color == -1) {
    cerr << "Could not bind attribute " << attribute_name << endl;
    return false;
  }
  return true;
}
