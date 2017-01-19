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
# Include Files
#==================================================
add_executable(my_game 
                        ../src/main.cpp 
                        ../src/display.cpp 
                        ../src/main_loop.cpp 
                        ../src/utils.cpp 
                        ../src/state.cpp
                        ../src/events.cpp
              )
#==================================================
# Linking
#==================================================
target_link_libraries(my_game ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )
