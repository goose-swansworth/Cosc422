//  ========================================================================
//  COSC422: Advanced Computer Graphics;  University of Canterbury (2023)
//
//  FILE NAME: Skeleton Animation.cpp
//  See Exer04_SkeletalAnimation.pdf for details 
//  ========================================================================

#include <iostream>
#include <fstream>
#include <GL/freeglut.h>
#include <glm/glm.hpp>

using namespace std;

#include <assimp/cimport.h>
#include <assimp/types.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "assimp_extras.h"

#include "camera.h"
#include "models.h"
#include "model_loader.h"
#include "charater.h"

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 900
#define SEC_MS 1000

#define N_CHARATERS 2


//----------Globals----------------------------
//const aiScene* scene = NULL;
Model models[4];
Charater charaters[2];
aiVector3D scene_min, scene_max, scene_center;
float scene_scale;
float platform_height = 0.15;

float xrot, yrot, zrot = 0;

int tPitch = 643;
float pitch_step = 0;

Camera viewer(glm::vec3(0, 0, 7));
glm::vec2 mouse_last;

void print_vec(glm::vec3 v) {
    cout << "(" << v[0] << ", " << v[1] << ", " << v[2] << ")" << endl;
}


glm::vec3 get_join_location(Charater charater, aiString name) {
    aiVector3D P = aiVector3D(0, 0, 0);
    aiNode* node = charater.scene->mRootNode->FindNode(name);
    while (node->mParent != NULL) {
        P *= node->mTransformation;
        node = node->mParent;
	}
	P *= node->mTransformation;
    return glm::vec3(P.x, P.y, P.z);
}

// ------A recursive function to traverse scene graph and render each mesh----------
// Simplified version for rendering a skeleton mesh


//----- Function to update node matrices for each tick ------
// Complete this function
void updateNodeMatrices(int tick, Charater* charater)
{
	aiAnimation* anim;   //Animation object
	aiMatrix4x4 T, R;  //Position, rotation, product matrices
	aiMatrix3x3 matRot3;
	aiVector3D pos;
	aiQuaternion rot;
	aiNode* node;

	anim = charater->scene->mAnimations[0];
	for (int i = 0; i < anim->mNumChannels; i++) {
		T = aiMatrix4x4();   //Identity
		R = aiMatrix4x4();
		aiNodeAnim* channel = anim->mChannels[i];      //Channel

		if (channel->mNumPositionKeys == 1) {
			pos = channel->mPositionKeys[0].mValue;
		} else {
			pos = channel->mPositionKeys[tick].mValue;
		}
		if (channel->mNumRotationKeys == 1) {
			rot = channel->mRotationKeys[0].mValue;
		} else {
			rot = channel->mRotationKeys[tick].mValue;
		}
		
		T.Translation(pos, T);
		matRot3 = rot.GetMatrix();
		R = aiMatrix4x4(matRot3);
		
		node = charater->scene->mRootNode->FindNode(channel->mNodeName);
		node->mTransformation = T * R;
	}
}

void update(int value) {
	unsigned int timeStep = SEC_MS / charaters[0].scene->mAnimations[0]->mTicksPerSecond;
	unsigned int tDuration;
	for (int i = 0; i < 2; i++) {
		const aiScene* scene = charaters[i].scene;
		if (i == 0) {
			tDuration = scene->mAnimations[i]->mDuration;
		} else {
			tDuration = tPitch;
		}
		//timeStep += SEC_MS / charaters[i].scene->mAnimations[i]->mTicksPerSecond;
		if (charaters[i].tick > tDuration) {
			charaters[i].tick = 0;
		}
		updateNodeMatrices(charaters[i].tick, &charaters[i]);
		charaters[i].tick += charaters[i].tickStep;
	}
	pitch_step += 0.01;
	if (pitch_step >= 1.0) {
		pitch_step = 0;
	}
	glutPostRedisplay();
	glutTimerFunc(1, update, 0);
	
	
	
}

