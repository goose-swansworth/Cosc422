#ifndef CHARATER_H
#define CHARATER_H

#include "model_loader.h"
#include "models.h"

struct Charater {
	const aiScene* scene = NULL;
	int tick = 0;
    int tickStep = 1;
};

void renderCharater(const aiNode* node, Charater charater, Model models[]);
bool replace_links(aiString name);

#endif