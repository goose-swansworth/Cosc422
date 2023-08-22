
#include "models.h"
#include "loadTGA.h"
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <cmath>
#include <iostream>

#define PI M_PI
#define GRAY_METAL 0
#define GREEN_METAL0 1
#define GREEN_METAL1 2
#define GREEN_METAL2 3
#define RUSTY_METAL 4
#define ENV 5
#define FLOOR 6
#define WOOD 7

static GLUquadric* q = gluNewQuadric();
static const float kneeRadius = 1.25;
static const float hipRadius = 1.3;
GLuint textureIds[8];


glm::vec3 rgbHexToVec(int hexcode) {
    glm::vec3 color(0);
    int r_mask = 0xff0000;
    int g_mask = 0x00ff00;
    int b_mask = 0x0000ff;
    color.x = (float)((hexcode & r_mask) >> 16) / 255;
    color.y = (float)((hexcode & g_mask) >> 8) / 255;
    color.z = (float)(hexcode & b_mask) / 255;
    return color;
}

void enviroment() {
    glColor3f(1.0, 1.0, 1.0);
    glDisable(GL_LIGHTING);
    glBindTexture(GL_TEXTURE_2D, textureIds[ENV]);
    glPushMatrix();
        //glTranslatef(0, 80, 0);
        //glRotatef(180, 0, 1, 0);
        glRotatef(-90, 1, 0, 0);
        gluSphere(q, 150, 15, 15);
    glPopMatrix();
    glEnable(GL_LIGHTING);
}

