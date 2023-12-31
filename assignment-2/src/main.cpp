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
#include <glm/gtx/transform.hpp>
#include <glm/gtc/random.hpp>
#include "loadTGA.h"
#include "camera.h"
#include "shader.h"
#include "stb_image.h"
#include "model.h"
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
const unsigned int numTextures = 11;
GLuint textureIds[numTextures];
float  eye_x = 0, eye_y = 20, eye_z = 30;      //Initial camera position
float look_x = 0, look_y = 0, look_z = -40;    //"Look-at" point along -z direction
float  theta = 0;                              //Look angle
float toRad = 3.14159265/180.0;     //Conversion from degrees to rad
float waterLevelf = 2.2;
int tick = 0;

float verts[100*3];       //10x10 grid (100 vertices)
GLushort elems[81*4];     //Element array for 9x9 = 81 quad patches

glm::mat4 projMatrix;
glm::mat4 viewMatrix;
glm::mat4 modelMatrix;
glm::mat4 projView;
glm::vec3 lightPosition(0, 40, -45);
glm::vec2 circleCenter(-2.25977, 7.41198);

Shader* terrianShader;
Shader* spriteShader;
Shader* skyboxShader;
Shader* modelShader;
Shader* imposterShader;
unsigned int frameBuffer;
unsigned int renderBuffer;
unsigned int imposterTextures[8];

const unsigned int nSprites = 150000;
const unsigned int nRocks = 50000;
static const float xMin = -45;
static const float xMax = 45;
static const float zMin = -45;
static const float zMax = 45;
float spritePoints[nSprites * 3];
float rockPoints[nRocks * 3];
unsigned int imposterSpritesVAO;
static const int iSpriteTextures = 7;

unsigned int cubemapTexture;
static bool lineFlag = false;

Model* model;
Model* tower;
static glm::vec3 towerPosition = glm::vec3(27, 9.25,19);

using namespace std;
vector<Model> rockModels;

static const vector<string> terrianTexFilenames = {
        "../textures/baysfliped3.tga",
        "../textures/roadmask.tga",
        "../textures/grass.tga",
        "../textures/water.tga",
        "../textures/sand.tga",
        "../textures/browngrass.tga",
        "../textures/road.tga"
        
};

static const vector<string> spriteTexFilenames = {
        "../textures/pine.tga",
        "../textures/birtch.tga",
        "../textures/pine1.tga",
        "../textures/birtch1.tga",
};

static const vector<string> terrianTexNames = {
        "heightMap",
        "mask",
        "grassTex",
        "waterTex",
        "sandTex",
        "brownGrassTex",
        "roadTex"
};

static const vector<string> spriteTexNames = {
    "tree1",
    "tree2",
    "tree3",
    "tree4"
};

static vector<string> faces {
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

static vector<float> texAspects;

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
    if (index == 0) {
        // Clamp the height map texture to edge
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    } else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    loadTGA(filename, width, height);
    cout << filename << ": (" << *width << ", " << *height << ")\n"; 
}

