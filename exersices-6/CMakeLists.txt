cmake_minimum_required(VERSION 3.14)

project(ex6)

add_executable(ex6.out SkeletalAnimation.cpp models.cpp loadTGA.cpp stb.cpp assimp_extras.h)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(DevIL REQUIRED)
find_package(assimp REQUIRED)

include_directories( ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${IL_INCLUDE_DIRS} ${ASSIMP_INCLUDE_DIRS})

target_link_libraries(ex6.out ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${IL_LIBRARIES} ${ASSIMP_LIBRARIES})
