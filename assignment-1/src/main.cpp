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
#include "stb_image.h"

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 900
#define SEC_MS 1000

//----------Globals----------------------------
const aiScene* scene = NULL;
aiVector3D scene_min, scene_max, scene_center;
float scene_scale;


Camera viewer(glm::vec3(0, 0, 7));
glm::vec2 mouse_last;

void print_vec(glm::vec3 v) {
    cout << "(" << v[0] << ", " << v[1] << ", " << v[2] << ")" << endl;
}



bool replace_links(aiString name) {
	float r = 1;
	if (name == aiString("Chest")) {
		chest();
		return true;
	} else if (name == aiString("lowerback")) {
		middle();
		return true;
	}  else if (name == aiString("Head")) {
		head();
		return true;
	} else if (name == aiString("LeftShoulder") || name == aiString("RightShoulder")) {
		shoulder();
		return true;
	} else if (name == aiString("RightHip")) {
		rightHip();
		return true;
	}  else if (name == aiString("LeftHip")) {
		leftHip();
		return true;
	} else if (name == aiString("LeftKnee")) {
		knee(true);
		return true;
	}  else if (name == aiString("RightKnee")) {
		knee(false);
		return true;
	}  else if (name == aiString("LeftElbow") || name == aiString("RightElbow")) {
		elbow();
		return true;
	} else if (name == aiString("lhand") || name == aiString("rhand")) {
		hand();
		return true;
	} else if (name == aiString("LHipJoint") || name == aiString("RHipJoint")) {
		return true;
	} else if (name == aiString("RightAnkle")) {
		foot(1.5, false);
		return true;
	} else if (name == aiString("LeftAnkle")) {
		foot(1.5, true);
		return true;
	} else if (name == aiString("RightToe") || name == aiString("LeftToe")) {
		return true;
	}
	return false; 
}

// ------A recursive function to traverse scene graph and render each mesh----------
// Simplified version for rendering a skeleton mesh
void render(const aiNode* node, bool links)
{
	aiMatrix4x4 m = node->mTransformation;
	aiMesh* mesh;
	aiFace* face;
	float materialCol[4] = { 1, 0, 1, 1 };
	int meshIndex;

	m.Transpose();   //Convert to column-major order
	glPushMatrix();
	glMultMatrixf((float*)&m);   //Multiply by the transformation matrix for this node

	//The scene graph for a skeleton contains at most one mesh per node
	//Skeleton meshes are always triangle meshes
	

	if(node->mNumMeshes > 0 && links) {

		if (!replace_links(node->mName)) {

			meshIndex = node->mMeshes[0];          //Get the mesh indices from the current node
			mesh = scene->mMeshes[meshIndex];    //Using mesh index, get the mesh object
			glColor4fv(materialCol);   //Default material colour

			//Draw the mesh in the current node
			for (int k = 0; k < mesh->mNumFaces; k++)
			{
				face = &mesh->mFaces[k];
				glBegin(GL_TRIANGLES);
				for (int i = 0; i < face->mNumIndices; i++) {
					int vertexIndex = face->mIndices[i];
					if (mesh->HasNormals())
						glNormal3fv(&mesh->mNormals[vertexIndex].x);
					glVertex3fv(&mesh->mVertices[vertexIndex].x);
				}
				glEnd();
			}
		}
	} else {
		replace_links(node->mName);
	}

	// Recursively draw all children of the current node
	for (int i = 0; i < node->mNumChildren; i++)
		render(node->mChildren[i], links);
		

	glPopMatrix();
}

//----- Function to update node matrices for each tick ------
// Complete this function
void updateNodeMatrices(int tick)
{
	aiAnimation* anim;   //Animation object
	aiMatrix4x4 T, R;  //Position, rotation, product matrices
	aiMatrix3x3 matRot3;
	aiVector3D pos;
	aiQuaternion rot;
	aiNode* node;

	anim = scene->mAnimations[0];
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
		
		node = scene->mRootNode->FindNode(channel->mNodeName);
		node->mTransformation = T * R;
	}
}

void update(int tick) {
	unsigned int tDuration = scene->mAnimations[0]->mDuration;
	unsigned int timeStep = SEC_MS / scene->mAnimations[0]->mTicksPerSecond;
	if (tick > tDuration) {
		tick = 0;
	}
	updateNodeMatrices(tick);
	tick++;
	glutTimerFunc(timeStep, update, tick);
	glutPostRedisplay();
	
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
	gluPerspective(70, (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.2, 500.0);

	loadTextures();

	//---- Load the model ------
	scene = aiImportFile("../bvh/89_04.bvh", aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_Debone);
	if (scene == NULL) {
		cout << "bvh file not found.\n";
		exit(1);
	}
	//printTreeInfo(scene->mRootNode);
	//printAnimInfo(scene, 0);

	get_bounding_box(scene, &scene_min, &scene_max);
	scene_center = (scene_min + scene_max) * 0.5f;
	aiVector3D scene_diag = scene_max - scene_center;
	scene_scale = 1.0 / scene_diag.Length();
}


//------The main display function---------
void display()
{
	float lightPosn[4] = { -5, 10, 10, 1 };
	aiMatrix4x4 m = scene->mRootNode->mTransformation;
	float xpos = m.a4;   //Root joint's position in world space
	float ypos = m.b4;
	float zpos = m.c4;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glm::vec3 look = viewer.position + viewer.fowards;
	gluLookAt(viewer.position.x, viewer.position.y, viewer.position.z, look.x, look.y, look.z, 0, 1, 0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosn);

	glPushMatrix();
	   glScalef(scene_scale, scene_scale, scene_scale);
	   //glTranslatef(-xpos, 0, -zpos);   //Move model to origin
	   render(scene->mRootNode, false);
	glPopMatrix();
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
		case 'z':
			dir.y = -1;
			break;
		case 'q':
			exit(0);
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

