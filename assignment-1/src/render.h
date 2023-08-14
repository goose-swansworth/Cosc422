#ifndef RENDER_H
#define RENDER_H


#include <iostream>
#include <fstream>
#include <GL/freeglut.h>
#include <glm/glm.hpp>


#include <assimp/cimport.h>
#include <assimp/types.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "assimp_extras.h"

void render(const aiNode* node, bool links);

#endif