void loadTextures() {
    glGenTextures(7, textureIds);

    glBindTexture(GL_TEXTURE_2D, textureIds[GRAY_METAL]);
    loadTGA("../textures/gray_metal.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, textureIds[GREEN_METAL0]);
    loadTGA("../textures/green_metal0.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, textureIds[GREEN_METAL1]);
    loadTGA("../textures/green_metal1.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, textureIds[GREEN_METAL2]);
    loadTGA("../textures/green_metal2.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, textureIds[RUSTY_METAL]);
    loadTGA("../textures/rusty_metal.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, textureIds[ENV]);
    loadTGA("../textures/env.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, textureIds[FLOOR]);
    loadTGA("../textures/ground.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, textureIds[WOOD]);
    loadTGA("../textures/wood.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}


void glmVertex(glm::vec3 v) {
    glVertex3f(v.x, v.y, v.z);
}

void glmNormal(glm::vec3 n) {
    glNormal3f(n.x, n.y, n.z);
}

void floorPlane(int width, int tileWidth) {
    glm::vec3 gray = rgbHexToVec(0x696969);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBindTexture(GL_TEXTURE_2D, textureIds[FLOOR]);
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_QUADS);
    for (int i = -width / tileWidth / 2; i < width/ tileWidth / 2; i++) {
        for (int j = -width / tileWidth / 2; j < width / tileWidth/  2; j++) {
            glTexCoord2f((float)(width/2 + i) / width, (float)(width/2 + j) / width);
            glVertex3f(i*tileWidth, 0, j*tileWidth);

            glTexCoord2f((float)(width/2 + i) / width, (float)(width/2 + j + 1) / width);
            glVertex3f(i*tileWidth, 0, (j + 1) * tileWidth);

            glTexCoord2f((float)(width/2 + i + 1) / width, (float)(width/2 + j + 1) / width);
            glVertex3f((i + 1) * tileWidth, 0, (j + 1) * tileWidth);

            glTexCoord2f((float)(width/2 + i + 1) / width, (float)(width/2 + j) / width);
            glVertex3f((i + 1) * tileWidth, 0, j * tileWidth);
        }
    }
    glEnd();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_TEXTURE_2D);
}

void platform(float r, float h, glm::vec3 pos) {
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, textureIds[WOOD]);
    glPushMatrix();
    glTranslatef(pos.x, pos.y, pos.z);
    glTranslatef(0, h, 0);
    glRotatef(90, 1, 0, 0);
    gluCylinder(q, r, r, h, 40, 40);
        glPushMatrix();
            glRotatef(180, 1, 0, 0);
            gluDisk(q, 0, r, 40, 40);
        glPopMatrix();
    glPopMatrix();
}


// https://wrfranklin.org/wiki/ComputerGraphicsFall2013/guha/ExperimenterSource/Chapter3/Hemisphere/hemisphere.cpp

void hemisphere(float r, int p, int q) {
    int i, j;
    float phi, phi_next, theta;

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    for(j = 0; j < q; j++) {

        glBegin(GL_TRIANGLE_STRIP);
            for(i = 0; i <= p; i++) {

                phi = (float)j/q * PI/2.0;
                phi_next = (float)(j+1)/q * PI/2.0;
                theta = 2.0 * (float)i/p * PI;

                glm::vec3 sphere_pt(r*cos(phi)*cos(theta), r*sin(phi), r*cos(phi)*sin(theta));
                glm::vec3 sphere_pt_next(r*cos(phi_next)*cos(theta), r*sin(phi_next), r*cos(phi_next)*sin(theta));

                glmNormal(sphere_pt_next);
                glTexCoord2f(theta / (2 * PI), phi_next / PI);
                glmVertex(sphere_pt_next);

                glmNormal(sphere_pt);
                glTexCoord2f(theta / (2 * PI), phi / PI);
                glmVertex(sphere_pt);  
            }
        glEnd();
    }
}

void ball_joint_1(float r) {
    hemisphere(r, 50, 150);
    gluSphere(q, 0.9 * r, 25, 25);
    glRotatef(90, 1, 0, 0);
    gluDisk(q, 0, r, 25, 25);

}

void ball_joint_2(float r) {
    gluSphere(q, r, 25, 25);
    glRotatef(90, 0, 1, 0);
    glPushMatrix();
        glTranslatef(0, 0, r);
        gluDisk(q, 0, r/2, 25, 25);
    glPopMatrix();
    glTranslatef(0, 0, -r);
    gluCylinder(q, r/2, r/2, 2*r, 25, 25);
    gluDisk(q, 0, r/2, 25, 25);
}

void chest() {
    float r = 2.75;
    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
        glTranslatef(0, r/2, 0);
        glBindTexture(GL_TEXTURE_2D, textureIds[GREEN_METAL0]);
        ball_joint_1(r);
    glPopMatrix();
}

void middle() {
    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
        glTranslatef(0, -0.05, 0);
        glBindTexture(GL_TEXTURE_2D, textureIds[GRAY_METAL]);
        ball_joint_2(1.1);
    glPopMatrix();
}

void head() {
    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, textureIds[GREEN_METAL2]);
        ball_joint_1(1.75);
    glPopMatrix();
}


void knee(bool left) {
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
        glRotatef(left ? -20 : 20, 0, 0, -1);
        glPushMatrix();
            glRotatef(90, 1, 0, 0);
            glBindTexture(GL_TEXTURE_2D, textureIds[GRAY_METAL]);
            gluCylinder(q, hipRadius/2.25, hipRadius/2.25, 6.5, 25, 25);
        glPopMatrix();
        glBindTexture(GL_TEXTURE_2D, textureIds[GREEN_METAL1]);
        ball_joint_2(kneeRadius);
	glPopMatrix();
}

void leftHip() {
    glm::vec3 gray = rgbHexToVec(0x696969);

    glBindTexture(GL_TEXTURE_2D, 0);
    gluQuadricTexture(q, GL_TRUE);
    glEnable(GL_TEXTURE_2D);

    
    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
        glRotatef(-20, 0, 0, -1);
        glPushMatrix();
            glRotatef(90, 1, 0, 0);
            glBindTexture(GL_TEXTURE_2D, textureIds[GRAY_METAL]);
            gluCylinder(q, hipRadius/2, hipRadius/2, 7, 25, 25);
        glPopMatrix();
        glBindTexture(GL_TEXTURE_2D, textureIds[GREEN_METAL0]);
        ball_joint_1(hipRadius);
	glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void rightHip() {

    glBindTexture(GL_TEXTURE_2D, 0);
    gluQuadricTexture(q, GL_TRUE);
    glEnable(GL_TEXTURE_2D);

    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
        glRotatef(20, 0, 0, -1);
        glPushMatrix();
            glRotatef(90, 1, 0, 0);
            glBindTexture(GL_TEXTURE_2D, textureIds[GRAY_METAL]);
            gluCylinder(q, hipRadius/2, hipRadius/2, 7, 25, 25);
        glPopMatrix();
        glBindTexture(GL_TEXTURE_2D, textureIds[GREEN_METAL1]);
        ball_joint_1(hipRadius);
	glPopMatrix();
}

void shoulder() {
    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
        float r = 1.5;
        glPushMatrix();
            glRotatef(90, 1, 0, 0);
            glBindTexture(GL_TEXTURE_2D, textureIds[GRAY_METAL]);
            gluCylinder(q, 0.5, 0.5, 4.5, 25, 25);
        glPopMatrix();
        glBindTexture(GL_TEXTURE_2D, textureIds[GREEN_METAL1]);
        ball_joint_1(r);
	glPopMatrix();
}

void elbow() {
    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
        glPushMatrix();
            glRotatef(90, 1, 0, 0);
            glBindTexture(GL_TEXTURE_2D, textureIds[GREEN_METAL2]);
            gluCylinder(q, 0.5, 0.5, 4, 25, 25);
        glPopMatrix();
		ball_joint_2(1);
	glPopMatrix();
}

void foot(float r, bool left) {
    float c_r = 0.4 * r;
    float angle;
    glPushMatrix();
        angle = left ? -20 : 20;
        glRotatef(angle, 0, 0, -1);
        glBindTexture(GL_TEXTURE_2D, textureIds[GREEN_METAL0]);
        hemisphere(r, 50, 150);
        glRotatef(90, 1, 0, 0);
        gluDisk(q, 0, r, 25, 25);

        glTranslatef(0, 0, -r * 0.75);
        glRotatef(90, 0, 1, 0);
        glPushMatrix();
            glTranslatef(0, 0, r);
            gluDisk(q, 0, c_r, 25, 25);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0, 0, -r);
            gluDisk(q, 0, c_r, 25, 25);
        glPopMatrix();
        glTranslatef(0, 0, -r);
        glBindTexture(GL_TEXTURE_2D, textureIds[GREEN_METAL1]);
        gluCylinder(q, c_r, c_r, 2*r, 25, 25);
       
    glPopMatrix();
}

void hand() {
    glBindTexture(GL_TEXTURE_2D, textureIds[GREEN_METAL0]);
    glPushMatrix();
		ball_joint_1(0.8);
	glPopMatrix();
}

