#include "chunk.h"
#include <vector>
#include <map>
#include <tuple>

#include <iostream>
using namespace std;

void draw_chunk(Chunk& chunk){
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

  /* Push each element in buffer_vertices to the vertex shader */
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk.ibo_chunk_elements);
  int size;  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
  glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

  glDisableVertexAttribArray(chunk.attribute_coord3d);
  glDisableVertexAttribArray(chunk.attribute_v_color);
}

bool init_chunk(Chunk& chunk, GLuint program, int cx, int cy, int cz){

  chunk.x = cx;
  chunk.y = cy;
  chunk.z = cz;

  vector<GLfloat> chunk_vertices;

  int D = 16;

  int X = D;
  int Y = D;
  int Z = D;

  map< tuple<int,int,int>, GLushort > vertex_indices;

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

  vector<GLushort> chunk_elements;

  for (int x = 0; x < X; x++){
    for (int y = 0; y < Y; y++){
      for (int z = 0; z < Z; z++){
        //if (rand() % 8 < 7)
        //  continue;
        if (cy + y > (rand() % 64))
          continue;
        unsigned short e000 = vertex_indices[tuple<int,int,int>(x, y, z)];
        unsigned short e001 = vertex_indices[tuple<int,int,int>(x, y, z+1)];
        unsigned short e010 = vertex_indices[tuple<int,int,int>(x, y+1, z)];
        unsigned short e011 = vertex_indices[tuple<int,int,int>(x, y+1, z+1)];
        unsigned short e100 = vertex_indices[tuple<int,int,int>(x+1, y, z)];
        unsigned short e101 = vertex_indices[tuple<int,int,int>(x+1, y, z+1)];
        unsigned short e110 = vertex_indices[tuple<int,int,int>(x+1, y+1, z)];
        unsigned short e111 = vertex_indices[tuple<int,int,int>(x+1, y+1, z+1)];
        vector<GLushort> cube_elements({
          // front
          e001, e101, e111,
          e111, e011, e001,
          // top
          e101, e100, e110,
          e110, e111, e101,
          // back
          e010, e110, e100,
          e100, e000, e010,
          // bottom
          e000, e001, e011,
          e011, e010, e000,
          // left
          e000, e100, e101,
          e101, e001, e000,
          // right
          e011, e111, e110,
          e110, e010, e011,
        });
        chunk_elements.insert(chunk_elements.begin(), cube_elements.begin(), cube_elements.end());
      }
    }
  }


  glGenBuffers(1, &chunk.vbo_chunk_vertices);
  glBindBuffer(GL_ARRAY_BUFFER, chunk.vbo_chunk_vertices);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*chunk_vertices.size(), &chunk_vertices[0], GL_STATIC_DRAW);

  glGenBuffers(1, &chunk.vbo_chunk_colors);
  glBindBuffer(GL_ARRAY_BUFFER, chunk.vbo_chunk_colors);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*chunk_colors.size(), &chunk_colors[0], GL_STATIC_DRAW);

  glGenBuffers(1, &chunk.ibo_chunk_elements);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk.ibo_chunk_elements);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*chunk_elements.size(), &chunk_elements[0], GL_STATIC_DRAW);

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
