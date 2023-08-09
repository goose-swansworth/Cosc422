
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


// https://wrfranklin.org/wiki/ComputerGraphicsFall2013/guha/ExperimenterSource/Chapter3/Hemisphere/hemisphere.cpp
void hemisphere(float r, int p, int q) {
    int i, j;
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Array of latitudinal triangle strips, each parallel to the equator, stacked one
    // above the other from the equator to the north pole.
    for(j = 0; j < q; j++)
    {
        // One latitudinal triangle strip.
        glBegin(GL_TRIANGLE_STRIP);
            for(i = 0; i <= p; i++)
            {
            glNormal3f( r * cos( (float)(j+1)/q * PI/2.0 ) * cos( 2.0 * (float)i/p * PI ),
                        r * sin( (float)(j+1)/q * PI/2.0 ),
                        r * cos( (float)(j+1)/q * PI/2.0 ) * sin( 2.0 * (float)i/p * PI ) );
            glVertex3f( r * cos( (float)(j+1)/q * PI/2.0 ) * cos( 2.0 * (float)i/p * PI ),
                        r * sin( (float)(j+1)/q * PI/2.0 ),
                        r * cos( (float)(j+1)/q * PI/2.0 ) * sin( 2.0 * (float)i/p * PI ) );


            glVertex3f( r * cos( (float)j/q * PI/2.0 ) * cos( 2.0 * (float)i/p * PI ),
                        r * sin( (float)j/q * PI/2.0 ),
                        r * cos( (float)j/q * PI/2.0 ) * sin( 2.0 * (float)i/p * PI ) );
            glNormal3f( r * cos( (float)j/q * PI/2.0 ) * cos( 2.0 * (float)i/p * PI ),
                        r * sin( (float)j/q * PI/2.0 ),
                        r * cos( (float)j/q * PI/2.0 ) * sin( 2.0 * (float)i/p * PI ) );                  
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
    //GLUquadric* q = gluNewQuadric();
    glColor3f(gray.x, gray.y, gray.z);
    hemisphere(r, 50, 150);
    gluSphere(q, 0.9 * r, 25, 25);
    glRotatef(90, 1, 0, 0);
    gluDisk(q, 0, r, 25, 25);

}

void leftKnee() {
    //GLUquadric* q = gluNewQuadric();
    glPushMatrix();
        glRotatef(-20, 0, 0, -1);
        ball_joint_2(kneeRadius);
	glPopMatrix();
}

void rightKnee() {
    glPushMatrix();
        glRotatef(20, 0, 0, -1);
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