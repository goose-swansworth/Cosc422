//  ========================================================================
//  COSC422: Advanced Computer Graphics
//  Assignment 1: Part 1 - Keetley Rate
//
//  Based on: Skeleton Animation.cpp
//  ========================================================================

#include <iostream>
#include <fstream>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

using namespace std;

#include <assimp/cimport.h>
#include <assimp/types.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "assimp_extras.h"

#include "camera.h"
#include "models.h"
#include "model_loader.h"
#include "character.h"
#include "shadow.h"

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900
#define SEC_MS 1000

#define N_CHARATERS 2
#define HITTER 0
#define PITCHER 1

// Model and Character arrays
Model models[4];
Character characters[2];
aiVector3D scene_min, scene_max, scene_center;
float scene_scale;
float platform_height = 0.15;

const float lightPosn[4] = {0, 100, 50, 1};
const float plane[4] = {0, 1, 0, 0};
static float shadowMat[16];

const glm::vec3 platformOffset0(8.8, 0, 0);
const glm::vec3 platformOffset1(7.6, 0, 1);

// Globals associated with the pitch/hit animation
static bool thrown = false;
static bool hit = false;
static glm::vec3 srcPos;
static glm::vec3 destPos;
static glm::vec3 ballPos;
static glm::vec3 hitDir;
const unsigned int firstHit = 324;
const unsigned int secondHit = 1011;
const unsigned int pitchTime = 438;
const int tPitch = 643;
const float hitRadius = 50.0f;
float pitchStep = 0;
float hitStep = 0;
const float pitchDelta = 0.1;
const float hitDelta = 0.15;
const glm::vec3 g(0, -4.9, 0);

// Viewer - Camera realted globals
Camera viewer(glm::vec3(0, 0, 7));
static glm::vec2 mouse_last;
static bool batterView = false;

static bool pitcherView = false;

const glm::vec3 pitcherViewPos1(3.59082, 1.49267, 7.90967);
const glm::vec3 pitcherViewFoward1(-0.655134, -0.160241, -0.738324);

const glm::vec3 pitcherViewPos2(-1.79289, 1.89172, 9.64499);
const glm::vec3 pitcherViewFoward2(0.0712425, -0.224809, -0.971795);

const glm::vec3 hitterViewPos1(-1.54559, 0.746906, -9.03904);
const glm::vec3 hitterViewFoward1(0.403485, 0.136573, 0.904736);

const glm::vec3 hitterViewPos2(3.2979, 1.39968, -8.78759);
const glm::vec3 hitterViewFoward2(-0.502268, -0.0378983, 0.863881);

const glm::vec3 perspectiveOffset(-8.7, 1, -15.4);

float xoff = 0;
float yoff= 0;
float zoff = 0;

glm::vec3 get_join_location(Character character, aiString name) {
    aiVector3D P = aiVector3D(0, 0, 0);
    aiNode* node = character.scene->mRootNode->FindNode(name);
    while (node->mParent != NULL) {
        P *= node->mTransformation;
        node = node->mParent;
	}
	P *= node->mTransformation;
    return glm::vec3(P.x, P.y, P.z);
}

void updateNodeMatrices(int tick, Character* character) {
	aiAnimation* anim;   
	aiMatrix4x4 T, R;  
	aiMatrix3x3 matRot3;
	aiVector3D pos;
	aiQuaternion rot;
	aiNode* node;

	anim = character->scene->mAnimations[0];
	for (int i = 0; i < anim->mNumChannels; i++) {
		T = aiMatrix4x4();   
		R = aiMatrix4x4();
		aiNodeAnim* channel = anim->mChannels[i];     

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
		
		node = character->scene->mRootNode->FindNode(channel->mNodeName);
		node->mTransformation = T * R;
	}
}

