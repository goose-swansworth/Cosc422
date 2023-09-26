cmake_minimum_required(VERSION 3.16)

project(assignment-2)

add_executable(main.out src/main.cpp src/shader.h src/loadTGA.h src/stb.cpp)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)

include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} )

target_link_libraries( main.out ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARIES} )
