#include "chunk.h"
#include <vector>
#include <map>
#include <tuple>

#include <iostream>
using namespace std;

vector<GLfloat> chunk_vertices;
GLuint vbo_chunk_vertices;
map< tuple<int,int,int>, GLushort > vertex_indices;

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

        tuple<int, int, int> coords(x, y, z);

        vertex_indices.insert(pair< tuple<int,int,int>, GLushort >(coords, index));

        index += 1;
      }
    }
  }
}

int draw_chunk(Chunk& chunk, double x, double y, double z, double rx, double ry, double rz, float fov){
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

  int size;

  bool draw_x_pos = -x > chunk.x;
  bool draw_x_neg = -x < chunk.x + D;
  bool draw_y_pos = -y > chunk.y;
  bool draw_y_neg = -y < chunk.y + D;
  bool draw_z_pos = -z > chunk.z;
  bool draw_z_neg = -z < chunk.z + D;

  int drawn = 0;

  if (draw_x_pos){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk.ibo_chunk_x_pos_elements);
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
    drawn += 1;
  }

  if (draw_x_neg){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk.ibo_chunk_x_neg_elements);
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
    drawn += 1;
  }

  if (draw_y_pos){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk.ibo_chunk_y_pos_elements);
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
    drawn += 1;
  }

  if (draw_y_neg){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk.ibo_chunk_y_neg_elements);
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
    drawn += 1;
  }

  if (draw_z_pos){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk.ibo_chunk_z_pos_elements);
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
    drawn += 1;
  }

  if (draw_z_neg){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk.ibo_chunk_z_neg_elements);
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
    drawn += 1;
  }

  glDisableVertexAttribArray(chunk.attribute_coord3d);
  glDisableVertexAttribArray(chunk.attribute_v_color);

  return drawn;
}

void unload_chunk(Chunk& chunk){
  //glDeleteBuffers(1, &vbo_chunk_vertices);
  glDeleteBuffers(1, &chunk.vbo_chunk_colors);
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
  }

  chunk.vbo_chunk_vertices = vbo_chunk_vertices;

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

  vector<GLfloat> chunk_colors;

  for (int i = 0; i < X*Y*Z; i++){
    chunk_colors.insert(chunk_colors.end(), cube_colors.begin(), cube_colors.end());
  }

  vector<GLushort> chunk_x_pos_elements;
  vector<GLushort> chunk_x_neg_elements;
  vector<GLushort> chunk_y_pos_elements;
  vector<GLushort> chunk_y_neg_elements;
  vector<GLushort> chunk_z_pos_elements;
  vector<GLushort> chunk_z_neg_elements;

  for (int x = 0; x < X; x++){
    for (int y = 0; y < Y; y++){
      for (int z = 0; z < Z; z++){
        //if (rand() % 8 < 7)
        //  continue;
        //if (cy + y > (rand() % 64))
        //  continue;
        if (!get_block(world, cx+x, cy+y, cz+z))
          continue;
        unsigned short e000 = vertex_indices[tuple<int,int,int>(x, y, z)];
        unsigned short e001 = vertex_indices[tuple<int,int,int>(x, y, z+1)];
        unsigned short e010 = vertex_indices[tuple<int,int,int>(x, y+1, z)];
        unsigned short e011 = vertex_indices[tuple<int,int,int>(x, y+1, z+1)];
        unsigned short e100 = vertex_indices[tuple<int,int,int>(x+1, y, z)];
        unsigned short e101 = vertex_indices[tuple<int,int,int>(x+1, y, z+1)];
        unsigned short e110 = vertex_indices[tuple<int,int,int>(x+1, y+1, z)];
        unsigned short e111 = vertex_indices[tuple<int,int,int>(x+1, y+1, z+1)];

        vector<GLushort> cube_x_pos_elements({
          e101, e100, e110,
          e110, e111, e101,
        });
        chunk_x_pos_elements.insert(chunk_x_pos_elements.begin(), cube_x_pos_elements.begin(), cube_x_pos_elements.end());

        vector<GLushort> cube_x_neg_elements({
          e000, e001, e011,
          e011, e010, e000,
        });
        chunk_x_neg_elements.insert(chunk_x_neg_elements.begin(), cube_x_neg_elements.begin(), cube_x_neg_elements.end());

        vector<GLushort> cube_y_pos_elements({
          e011, e111, e110,
          e110, e010, e011,
        });
        chunk_y_pos_elements.insert(chunk_y_pos_elements.begin(), cube_y_pos_elements.begin(), cube_y_pos_elements.end());

        vector<GLushort> cube_y_neg_elements({
          e000, e100, e101,
          e101, e001, e000,
        });
        chunk_y_neg_elements.insert(chunk_y_neg_elements.begin(), cube_y_neg_elements.begin(), cube_y_neg_elements.end());

        vector<GLushort> cube_z_pos_elements({
          e001, e101, e111,
          e111, e011, e001,
        });
        chunk_z_pos_elements.insert(chunk_z_pos_elements.begin(), cube_z_pos_elements.begin(), cube_z_pos_elements.end());

        vector<GLushort> cube_z_neg_elements({
          e010, e110, e100,
          e100, e000, e010,
        });
        chunk_z_neg_elements.insert(chunk_z_neg_elements.begin(), cube_z_neg_elements.begin(), cube_z_neg_elements.end());
      }
    }
  }



  glGenBuffers(1, &chunk.vbo_chunk_colors);
  glBindBuffer(GL_ARRAY_BUFFER, chunk.vbo_chunk_colors);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*chunk_colors.size(), &chunk_colors[0], GL_STATIC_DRAW);

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
