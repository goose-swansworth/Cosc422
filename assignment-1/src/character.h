#ifndef CHARATER_H
#define CHARATER_H

#include "model_loader.h"
#include "models.h"

#define BAT 0
#define HELMET 1
#define HAT 2
#define BALL 3

struct Character {
	const aiScene* scene = NULL;
	int tick = 0;
    int tickStep = 1;
	glm::vec3 position;
	float angle;
	glm::vec3 axis;
    int code;
};

void renderCharacter(const aiNode* node, Character character, Model models[]);
bool replace_links(aiString name);
Character init_character(const aiScene* scene, glm::vec3 position, float angle, glm::vec3 axis, int code);

#endif
