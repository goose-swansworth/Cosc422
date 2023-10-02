//  ========================================================================
//  COSC422: Computer Graphics (2023);  University of Canterbury.
//  FILE NAME: TerrainP.cpp
//  See Exer14.pdf for details.
//
//  The program generates and loads the mesh data for a terrain floor (100 verts, 81 elems).
//  Requires files  Terrain.vert, Terrain.frag
//                  Terrain.cont, Terrains.eval
//  ========================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>
#include "loadTGA.h"
#include "camera.h"
#include "shader.h"
#include "stb_image.h"
using namespace std;

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900



Camera viewer(glm::vec3(-5, 7, -32));
static glm::vec2 mouse_last;

GLuint vaoID;
GLuint spritePointsVAO;
GLuint skyboxVAO;
GLuint theProgram;
GLuint mvpMatrixLoc, eyeLoc, zNear, zFar, waveTick, waterHeight;
const unsigned int numTextures = 6;
GLuint textureIds[numTextures];
float  eye_x = 0, eye_y = 20, eye_z = 30;      //Initial camera position
float look_x = 0, look_y = 0, look_z = -40;    //"Look-at" point along -z direction
float  theta = 0;                              //Look angle
float toRad = 3.14159265/180.0;     //Conversion from degrees to rad
float waterLevelf = 2.2;
int tick = 0;

float verts[100*3];       //10x10 grid (100 vertices)
GLushort elems[81*4];     //Element array for 9x9 = 81 quad patches

glm::mat4 projView;
glm::vec3 lightPosition(0, 40, -45);

Shader* terrianShader;
Shader* spriteShader;
Shader* skyboxShader;

const unsigned int nSprites = 150000;
static const float xMin = -45;
static const float xMax = 45;
static const float zMin = -45;
static const float zMax = 45;
float spritePoints[nSprites * 3];

unsigned int cubemapTexture;


static const std::vector<std::string> texFilenames = {
        "../textures/baysfilp.tga",
        "../textures/grass.tga",
        "../textures/water.tga",
        "../textures/sand.tga",
        "../textures/browngrass.tga",
        "../textures/pine.tga",
        "../textures/birtch.tga",
        "../textures/pine1.tga",
        "../textures/birtch1.tga",
        "../textures/bush.tga"
    };


static const std::vector<std::string> texNames = {
        "heightMap",
        "grassTex",
        "waterTex",
        "sandTex",
        "brownGrassTex",
        "tree1",
        "tree2",
        "tree3",
        "tree4",
        "tree5"
    };

static vector<std::string> faces {
    "../textures/skybox/right.jpg",
    "../textures/skybox/left.jpg",
    "../textures/skybox/top.jpg",
    "../textures/skybox/bottom.jpg",
    "../textures/skybox/front.jpg",
    "../textures/skybox/back.jpg"
};

float skyboxVertices[] = {
    // positions
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

static std::vector<unsigned int> texWidths;
static std::vector<unsigned int> texHeights;

//Generate vertex and element data for the terrain floor
void generateData()
{
    int indx, start;
    //verts array
    for(int i = 0; i < 10; i++)   //100 vertices on a 10x10 grid
    {
        for(int j = 0; j < 10; j++)
        {
            indx = 10*i + j;
            verts[3*indx] = 10*i-45;      //x
            verts[3*indx+1] = 0;            //y
            verts[3*indx+2] = -10*j+45;        //z
        }
    }

    //elems array
    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            indx = 9*i +j;
            start = 10*i + j;
            elems[4*indx] = start;
            elems[4*indx+1] = start + 10;
            elems[4*indx+2] = start + 11;
            elems[4*indx+3] = start + 1;
        }
    }
}

