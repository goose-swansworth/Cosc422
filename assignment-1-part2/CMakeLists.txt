cmake_minimum_required(VERSION 3.14)

project(assignment-1-part2)

add_executable(main.out src/main.cpp src/model_loader.cpp src/assimp_extras.h)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(DevIL REQUIRED)
find_package(assimp REQUIRED)

include_directories( ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${IL_INCLUDE_DIRS} ${ASSIMP_INCLUDE_DIRS})

target_link_libraries(main.out ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${IL_LIBRARIES} ${ASSIMP_LIBRARIES})
