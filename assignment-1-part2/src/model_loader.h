#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#include <iostream>
#include <GL/freeglut.h>
#include <IL/il.h>
using namespace std;

#include <assimp/cimport.h>
#include <assimp/types.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


struct Model {
	const aiScene* scene = NULL;
	aiVector3D scene_center;
	float scene_scale;
	GLuint* textureIds = NULL;
};

bool loadModel(const char* fileName, Model* model);
void loadGLTextures(Model* model);
void renderModel(const aiNode* node, Model* model, bool overideColor);

#endif