void loadTexture(const char* filename, int index, int glTexNum, unsigned int* width, unsigned int* height) {
    glActiveTexture(glTexNum);
    glBindTexture(GL_TEXTURE_2D, textureIds[index]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    loadTGA(filename, width, height);
    cout << filename << ": (" << *width << ", " << *height << ")\n"; 
}

void loadCubemap(vector<std::string> faces) {
    glGenTextures(1, &cubemapTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

}

//Loads height map
void loadTextures()
{
    glGenTextures(texNames.size(), textureIds);
    unsigned int width, height;
    for (int i = 0; i < texNames.size(); i++) {
        loadTexture(texFilenames[i].c_str(), i, GL_TEXTURE0 + i, &width, &height);
        texWidths.push_back(width);
        texHeights.push_back(height);
    }

}

void populateSpriteArray(float points[], unsigned int nSprites) {
    float x, z;
    for (int i = 0; i < 3 * nSprites; i+=3) {
        x = glm::linearRand(xMin, xMax);
        z = glm::linearRand(zMin, zMax);
        points[i] = x;
        points[i + 1] = 0.0;
        points[i + 2] = z;
    }
}

void resizeSkybox() {
    for (int i = 0; i < 108; i++) {
        skyboxVertices[i] = 180 * skyboxVertices[i];
    }
}

//Initialise the shader program, create and load buffer data
void initialise() {
//--------Load terrain height map-----------
    loadTextures();
    populateSpriteArray(spritePoints, nSprites);
    loadCubemap(faces);
    resizeSkybox();

    spriteShader = new Shader();
    spriteShader->attachShader(GL_VERTEX_SHADER, "../src/shaders/sprite.vert");
    spriteShader->attachShader(GL_FRAGMENT_SHADER, "../src/shaders/sprite.frag");
    spriteShader->link();


    terrianShader = new Shader();
    terrianShader->attachShader(GL_VERTEX_SHADER, "../src/shaders/Terrain.vert");
    terrianShader->attachShader(GL_FRAGMENT_SHADER, "../src/shaders/Terrain.frag");
    terrianShader->attachShader(GL_TESS_CONTROL_SHADER, "../src/shaders/Terrain.cont");
    terrianShader->attachShader(GL_TESS_EVALUATION_SHADER, "../src/shaders/Terrain.tese");
    terrianShader->link();

    skyboxShader = new Shader();
    skyboxShader->attachShader(GL_VERTEX_SHADER, "../src/shaders/skybox.vert");
    skyboxShader->attachShader(GL_FRAGMENT_SHADER, "../src/shaders/skybox.frag");
    skyboxShader->link();

//---------Load buffer data-----------------------
    generateData();
    GLuint vboID[2];
    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);

    glGenBuffers(2, vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);  // Vertex position
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elems), elems, GL_STATIC_DRAW);
    glBindVertexArray(0);
    glPatchParameteri(GL_PATCH_VERTICES, 4);

    GLuint spritePointsVBO;
    glGenVertexArrays(1, &spritePointsVAO);
    glBindVertexArray(spritePointsVAO);
    glGenBuffers(1, &spritePointsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, spritePointsVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(spritePoints), spritePoints, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    GLuint skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_POINT_SPRITE);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glPointParameteri(GL_POINT_SPRITE_COORD_ORIGIN, GL_LOWER_LEFT);
}

void updateViewProjMatrix(Camera viewer) {
    glm::vec4 cameraPosn = glm::vec4(viewer.position, 1.0);
    glm::vec3 look = viewer.position + viewer.fowards;
    float aspect = (float)WINDOW_WIDTH / WINDOW_HEIGHT;
    glm::mat4 proj = glm::perspective(80.0f * toRad, aspect, 1.0f, 500.0f);  //perspective projection matrix
    glm::mat4 view = lookAt(glm::vec3(cameraPosn), look, glm::vec3(0.0, 1.0, 0.0)); //view matri
    projView = proj * view;  //Product matrix
}

void drawTerrian() {
    terrianShader->use();

    for (int i = 0; i < texNames.size(); i++) {
        terrianShader->setInt(texNames[i], i);
    }

    terrianShader->setVec3("lightPos", lightPosition);
    terrianShader->setInt("zNear", 20);
    terrianShader->setInt("zFar", 500);
    terrianShader->setFloat("waterHeight", waterLevelf);
    terrianShader->setInt("waveTick", tick);
    glm::vec4 cameraPosn = glm::vec4(viewer.position, 1.0);
    terrianShader->setVec3("eyePos", cameraPosn);
    terrianShader->setMat4("mvpMatrix", projView);

    glBindVertexArray(vaoID);
    glDrawElements(GL_PATCHES, 81 * 4, GL_UNSIGNED_SHORT, NULL);
}

