
#include "models.h"
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <cmath>

#define PI M_PI

// https://wrfranklin.org/wiki/ComputerGraphicsFall2013/guha/ExperimenterSource/Chapter3/Hemisphere/hemisphere.cpp
void hemisphere(float r, int p, int q) {
    int i, j;
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glColor3f(0.0, 1.0, 0.0);

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
    GLUquadric* q = gluNewQuadric();
    hemisphere(r, 50, 200);
    gluSphere(q, 0.9 * r, 50, 50);
    glRotatef(90, 1, 0, 0);
    gluDisk(q, 0, 3, 50, 50);

}