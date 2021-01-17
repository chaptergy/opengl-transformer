#define _CRT_SECURE_NO_DEPRECATE

#include <GL/freeglut.h>
#include "utils.h"
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include "textureVars.h"

GLuint tex_sky;
GLuint tex_road;
GLuint tex_roof;

/**
* Custom cosine function for 20-vertex circle 
* 
* @param step Which vertex of the circle is being calculated (range [0-20])
* 
* @return The cos of that vertex
*/

float customCos(unsigned int step) {
	if (step <= 5) {
		return sincos[step];
	}
	else if (step <= 10) {
		return -sincos[10-step];
	}
	else if (step <= 15) {
		return -sincos[step - 10];
	}
	else if (step <= 20) {
		return sincos[20 - step];
	}
	else {
		return 0;
	}
}

/**
* Custom sine function for 20-vertex circle
* 
* @param step Which vertex of the circle is being calculated (range [0-20])
*
* @return The sin of that vertex
*/
float customSin(int step) {
	if (step <= 5) {
		return sincos[5 - step];
	}
	else if (step <= 10) {
		return sincos[step - 5];
	}
	else if (step <= 15) {
		return -sincos[15 - step];
	}
	else if (step <= 20) {
		return -sincos[step - 15];
	}
	else {
		return 0;
	}
}

/**
* Enable sunlight for the scene
*/
void sunlight() {
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	float light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	float light_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	float light_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	float light_position[] = { -1, 2, 2, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

/**
* Sets the material color o the currently drawn object
* 
* @param r the red-channel of the color
* @param g the green-channel of the color
* @param b the blue-channel of the color
* @param a the alpha-channel of the color
*/
void materialColor(float r, float g, float b, float a) {
	previousMaterialColor[0] = currentMaterialColor[0];
	previousMaterialColor[1] = currentMaterialColor[1];
	previousMaterialColor[2] = currentMaterialColor[2];
	previousMaterialColor[3] = currentMaterialColor[3];
	currentMaterialColor[0] = r;
	currentMaterialColor[1] = g;
	currentMaterialColor[2] = b;
	currentMaterialColor[3] = a;
	float mat_specular[] = { 1.0f, 1.0f, 1.0f, 0.5f };
	float shine[] = { 128 };

	glMaterialfv(GL_FRONT, GL_AMBIENT, currentMaterialColor);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, currentMaterialColor);
	//glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	//glMaterialfv(GL_FRONT, GL_SHININESS, shine);
}
/**
* Revert he material color to the previously used one.
* The color can only be reverted once in a row.
*/
void revertMaterialColor() {
	if (previousMaterialColor[0] != -1)
		materialColor(previousMaterialColor[0], previousMaterialColor[1], previousMaterialColor[2], previousMaterialColor[3]);
	else
		materialColor(1.0f, 1.0f, 1.0f, 1.0f);
}

/**
* Cubic Easing between 0 and 1
*/
float easeInOut(float t) {
	return t < .5 ? 4 * t*t*t : (t - 1)*(2 * t - 2)*(2 * t - 2) + 1;
}

/**
* Loads a texture from file.
* 
* @param filePath The path to the image file
* @param imageMode Which OpenGL color mode should be used to load the image
* @param wrap_hor How to handle horizontal wrapping. See GL_TEXTURE_WRAP_S
* @param wrap_ver How to handle vertical wrapping. See GL_TEXTURE_WRAP_T
* 
* @return Generated OpenGLs texure name
*/
GLuint loadTexture(const char * filePath, GLint imageMode = GL_BGR_EXT, GLint wrap_hor = GL_CLAMP, GLint wrap_ver = GL_CLAMP) {

	unsigned char header[54]; // BMP header
	unsigned int dataPos;     // Position of data in file
	unsigned int width, height;
	unsigned int imageSize;   // = width*height*3
	unsigned char * data; // RGB-Data

	// Open File
	FILE * file = fopen(filePath, "rb");
	if (!file) { printf("Unable to load file: %s\n", strerror(errno)); return 0; }

	if (fread(header, 1, 54, file) != 54) {
		printf("File is not a valid BMP-File\n");
		return 0;
	}

	if (header[0] != 'B' || header[1] != 'M') {
		printf("File is not a valid BMP-File\n");
		return 0;
	}

	// read from byte array
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	// guess missing information
	if (imageSize == 0)    imageSize = width * height * 3; // 3 channels (rgb)
	if (dataPos == 0)      dataPos = 54; // BMP-Header always has 54 bytes

	// create buffer and fill
	data = new unsigned char[imageSize];
	fread(data, 1, imageSize, file);
	fclose(file);

	GLuint textureID;

	glGenTextures(1, &textureID); // Generate a texturename and save it in textureID

	// make texture "active"
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Have OpenGL process the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, imageMode, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // use linear for resizing image larger
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // use linear for resizing image smaller
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_hor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_ver);

	glFlush();

	return textureID;
}

/**
* Loads and processes all texture files
*/
void loadAllTextureFiles() {
	tex_sky = loadTexture("./textures/sky.bmp",GL_BGR_EXT, GL_REPEAT);
	tex_road = loadTexture("./textures/road.bmp",GL_RGB, GL_REPEAT);
	tex_roof = loadTexture("./textures/roof.bmp",GL_BGR_EXT, GL_REPEAT, GL_REPEAT);
}

