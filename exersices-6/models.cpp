
#include "models.h"
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <cmath>

#define PI M_PI

static GLUquadric* q = gluNewQuadric();
static const float kneeRadius = 1.25;
static const float hipRadius = 1.3;

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


void glmVertex(glm::vec3 v) {
    glVertex3f(v.x, v.y, v.z);
}

void glmNormal(glm::vec3 n) {
    glNormal3f(n.x, n.y, n.z);
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
                glmVertex(sphere_pt_next);

                glmNormal(sphere_pt);
                glmVertex(sphere_pt);  
            }
        glEnd();
    }
}

void ball_joint_1(float r) {
    glm::vec3 green = rgbHexToVec(0x6b8e23);//rgbHexToVec(0x228b22);
    //GLUquadric* q = gluNewQuadric();
    glColor3f(green.x, green.y, green.z);
    hemisphere(r, 50, 150);
    gluSphere(q, 0.9 * r, 25, 25);
    glRotatef(90, 1, 0, 0);
    gluDisk(q, 0, r, 25, 25);

}

void ball_joint_2(float r) {
    glm::vec3 gray = rgbHexToVec(0x696969);
    glColor3f(gray.x, gray.y, gray.z);
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


void knee(bool left) {
    glm::vec3 green = rgbHexToVec(0x6b8e23);//rgbHexToVec(0x228b22);
    glPushMatrix();
        glRotatef(left ? -20 : 20, 0, 0, -1);
        glPushMatrix();
            glColor3f(green.x, green.y, green.z);
            glRotatef(90, 1, 0, 0);
            gluCylinder(q, hipRadius/2.25, hipRadius/2.25, 6.5, 25, 25);
        glPopMatrix();
        ball_joint_2(kneeRadius);
	glPopMatrix();
}

void leftHip() {
    glm::vec3 gray = rgbHexToVec(0x696969);
    glPushMatrix();
        glRotatef(-20, 0, 0, -1);
        glPushMatrix();
            glColor3f(gray.x, gray.y, gray.z);
            glRotatef(90, 1, 0, 0);
            gluCylinder(q, hipRadius/2, hipRadius/2, 7, 25, 25);
        glPopMatrix();
        ball_joint_1(hipRadius);
	glPopMatrix();
}

void rightHip() {
    glm::vec3 gray = rgbHexToVec(0x696969);
    glPushMatrix();
        glRotatef(20, 0, 0, -1);
        glPushMatrix();
            glColor3f(gray.x, gray.y, gray.z);
            glRotatef(90, 1, 0, 0);
            gluCylinder(q, hipRadius/2, hipRadius/2, 7, 25, 25);
        glPopMatrix();
        ball_joint_1(hipRadius);
	glPopMatrix();
}

void shoulder() {
    glm::vec3 gray = rgbHexToVec(0x696969);
    glColor3f(gray.x, gray.y, gray.z);
    glPushMatrix();
        float r = 1.5;
        glPushMatrix();
            glRotatef(90, 1, 0, 0);
            gluCylinder(q, 0.5, 0.5, 4.5, 25, 25);
        glPopMatrix();
        ball_joint_1(r);
	glPopMatrix();
}

void elbow() {
    glm::vec3 green = rgbHexToVec(0x6b8e23);//rgbHexToVec(0x228b22);
    glColor3f(green.x, green.y, green.z);
    glPushMatrix();
        glPushMatrix();
            glRotatef(90, 1, 0, 0);
            gluCylinder(q, 0.5, 0.5, 3.75, 25, 25);
        glPopMatrix();
		ball_joint_2(1);
	glPopMatrix();
}

void foot(float r, bool left) {
    glm::vec3 green = rgbHexToVec(0x6b8e23);//rgbHexToVec(0x228b22);
    glColor3f(green.x, green.y, green.z);
    float c_r = 0.4 * r;
    float angle;
    glPushMatrix();
        angle = left ? -20 : 20;
        glRotatef(angle, 0, 0, -1);

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
        gluCylinder(q, c_r, c_r, 2*r, 25, 25);
       
    glPopMatrix();
}