//  ========================================================================
//  COSC422: Computer Graphics (2023);  University of Canterbury.
//  FILE NAME: TerrainP.cpp
//  See Exer14.pdf for details.
//
//	The program generates and loads the mesh data for a terrain floor (100 verts, 81 elems).
//  Requires files  Terrain.vert, Terrain.frag
//                  Terrain.cont, Terrains.eval
//  ========================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "loadTGA.h" 
#include "camera.h"
using namespace std;

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900

Camera viewer(glm::vec3(0, 0, 7));
static glm::vec2 mouse_last;

GLuint vaoID;
GLuint theProgram;
GLuint mvpMatrixLoc, eyeLoc, zNear, zFar, waveTick, waterHeight, lightPosLoc;
const unsigned int numTextures = 5; 
GLuint textureIds[numTextures];
float  eye_x = 0, eye_y = 20, eye_z = 30;      //Initial camera position
float look_x = 0, look_y = 0, look_z = -40;    //"Look-at" point along -z direction
float  theta = 0;                              //Look angle
float toRad = 3.14159265/180.0;     //Conversion from degrees to rad
float waterLevelf = 2.2;

float verts[100*3];       //10x10 grid (100 vertices)
GLushort elems[81*4];     //Element array for 9x9 = 81 quad patches

glm::mat4 projView;
glm::vec3 lightPosition(0, 20, 0);

static const std::vector<std::string> texFilenames = {
		"../bays.tga",
		"../grass.tga",
		"../water.tga",
		"../sand.tga",
		"../browngrass.tga"
	};

static const std::vector<std::string> texNames = {
		"heightMap",
		"grassTex",
		"waterTex",
		"sandTex",
		"brownGrassTex"
	};

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
			verts[3*indx] = 10*i - 45;		//x
			verts[3*indx+1] = 0;			//y
			verts[3*indx+2] = -10*j;		//z
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

void loadTexture(const char* filename, int index, int glTexNum) {
	glActiveTexture(glTexNum);
    glBindTexture(GL_TEXTURE_2D, textureIds[index]);
	loadTGA(filename);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

//Loads height map
void loadTextures() {
    glGenTextures(numTextures, textureIds);
	for (int i = 0; i < texFilenames.size(); i++) {
		loadTexture(texFilenames[i].c_str(), i, GL_TEXTURE0 + i);
	}
}

//Loads a shader file and returns the reference to a shader object
GLuint loadShader(GLenum shaderType, string filename)
{
	ifstream shaderFile(filename.c_str());
	if(!shaderFile.good()) cout << "Error opening shader file." << endl;
	stringstream shaderData;
	shaderData << shaderFile.rdbuf();
	shaderFile.close();
	string shaderStr = shaderData.str();
	const char* shaderTxt = shaderStr.c_str();

	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderTxt, NULL);
	glCompileShader(shader);
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);
		const char *strShaderType = NULL;
		cerr <<  "Compile failure in shader: " << strInfoLog << endl;
		delete[] strInfoLog;
	}
	return shader;
}

//Initialise the shader program, create and load buffer data
void initialise()
{
	
	viewer.position = glm::vec3(-1, 8, -31);
//--------Load terrain height map-----------
	loadTextures();
//--------Load shaders----------------------
	GLuint shaderv = loadShader(GL_VERTEX_SHADER, "../Terrain.vert");
	GLuint shaderf = loadShader(GL_FRAGMENT_SHADER, "../Terrain.frag");
	GLuint shaderc = loadShader(GL_TESS_CONTROL_SHADER, "../Terrain.cont");
	GLuint shadere = loadShader(GL_TESS_EVALUATION_SHADER, "../Terrain.eval");

	GLuint program = glCreateProgram();
	glAttachShader(program, shaderv);
	glAttachShader(program, shaderf);
	glAttachShader(program, shaderc);
	glAttachShader(program, shadere);

	glLinkProgram(program);

	GLint status;
	glGetProgramiv (program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
		fprintf(stderr, "Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;
	}
	glUseProgram(program);

	mvpMatrixLoc = glGetUniformLocation(program, "mvpMatrix");
	eyeLoc = glGetUniformLocation(program, "eyePos");
	zNear = glGetUniformLocation(program, "zNear");
	zFar = glGetUniformLocation(program, "zFar");
	waveTick = glGetUniformLocation(program, "waveTick");
	waterHeight = glGetUniformLocation(program, "waterHeight");
	glUniform1i(waveTick, 0);
	glUniform1f(waterHeight, waterLevelf);

	GLuint texLoc;
	for (int i = 0; i < texNames.size(); i++) {
		texLoc = glGetUniformLocation(program, texNames[i].c_str());
		glUniform1i(texLoc, i);
	}

	lightPosLoc = glGetUniformLocation(program, "lightPos");
	glUniform3fv(lightPosLoc, 1, &lightPosition[0]);

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

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

//Display function to compute uniform values based on transformation parameters and to draw the scene
void display()
{
	glm::vec4 cameraPosn = glm::vec4(viewer.position, 1.0);
	glm::vec3 look = viewer.position + viewer.fowards;
	glUniform4fv(eyeLoc, 1, &cameraPosn[0]);
	glUniform1i(zNear, 20);
	glUniform1i(zFar, 500);

	//--------Compute matrices----------------------
	float aspect = (float)WINDOW_WIDTH / WINDOW_HEIGHT;
	glm::mat4 proj = glm::perspective(80.0f * toRad, aspect, 1.0f, 500.0f);  //perspective projection matrix
	glm::mat4 view = lookAt(viewer.position, look, glm::vec3(0.0, 1.0, 0.0)); //view matri
	projView = proj * view;  //Product matrix
	glUniformMatrix4fv(mvpMatrixLoc, 1, GL_FALSE, &projView[0][0]);

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindVertexArray(vaoID);
	glDrawElements(GL_PATCHES, 81 * 4, GL_UNSIGNED_SHORT, NULL);
	glutSwapBuffers();
}

void special(int key, int x, int y)
{
	int step = 0;
	float dir_x, dir_z;
	if (key == GLUT_KEY_LEFT) lightPosition.x += 1;   //in radians
	else if (key == GLUT_KEY_RIGHT) lightPosition.x -= 1;
	else if (key == GLUT_KEY_DOWN) lightPosition.z -= 1;
	else if (key == GLUT_KEY_UP) lightPosition.z += 1;
	cout << lightPosition.x << ", " << lightPosition.y << ", " << lightPosition.z << "\n";
	glUniform1f(waterHeight, waterLevelf);
	glUniform3fv(lightPosLoc, 1, &lightPosition[0]);
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
		case 't':
			lightPosition.y += 0.1;
			break;
		case 'g':
			lightPosition.y -= 0.1;
			break;
		case 'q':
			exit(0);
	}
	cout << viewer.position.x << ", " << viewer.position.y << ", " << viewer.position.z << "\n";
	glUniform3fv(lightPosLoc, 1, &lightPosition[0]);
	viewer.move(dir);
	glutPostRedisplay();
}

void animate(int tick) {
	tick++;
	glUniform1i(waveTick, tick);
	glutTimerFunc(50, animate, tick);
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

