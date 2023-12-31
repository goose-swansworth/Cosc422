#ifndef MODELS_H
#define MODELS_H

#include <GL/freeglut.h>
#include <glm/glm.hpp>

void ball_joint_1(float r);
void ball_joint_2(float r);
void knee(bool left);
void leftHip();
void rightHip();
void shoulder();
void elbow();
void chest();
void middle();
void head();
void hand();
void foot(float r, bool left);
void enviroment();
void platform(float r, float h, glm::vec3 pos);
void floorPlane(int width, int tileWidth);
void loadTextures();

#endif