void updateCharaters(int value) {
	unsigned int timeStep = SEC_MS / characters[HITTER].scene->mAnimations[0]->mTicksPerSecond;
	unsigned int hitDur = characters[HITTER].scene->mAnimations[0]->mDuration;
	unsigned int pitchDur = tPitch;
	
	if (characters[HITTER].tick < pitchDur) {
		characters[HITTER].tick += characters[HITTER].tickStep;
	} else {
		characters[HITTER].tick = 0;
	}

	if (characters[HITTER].tick <= firstHit) {
		float t = (float)characters[HITTER].tick / firstHit;
		characters[PITCHER].tick = (int)(pitchTime * t);
	} else {
		float t = (float)(characters[HITTER].tick - firstHit) / (secondHit - firstHit);
		characters[PITCHER].tick = (int)(pitchTime + pitchTime*t) % pitchDur;
	}

	if (characters[PITCHER].tick >= pitchTime) {
		thrown = true;
		pitchStep += pitchDelta;
	}

	if (characters[HITTER].tick % pitchDur < 1) {
		thrown = false; 
		hit = false;
		pitchStep = 0;
		hitStep = 0;
	}

	if (characters[HITTER].tick >= firstHit + 25 && !hit) {
		hit = true;
		hitDir = glm::ballRand(hitRadius);
		hitDir.z = abs(hitDir.z);
		hitDir.y = abs(hitDir.y) + 20;
	}

	if (hit) {
		hitStep += hitDelta;
	} 

	for (int i = 0; i < N_CHARATERS; i++) {
		updateNodeMatrices(characters[i].tick, &characters[i]);
	}
	glutPostRedisplay();
	glutTimerFunc(1, updateCharaters, 0);
}

void renderCharaters() {
	glm::vec3 pos, axis;
    for (int i = 0; i < N_CHARATERS; i++) { 
        pos = characters[i].position;
        axis = characters[i].axis;
        glPushMatrix();
            glTranslatef(pos.x, pos.y, pos.z);
            glRotatef(characters[i].angle, axis.x, axis.y, axis.z);
            renderCharacter(characters[i].scene->mRootNode, characters[i], models);
        glPopMatrix();
	}
}

void renderBaseball(bool shadow) {
	glm::vec3 axis = characters[PITCHER].axis;
	glm::vec3 pos = characters[PITCHER].position;
	float angle = characters[PITCHER].angle; 
	glPushMatrix();
		if (!thrown && !hit) {
			srcPos = get_join_location(characters[PITCHER], aiString("rhand"));
			glTranslatef(pos.x, pos.y, pos.z);
			glRotatef(angle, axis.x, axis.y, axis.z);
			glTranslatef(srcPos.x, srcPos.y, srcPos.z);
			srcPos = glm::vec3(
				glm::translate(pos) * glm::rotate(angle, axis) * glm::translate(srcPos) * glm::vec4(0.0, 0.0, 0.0, 1.0)
			);
			destPos = get_join_location(characters[HITTER], aiString("lhand")) + characters[HITTER].position;
			ballPos = srcPos;
		} else if (thrown && !hit) {
			glm::vec3 t = glm::mix(srcPos, destPos, pitchStep);
			ballPos = t;
			glTranslatef(t.x, t.y, t.z);
		} else {
			// Projectile motion
			float s = glm::clamp(pitchStep, 0.0f, 1.0f);
			glm::vec3 vi = hitDir - destPos;
			glm::vec3 delta = vi * s + 0.5f * g * s * s;
			glm::vec3 t = destPos + delta;
			ballPos = t;
			glTranslatef(t.x, t.y, t.z);
		}
		glRotatef(glm::degrees(2 * M_PI) * (hitStep + pitchStep), 1, 0.1, 0);
		renderModel(models[BALL].scene->mRootNode, &models[BALL], shadow);
	glPopMatrix();
}