void drawSprites() {
    spriteShader->use();
    spriteShader->setMat4("mvpMatrix", projView);
    spriteShader->setFloat("treeHeight", 7.0);
    spriteShader->setFloat("offset", lightPosition.x);
    spriteShader->setVec3("lightPos", lightPosition);

    for (int i = 0; i < texNames.size(); i++) {
        spriteShader->setInt(texNames[i], i);
        spriteShader->setFloat(texNames[i] + "Aspect", (float)texWidths[i]/texHeights[i]);
    }

    glBindVertexArray(spritePointsVAO);
    glDrawArrays(GL_POINTS, 0, 3 * nSprites);

}

void drawSkybox() {
    skyboxShader->use();
    skyboxShader->setMat4("mvpMatrix", projView);
    skyboxShader->setInt("skybox", 0);
    glDepthFunc(GL_LEQUAL);
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(skyboxVertices) / 3);
    glDepthFunc(GL_LESS);

}

void display() {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    updateViewProjMatrix(viewer);
    
    drawTerrian();
    drawSprites();
    drawSkybox();

    glutSwapBuffers();
}

void special(int key, int x, int y)
{
    int step = 0;
    float dir_x, dir_z;
    if (key == GLUT_KEY_LEFT) lightPosition.x += 1;   //in radians
    else if (key == GLUT_KEY_RIGHT) lightPosition.x -= 1;
    else if (key == GLUT_KEY_DOWN) lightPosition.x -= 0.01;
    else if (key == GLUT_KEY_UP) lightPosition.x += 0.01;
    cout << lightPosition.x << ", " << viewer.position.y << ", " << viewer.position.z << "\n";
    terrianShader->setVec3("lightPos", lightPosition);
    glutPostRedisplay();
}

void mouse_handler(int x, int y) {
    float screen_x = (2 * (float)x - WINDOW_WIDTH) / WINDOW_WIDTH;
    float screen_y = (2 * (float)y - WINDOW_HEIGHT) / WINDOW_HEIGHT;
    glm::vec2 mouse(screen_x, screen_y);
    viewer.update(mouse, mouse_last);
    mouse_last = mouse;
    glutPostRedisplay();
}

void keyboard_handler(unsigned char key, int x, int y) {
    glm::vec3 dir(0.0);
    switch (key) {
        case 'w':
            dir.x = 1;
            break;
        case 's':
            dir.x = -1;
            break;
        case 'a':
            dir.z = -1;
            break;
        case 'd':
            dir.z = 1;
            break;
        case ' ':
            dir.y = 1;
            break;
        case 'q':
            exit(0);
    }
    viewer.move(dir);
    glutPostRedisplay();
}

void updateLightPos(int tick) {
    float minor = 40.0;
    float major = 100.0;
    int cycle = 1000;
    float t = (float)tick/cycle * M_2PI;
    if (t > M_PI) {
        t = M_2PI - M_PI;
    }
    lightPosition.x = major * cos(t);
    lightPosition.y = minor * sin(t);
    cout << lightPosition.x << ", " << lightPosition.y << ", " << lightPosition.z << "\n";
}

void animate(int val) {
    tick++;
    //updateLightPos(tick);
    glutTimerFunc(50, animate, val);
    glutPostRedisplay();

}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutSetOption(GLUT_MULTISAMPLE, 8);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Terrain");
    glutInitContextVersion (4, 2);
    glutInitContextProfile ( GLUT_CORE_PROFILE );
    glutPassiveMotionFunc(mouse_handler);
    glutKeyboardFunc(keyboard_handler);
    glutSetCursor(GLUT_CURSOR_NONE);

    if(glewInit() == GLEW_OK)
    {
        cout << "GLEW initialization successful! " << endl;
        cout << " Using GLEW version " << glewGetString(GLEW_VERSION) << endl;
    }
    else
    {
        cerr << "Unable to initialize GLEW  ...exiting." << endl;
        exit(EXIT_FAILURE);
    }

    initialise();
    glutDisplayFunc(display);
    glutSpecialFunc(special);
    glutTimerFunc(50, animate, 0);
    glutMainLoop();
    return 0;
}

