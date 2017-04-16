cmake_minimum_required(VERSION 2.8)
PROJECT(My_Game)

#==================================================
# Find GLUT
#==================================================

find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})
if(NOT GLUT_FOUND)
    message(ERROR " GLUT not found!")
endif(NOT GLUT_FOUND)

#==================================================
# Find OPENGL
#==================================================

find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

#==================================================
# Find GLEW
#==================================================

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
link_libraries(${GLEW_LIBRARIES})
if(NOT GLEW_FOUND)
    message(ERROR " GLEW not found!")
endif(NOT GLEW_FOUND)

#==================================================
# Find BOOST
#==================================================

find_package( Boost COMPONENTS thread system filesystem REQUIRED ) #whatever libs you need
include_directories( ${Boost_INCLUDE_DIRS} )
find_package( Threads )

#==================================================
# Find SOIL
#==================================================

FIND_PATH(SOIL_INCLUDE_DIR SOIL.h PATH_SUFFIXES include/SOIL include)
SET(SOIL_NAMES SOIL Soil soil)
FIND_LIBRARY(SOIL_LIBRARY NAMES ${SOIL_NAMES} )

#==================================================
# Include Files
#==================================================
add_executable(my_game 
                        ../src/main.cpp 
                        ../src/display.cpp 
                        ../src/main_loop.cpp 
                        ../src/state.cpp

                        ../src/utils/utils.cpp 
                        ../src/utils/events.cpp
                        ../src/utils/shader_utils.cpp 
                        ../src/utils/async_chunk_action_stream.cpp 
                        ../src/utils/lazy_diamond_square.cpp 

                        ../src/chunk/chunk.cpp 
                        ../src/chunk/chunk_loader.cpp 
                        ../src/chunk/blocks.cpp 
                        ../src/world/world.cpp 
                        ../src/world/world_gen.cpp 
                        ../src/automata/automata.cpp 
                        ../src/automata/automata_ruleset.cpp 
              )
#==================================================
# Linking
#==================================================
target_link_libraries(my_game ${OPENGL_LIBRARIES} 
                              ${GLUT_LIBRARY} 
                              ${Boost_LIBRARIES} 
                              ${SOIL_LIBRARY} 
                              ${CMAKE_THREAD_LIBS_INIT} )