void initialise() {
	float ambient[4] = { 0.2, 0.2, 0.2, 1.0 };  
	float white[4] = { 1, 1, 1, 1 };		    
	float black[4] = { 0, 0, 0, 1 };
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, black);   
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

	// Load all models and associated textures
	loadModel("../models/Bat/BaseballBat.obj", &models[BAT]);
	loadGLTextures(&models[BAT]);

	loadModel("../models/baseball_helmet.glb", &models[HELMET]);

    loadModel("../models/Hat/Accesorios 14.obj", &models[HAT]);
    loadGLTextures(&models[HAT]);

    loadModel("../models/Ball/bbl_Ball.obj", &models[BALL]);
    loadGLTextures(&models[BALL]);

	// Initialise characters, load bvh files
    characters[HITTER] = init_character(
            aiImportFile("../bvh/hit.bvh", aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_Debone),
            glm::vec3(0, 0, -112.1),
            0,
            glm::vec3(0, 1, 0),
            0
    );

    characters[PITCHER] = init_character(
            aiImportFile("../bvh/pitch.bvh", aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_Debone),
            glm::vec3(10, 0, 112.1),
            180,
            glm::vec3(0, 1, 0),
            1
    );

	if (characters[HITTER].scene == NULL) {
		cout << "bvh file not found.\n";
		exit(1);
	}
	if (characters[PITCHER].scene == NULL) {
		cout << "bvh file not found.\n";
		exit(1);
	}

	characters[HITTER].tickStep = 3;
	characters[PITCHER].tickStep = 3;
	viewer.position = pitcherViewPos1;
	viewer.fowards = pitcherViewFoward1;

	shadowMatrix(plane, lightPosn, shadowMat);

	get_bounding_box(characters[HITTER].scene, &scene_min, &scene_max);
	scene_center = (scene_min + scene_max) * 0.5f;
	aiVector3D scene_diag = scene_max - scene_center;
	scene_scale = 1.0 / scene_diag.Length();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Orient the viewer
	if (batterView) {
		viewer.position = scene_scale * (get_join_location(characters[HITTER], aiString("Head")) + characters[HITTER].position + perspectiveOffset);
		if (pitchStep < 1) {
			viewer.fowards = glm::normalize(srcPos - viewer.position);
		} else {
			viewer.fowards = glm::normalize(ballPos - viewer.position);
		}
		//  else {
		// 	glm::vec3 tangent = glm::normalize(hitDir - destPos + g * hitStep);
		// 	viewer.position = scene_scale * (ballPos - tangent)  ;
		// 	viewer.fowards = tangent;
		// }
	}
	glm::vec3 look = viewer.position + viewer.fowards;
	gluLookAt(viewer.position.x, viewer.position.y, viewer.position.z, look.x, look.y, look.z, 0, 1, 0);

	glLightfv(GL_LIGHT0, GL_POSITION, lightPosn);

	// Render the enviorment models
	glPushMatrix();
		platform(1, platform_height, (characters[HITTER].position + platformOffset0) * scene_scale);
		platform(1, platform_height, (characters[PITCHER].position - platformOffset1) * scene_scale);
	glPopMatrix();
	glPushMatrix();
		glRotatef(42.5, 0, 1, 0);
		floorPlane(40, 1);
	glPopMatrix();
	enviroment();

	
	glPushMatrix();
		glScalef(scene_scale, scene_scale, scene_scale); // Scale the scene to the characters
		glTranslatef(0, (platform_height - 0.055) / scene_scale, 0);

		//Render the characters and baseball
		renderCharaters();
		renderBaseball(false);

		// Render the baseballs shadow
		if (hit || thrown) {
			glPushMatrix();
				glMultMatrixf(shadowMat);
				glTranslatef(0, 0.001, 0);
				glDisable(GL_TEXTURE_2D);
				glColor3f(0.3, 0.3, 0.3);
				renderBaseball(true);
				glEnable(GL_TEXTURE_2D);
			glPopMatrix();
		}
    glPopMatrix();
	glutSwapBuffers();
}

void keyboard_handler(unsigned char key, int x, int y) {
	glm::vec3 dir(0.0);
	switch (key) {
		case 'w':
			//dir.x = 1;
			xoff += 0.1;
			break;
		case 's':
			//dir.x = -1;
			xoff -= 0.1;
			break;
		case 'a':
			//dir.z = -1;
			zoff += 0.1;
			break;
		case 'd':
			//dir.z = 1;
			zoff -= 0.1;
			break;
		case ' ':
			//dir.y = 1;
			yoff += 0.1;
			break;
		case 'z':
			yoff -= 0.1;
			break;
		case '1':
			viewer.position = pitcherViewPos1;
			viewer.fowards = pitcherViewFoward1;
			break;
		case '2':
			viewer.position = pitcherViewPos2;
			viewer.fowards = pitcherViewFoward2;
			break;
		case '3':
			viewer.position = hitterViewPos1;
			viewer.fowards = hitterViewFoward1;
			break;
		case '4':
			viewer.position = hitterViewPos2;
			viewer.fowards = hitterViewFoward2;
			break;
		case '5':
			batterView = !batterView;
			break;
		case 'q':
			exit(0);
	}
	cout << xoff << ", " << yoff << ", " << zoff << "\n";
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

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutSetOption(GLUT_MULTISAMPLE, 8);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Play Ball!");
	glutSetCursor(GLUT_CURSOR_NONE);
	initialise();
	glutTimerFunc(0, updateCharaters, 0);
	glutKeyboardFunc(keyboard_handler);
	glutPassiveMotionFunc(mouse_handler);
	glutDisplayFunc(display);
	glutMainLoop();
}