void loadCubemap(vector<string> faces) {
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
            cout << "Cubemap tex failed to load at path: " << faces[i] << endl;
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
void loadTextures() {
    glGenTextures(terrianTexNames.size() + spriteTexNames.size(), textureIds);
    unsigned int width, height;
    int k = 0;
    // Load the terrian textures
    for (int i = 0; i < terrianTexNames.size(); i++) {
        loadTexture(terrianTexFilenames[i].c_str(), k, GL_TEXTURE0 + k, &width, &height);
        k++;
    }
    // Load the sprite textures
    for (int i = 0; i < spriteTexNames.size(); i++) {
        loadTexture(spriteTexFilenames[i].c_str(), k, GL_TEXTURE0 + k, &width, &height);
        texAspects.push_back((float)width / height);
        k++;
    }
    texAspects.push_back((float)WINDOW_WIDTH / WINDOW_HEIGHT);
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

void bufferTerrainPatches() {
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
}

void bufferSpriteData() {
    GLuint spritePointsVBO;
    glGenVertexArrays(1, &spritePointsVAO);
    glBindVertexArray(spritePointsVAO);
    glGenBuffers(1, &spritePointsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, spritePointsVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(spritePoints), spritePoints, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);
}

void bufferImposterData() {
    GLuint imposterSpritesVBO;
    glGenVertexArrays(1, &imposterSpritesVAO);
    glBindVertexArray(imposterSpritesVAO);
    glGenBuffers(1, &imposterSpritesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, imposterSpritesVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rockPoints), rockPoints, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);
}

void bufferSkyboxData() {
    GLuint skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindVertexArray(0);
}

void prepareFrameBuffer() {
    glGenTextures(8, imposterTextures);
    for (int i = 0; i < 8; i++) {
        glBindTexture(GL_TEXTURE_2D, imposterTextures[i]);
        glTexImage2D(GL_TEXTURE_2D,0, GL_RGB, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, imposterTextures[0], 0);

    glGenRenderbuffers(1, &renderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WINDOW_WIDTH, WINDOW_HEIGHT);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) { cout << "FBO Error!" << endl; }
    
}

//Initialise the shader program, create and load buffer data
void initialise() {
    loadTextures();
    populateSpriteArray(spritePoints, nSprites);
    populateSpriteArray(rockPoints, nRocks);
    loadCubemap(faces);
    resizeSkybox();

    spriteShader = new Shader();
    spriteShader->attachShader(GL_VERTEX_SHADER, "../src/shaders/sprite.vert");
    spriteShader->attachShader(GL_FRAGMENT_SHADER, "../src/shaders/sprite.frag");
    spriteShader->link();


    terrianShader = new Shader();
    terrianShader->attachShader(GL_VERTEX_SHADER, "../src/shaders/Terrain.vert");
    terrianShader->attachShader(GL_FRAGMENT_SHADER, "../src/shaders/Terrain.frag");
    terrianShader->attachShader(GL_TESS_CONTROL_SHADER, "../src/shaders/Terrain.tesc");
    terrianShader->attachShader(GL_TESS_EVALUATION_SHADER, "../src/shaders/Terrain.tese");
    terrianShader->link();

    skyboxShader = new Shader();
    skyboxShader->attachShader(GL_VERTEX_SHADER, "../src/shaders/skybox.vert");
    skyboxShader->attachShader(GL_FRAGMENT_SHADER, "../src/shaders/skybox.frag");
    skyboxShader->link();

    bufferTerrainPatches();
    bufferSpriteData();
    bufferImposterData();
    bufferSkyboxData();
    prepareFrameBuffer();

    modelShader = new Shader();
    modelShader->attachShader(GL_VERTEX_SHADER, "../src/shaders/model.vert");
    modelShader->attachShader(GL_FRAGMENT_SHADER, "../src/shaders/model.frag");
    modelShader->link();

    imposterShader = new Shader();
    imposterShader->attachShader(GL_VERTEX_SHADER, "../src/shaders/imposter.vert");
    imposterShader->attachShader(GL_FRAGMENT_SHADER, "../src/shaders/imposter.frag");
    imposterShader->link();



    model = new Model("../models/KukuisBoat/ev0421_sm_yacht_yacht.dae");
    tower = new Model("../models/tower/RadioTower.obj");
    rockModels.push_back(Model("../models/rock1/Rock1.obj"));
    rockModels.push_back(Model("../models/rock/rock.fbx"));

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_POINT_SPRITE);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glPointParameteri(GL_POINT_SPRITE_COORD_ORIGIN, GL_LOWER_LEFT);

    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void updateViewProjMatrix(glm::vec3 position, glm::vec3 look) {
    glm::vec4 cameraPosn = glm::vec4(position, 1.0);
    //glm::vec3 look = viewer.position + viewer.fowards;
    float aspect = (float)WINDOW_WIDTH / WINDOW_HEIGHT;
    glm::mat4 projMatrix = glm::perspective(80.0f * toRad, aspect, 0.01f, 500.0f);  //perspective projection matrix
    glm::mat4 viewMatrix = lookAt(glm::vec3(cameraPosn), look, glm::vec3(0.0, 1.0, 0.0)); //view matri
    projView = projMatrix * viewMatrix;  //Product matrix
}

void drawTerrian() {
    terrianShader->use();

    for (int i = 0; i < terrianTexNames.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        terrianShader->setInt(terrianTexNames[i], i);
        glBindTexture(GL_TEXTURE_2D, textureIds[i]);
    }

    terrianShader->setVec3("lightPos", lightPosition);
    terrianShader->setInt("zNear", 20);
    terrianShader->setInt("zFar", 500);
    terrianShader->setFloat("waterHeight", waterLevelf);
    terrianShader->setInt("waveTick", tick);
    glm::vec4 cameraPosn = glm::vec4(viewer.position, 1.0);
    terrianShader->setVec4("eyePos", cameraPosn);
    terrianShader->setMat4("mvpMatrix", projView);

    glBindVertexArray(vaoID);
    glDrawElements(GL_PATCHES, 81 * 4, GL_UNSIGNED_SHORT, NULL);
}

void drawSprites() {
    //glEnable(GL_BLEND);
    spriteShader->use();
    spriteShader->setMat4("mvpMatrix", projView);
    spriteShader->setFloat("treeHeight", 7.0);
    spriteShader->setFloat("offset", lightPosition.x);
    spriteShader->setVec3("lightPos", lightPosition);

    // Send the height map to the sprite shader
    glActiveTexture(GL_TEXTURE0);
    spriteShader->setInt(terrianTexNames[0], 0);
    glBindTexture(GL_TEXTURE_2D, textureIds[0]);
    // Send the sprite textures to the sprite shader (+1 offset because of height map)
    for (int i = 0; i < spriteTexNames.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i + 1);
        spriteShader->setInt(spriteTexNames[i], i + 1);
        glBindTexture(GL_TEXTURE_2D, textureIds[terrianTexNames.size() + i]);
    }

    glUniform1fv(glGetUniformLocation(spriteShader->ID, "texAspects"), texAspects.size(), &texAspects[0]);
    glBindVertexArray(spritePointsVAO);
    glDrawArrays(GL_POINTS, 0, 3 * nSprites);
    glDisable(GL_BLEND);

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

void drawModel() {
    glEnable(GL_BLEND);
    glm::mat4 boatModelMatrix = glm::mat4(1.0f);
    boatModelMatrix = glm::translate(boatModelMatrix, glm::vec3(10.0f, 2.25f, 10.0f));
    boatModelMatrix = glm::scale(boatModelMatrix, 0.001f * glm::vec3(1.0f));
    glm::mat4 towerModelMatrix = glm::mat4(1.0f);
    towerModelMatrix = glm::translate(towerModelMatrix, towerPosition);
    towerModelMatrix = glm::scale(towerModelMatrix, 0.1f * glm::vec3(1.0f));
    modelShader->use();
    modelShader->setMat4("projViewMatrix", projView);
    modelShader->setMat4("modelMatrix", boatModelMatrix);
    modelShader->setVec3("lightPos", lightPosition);
    model->Draw(*modelShader);
    modelShader->setMat4("modelMatrix", towerModelMatrix);
    tower->Draw(*modelShader);
    glDisable(GL_BLEND);
}

void drawImposters() {
    modelMatrix = glm::mat4(1.0f);
    //modelMatrix = glm::scale(modelMatrix, 0.5f * glm::vec3(1.0f));
    glm::mat4 rotate90 = glm::rotate(90.0f, glm::vec3(0, 1, 0));
    updateViewProjMatrix(-6.0f*viewer.fowards, glm::vec3(0));
    imposterShader->use();
    imposterShader->setInt("pass", 0);
    imposterShader->setMat4("projViewMatrix", projView);
    imposterShader->setVec3("lightPos", glm::vec3(-5.0f*viewer.fowards) + glm::vec3(0, 10, 0));
    GLenum drawBuffers[] = {GL_COLOR_ATTACHMENT0};
    glm::mat4 modelRotation = glm::mat4(1.0f);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glm::mat4 M;
    for (int i = 0; i < rockModels.size(); i++) {
        M = glm::mat4(1.0f);
        if (i < 1) {
            // The first rock model requires a scaling
            M = glm::scale(M, 0.5f * glm::vec3(1.0f));
        } else {
            // The second requires a translation to the orgin
            M = glm::translate(M, -glm::vec3(-10.0958, 0.802082, 1.19187));
        }
        // Create 4 different views of each rock
        for (int j = 0; j < 4; j++) {
            //cout << 4 * i + j << "\n";
            imposterShader->setMat4("modelMatrix", M);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, imposterTextures[4 * i + j], 0);
            glDrawBuffers(1, drawBuffers);
            rockModels[i].Draw(*imposterShader);
            M = M * rotate90;
            glFlush();
        }
    }
 
    
    //Draw the imposter sprites
    updateViewProjMatrix(viewer.position, viewer.position + viewer.fowards);
    imposterShader->setMat4("projViewMatrix", projView);
    imposterShader->setInt("pass", 1);
    imposterShader->setInt("aspect", (float)WINDOW_WIDTH / WINDOW_HEIGHT);
    imposterShader->setVec3("lightPos", lightPosition);

    // Send the height map to the shader for normals
    glActiveTexture(GL_TEXTURE0);
    imposterShader->setInt("heightMap", 0);
    glBindTexture(GL_TEXTURE_2D, textureIds[0]);

    // Send the imposter textrues
    for (int i = 0; i < 8; i++) {
        glActiveTexture(GL_TEXTURE1 + i);
        imposterShader->setInt(("imposterTex" + to_string(i)).c_str(), i + 1);
        glBindTexture(GL_TEXTURE_2D, imposterTextures[i]);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindVertexArray(imposterSpritesVAO);
    glDrawArrays(GL_POINTS, 0, nRocks);

    // Clear the textures
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    for (int i = 0; i < 8; i++) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, imposterTextures[i], 0);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, lineFlag ? GL_LINE : GL_FILL);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    updateViewProjMatrix(viewer.position, viewer.position + viewer.fowards);
    
    drawSkybox();
    drawTerrian();
    drawImposters();
    drawSprites();
    drawModel();
    
    glutSwapBuffers();
}

void special(int key, int x, int y)
{
    int step = 0;
    float dir_x, dir_z;
    if (key == GLUT_KEY_LEFT) towerPosition.y += 0.1;   //in radians
    else if (key == GLUT_KEY_RIGHT) towerPosition.y -= 0.1;
    else if (key == GLUT_KEY_DOWN) towerPosition.z += 1;
    else if (key == GLUT_KEY_UP) towerPosition.z -= 1;
    cout << towerPosition.x << ", " << towerPosition.y << ", " << towerPosition.z << "\n";
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
            break;
        case 'f':
            lineFlag = !lineFlag;
            break;
    }
    viewer.move(dir);
    glutPostRedisplay();
}

void updateCamPos(int tick) {
    float minor = 8;
    float major = 5.5;
    float r = 5;
    int cycle = 8000;
    float t = (float)tick/cycle * M_2PI;
    viewer.position.x = minor * cos(t) + circleCenter.x;
    viewer.position.z = major * sin(t) + circleCenter.y;
    tick++;
    glutTimerFunc(10, updateCamPos, tick);
}

void animate(int val) {
    tick++;
    //updateCamPos(tick);
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
    //glutTimerFunc(10, updateCamPos, 0);
    glutMainLoop();
    return 0;
}