//--------------------OpenGL initialization------------------------
void initialise()
{
	float ambient[4] = { 0.2, 0.2, 0.2, 1.0 };  //Ambient light
	float white[4] = { 1, 1, 1, 1 };		    //Light's colour
	float black[4] = { 0, 0, 0, 1 };
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, black);   //Disable specular light
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glMatrixMode(GL_PROJECTION);
	glEnable(GL_MULTISAMPLE);
    glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);
	glLoadIdentity();
	gluPerspective(80, (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.2, 500.0);

	loadTextures();

	loadModel("../models/Bat/BaseballBat.obj", &models[0]);
	loadGLTextures(&models[0]);

	loadModel("../models/baseball_helmet.glb", &models[1]);

    loadModel("../models/Hat/Accesorios 14.obj", &models[2]);
    loadGLTextures(&models[2]);

    loadModel("../models/Ball/bbl_Ball.obj", &models[3]);
    loadGLTextures(&models[3]);


    charaters[0] = init_charater(
            aiImportFile("../bvh/hit.bvh", aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_Debone),
            glm::vec3(0, 0, -50),
            0,
            glm::vec3(0, 1, 0),
            0
    );

    charaters[1] = init_charater(
            aiImportFile("../bvh/pitch.bvh", aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_Debone),
            glm::vec3(10, 0, 50),
            180,
            glm::vec3(0, 1, 0),
            1
    );

	//---- Load the model ------
	//charaters[0].scene = aiImportFile("../bvh/hit.bvh", aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_Debone);
	if (charaters[0].scene == NULL) {
		cout << "bvh file not found.\n";
		exit(1);
	}

	//charaters[1].scene = aiImportFile("../bvh/pitch.bvh", aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_Debone);
	if (charaters[1].scene == NULL) {
		cout << "bvh file not found.\n";
		exit(1);
	}

	charaters[0].tickStep = 3;
	charaters[1].tickStep = 3;
	//printTreeInfo(scene->mRootNode);
	//printAnimInfo(scene, 0);

	get_bounding_box(charaters[0].scene, &scene_min, &scene_max);
	scene_center = (scene_min + scene_max) * 0.5f;
	aiVector3D scene_diag = scene_max - scene_center;
	scene_scale = 1.0 / scene_diag.Length();
}


//------The main display function---------
void display()
{
	float lightPosn[4] = { -5, 10, 10, 1 };
	// aiMatrix4x4 m = scene->mRootNode->mTransformation;
	// float xpos = m.a4;   //Root joint's position in world space
	// float ypos = m.b4;
	// float zpos = m.c4;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glm::vec3 look = viewer.position + viewer.fowards;
	gluLookAt(viewer.position.x, viewer.position.y, viewer.position.z, look.x, look.y, look.z, 0, 1, 0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosn);

	glPushMatrix();
    glScalef(scene_scale, scene_scale, scene_scale); // Scale the scene to the charaters
    glTranslatef(0, (platform_height - 0.055) / scene_scale, 0);
    //Render the charaters
    glm::vec3 pos, axis;
    for (int i = 0; i < N_CHARATERS; i++) {
        pos = charaters[i].position;
        axis = charaters[i].axis;
        glPushMatrix();
            glTranslatef(pos.x, pos.y, pos.z);
            glRotatef(charaters[i].angle, axis.x, axis.y, axis.z);
            renderCharater(charaters[i].scene->mRootNode, charaters[i], models);



        glPopMatrix();
	}
	
	// Render the baseball
	
	axis = charaters[1].axis;
	pos = charaters[1].position;
	float angle = charaters[1].angle;
	glm::vec3 src_pos = get_join_location(charaters[1], aiString("rhand"));
	glm::vec3 dest_pos = get_join_location(charaters[0], aiString("lhand")) + charaters[0].position;
	

	//src_pos = glm::vec3((glm::rotate(angle, axis) * glm::vec4(src_pos, 0.0)) + glm::vec4(pos, 1.0));

	
	glm::vec3 t = glm::mix(glm::vec3(src_pos), dest_pos, pitch_step);
	
	
	
	glPushMatrix();

		glTranslatef(pos.x, pos.y, pos.z);
		glRotatef(angle, axis.x, axis.y, axis.z);
		glTranslatef(src_pos.x, src_pos.y, src_pos.z);
		renderModel(models[3].scene->mRootNode, &models[3], false);

	glPopMatrix();


	glTranslatef(dest_pos.x, dest_pos.y, dest_pos.z);
	glutSolidCube(2);


    glPopMatrix();

	

    // Render the enviorment models
    glm::vec3 offset(8.8, yrot, zrot);
    glm::vec3 offset1(7.6, yrot, 1);
    platform(1, platform_height, (charaters[0].position + offset) * scene_scale);
    platform(1, platform_height, (charaters[1].position - offset1) * scene_scale);
	floorPlane(32, 4);
	enviroment();

    

	glutSwapBuffers();
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
		case 'u':
			dir.y = -1;
			break;
		case 'q':
			exit(0);
			break;
		case 'z':
            xrot += 0.2;
			cout << xrot << ", " << yrot << ", " << zrot << "\n";
			break;
		case 'x':
            yrot += 0.2;
			cout << xrot << ", " << yrot << ", " << zrot << "\n";
			break;
        case 'c':
            zrot += 0.2;
			cout << xrot << ", " << yrot << ", " << zrot << "\n";
			break;
	}
	viewer.move(dir);
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

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutSetOption(GLUT_MULTISAMPLE, 8);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Skeleton Animation");
	//glutSetCursor(GLUT_CURSOR_NONE);
	initialise();
	glutTimerFunc(0, update, 0);
	glutKeyboardFunc(keyboard_handler);
	glutPassiveMotionFunc(mouse_handler);
	glutDisplayFunc(display);
	glutMainLoop();
}

