#include <GL/freeglut.h>
#include <math.h>
#include <iostream>
#include "shapes.h"
#include "utils.h"
#include "textureVars.h"

#define PI 3.14159265358979323846
/**
* Draws a wheel
* 
* @param radius The radius of the wheel
* @param width with of the wheel (height of the cylinder)
*/
void wheel(GLfloat radius, GLfloat width) {
	const int sides = 20;
	const float angleStepSize = ((2*PI) / sides) * 1.0f;

	materialColor(0.5f, 0.5f, 0.5f, 1.0f);
	// Tire thread	
	{
		float angle = 0.0f;
		while (angle < 2 * PI) {
			glBegin(GL_POLYGON);
			{
				float xpos1 = radius * cos(angle);
				float ypos1 = radius * sin(angle);

				glNormal3f(sin(angle), cos(angle), 0.0f);
				glTexCoord2f(0.0f, 1.0f); glVertex3f(xpos1, ypos1, width / 2.0f);
				glTexCoord2f(0.0f, 0.0f); glVertex3f(xpos1, ypos1, -width / 2.0f);

				angle += angleStepSize;

				float xpos2 = radius * cos(angle);
				float ypos2 = radius * sin(angle);

				glNormal3f(sin(angle), cos(angle), 0.0f);
				glTexCoord2f(1.0f, 0.0f); glVertex3f(xpos2, ypos2, -width / 2.0f);
				glTexCoord2f(1.0f, 1.0f); glVertex3f(xpos2, ypos2, width / 2.0f);
			}
			glEnd();
		}
	}

	// Tire Cap 1
	glNormal3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_POLYGON);
	{
		for (int i = 0; i < 20; i++){

			float xpos = radius * customCos(i);
			float ypos = radius * customSin(i);

			//glTexCoord2d((customCos(i) + 1.0f) / 2.0f, (customSin(i) + 1.0f) / 2.0f);
			glVertex3f(xpos, ypos, width / 2.0f);
		}
	}
	glEnd();
	
	// Tire Cap 2
	glNormal3f(0.0f, 0.0f, -1.0f);
	glBegin(GL_POLYGON);
	{
		float winkel = 0.0f;
		while (winkel < 2 * PI) {
			float xpos = radius * cos(winkel);
			float ypos = radius * sin(winkel);

			//glTexCoord2d((xpos + radius) / (2 * radius), ((2 * radius) - (ypos + radius)) / (2 * radius));
			glVertex3f(xpos, ypos, -width / 2.0f);

			winkel += angleStepSize;
		}
	}
	glEnd();

	revertMaterialColor();
}

/**
* Draws a cuboid with its origin centered on the bottom face
* 
* @param width The width of the cuboid
* @param height The height of the cuboid
* @param depth The depth of the cuboid
*/
void cuboid(GLfloat width, GLfloat height, GLfloat depth)
{
	// Front
	glNormal3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_POLYGON);
	glVertex3f(-width / 2.0f, 0.0f, +depth / 2.0f);
	glVertex3f(+width / 2.0f, 0.0f, +depth / 2.0f);
	glVertex3f(+width / 2.0f, +height, +depth / 2.0f);
	glVertex3f(-width / 2.0f, +height, +depth / 2.0f);
	glEnd();

	// Right
	glNormal3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_POLYGON);
	glVertex3f(+width / 2.0f, 0.0f, +depth / 2.0f);
	glVertex3f(+width / 2.0f, 0.0f, -depth / 2.0f);
	glVertex3f(+width / 2.0f, +height, -depth / 2.0f);
	glVertex3f(+width / 2.0f, +height, +depth / 2.0f);
	glEnd();

	// Back
	glNormal3f(0.0f, 0.0f, -1.0f);
	glBegin(GL_POLYGON);
	glVertex3f(+width / 2.0f, +height, -depth / 2.0f);
	glVertex3f(+width / 2.0f, 0.0f, -depth / 2.0f);
	glVertex3f(-width / 2.0f, 0.0f, -depth / 2.0f);
	glVertex3f(-width / 2.0f, +height, -depth / 2.0f);
	glEnd();

	// Left
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glBegin(GL_POLYGON);
	glVertex3f(-width / 2.0f, +height, -depth / 2.0f);
	glVertex3f(-width / 2.0f, 0.0f, -depth / 2.0f);
	glVertex3f(-width / 2.0f, 0.0f, +depth / 2.0f);
	glVertex3f(-width / 2.0f, +height, +depth / 2.0f);
	glEnd();

	// Top
	glNormal3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_POLYGON);
	glVertex3f(-width / 2.0f, +height, +depth / 2.0f);
	glVertex3f(+width / 2.0f, +height, +depth / 2.0f);
	glVertex3f(+width / 2.0f, +height, -depth / 2.0f);
	glVertex3f(-width / 2.0f, +height, -depth / 2.0f);
	glEnd();

	// Bottom
	glNormal3f(0.0f, -1.0f, 0.0f);
	glBegin(GL_POLYGON);
	glVertex3f(-width / 2.0f, 0.0f, -depth / 2.0f);
	glVertex3f(+width / 2.0f, 0.0f, -depth / 2.0f);
	glVertex3f(+width / 2.0f, 0.0f, +depth / 2.0f);
	glVertex3f(-width / 2.0f, 0.0f, +depth / 2.0f);
	glEnd();
}

/**
* Draws the cockpit with its origin centered on the bottom face
*
* @param width The width of the cockpit
* @param height The height of the cockpit
* @param depth The depth of the cockpit
* @param windowFrameWidth The width of the window frame
*/
void cockpit(GLfloat width, GLfloat height, GLfloat depth, GLfloat windowFrameWidth) {

	// Back
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glBegin(GL_POLYGON);
	glVertex3f(-width / 2.0f, +height, +depth / 2.0f);
	glVertex3f(-width / 2.0f, +height, -depth / 2.0f);
	glVertex3f(-width / 2.0f, 0.0f, -depth / 2.0f);
	glVertex3f(-width / 2.0f, 0.0f, +depth / 2.0f);
	glEnd();

	// Bottom
	glNormal3f(0.0f, -1.0f, 0.0f);
	glBegin(GL_POLYGON);
	glVertex3f(-width / 2.0f, 0.0f, +depth / 2.0f);
	glVertex3f(-width / 2.0f, 0.0f, -depth / 2.0f);
	glVertex3f((+width / 2.0f), 0.0f, -depth / 2.0f);
	glVertex3f((+width / 2.0f), 0.0f, +depth / 2.0f);
	glEnd();

	// Windowframe Top
	glNormal3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_POLYGON);
	glVertex3f((-width / 2.0f) + windowFrameWidth, +height, +depth / 2.0f);
	glVertex3f((-width / 2.0f) + windowFrameWidth, +height, -depth / 2.0f);
	glVertex3f(-width / 2.0f, +height, -depth / 2.0f);
	glVertex3f(-width / 2.0f, +height, +depth / 2.0f);
	glEnd();

	// Windowframe Bottom
	glNormal3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_POLYGON);
	glVertex3f((+width / 2.0f), 0.0f, +depth / 2.0f);
	glVertex3f((+width / 2.0f), 0.0f, -depth / 2.0f);
	glVertex3f((+width / 2.0f), +windowFrameWidth, -depth / 2.0f);
	glVertex3f((+width / 2.0f), +windowFrameWidth, +depth / 2.0f);
	glEnd();

	// Side 1
	glNormal3f(0.0f, 0.0f, -1.0f);
	glBegin(GL_POLYGON);
	glVertex3f((-width / 2.0f) + windowFrameWidth, +height, -depth / 2.0f);
	glVertex3f(-width / 2.0f, +height, -depth / 2.0f);
	glVertex3f(-width / 2.0f, 0.0f, -depth / 2.0f);
	glVertex3f((+width / 2.0f), 0.0f, -depth / 2.0f);
	glVertex3f((+width / 2.0f), +windowFrameWidth, -depth / 2.0f);
	glEnd();

	// Side 2
	glNormal3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_POLYGON);
	glVertex3f((-width / 2.0f) + windowFrameWidth, +height, depth / 2.0f);
	glVertex3f(-width / 2.0f, +height, depth / 2.0f);
	glVertex3f(-width / 2.0f, 0.0f, depth / 2.0f);
	glVertex3f((+width / 2.0f), 0.0f, depth / 2.0f);
	glVertex3f((+width / 2.0f), +windowFrameWidth, depth / 2.0f);
	glEnd();

	// Window
	float nVectorX = windowFrameWidth - height * -1.0f;
	float nVectorY = (-width / 2.0f) + windowFrameWidth - (width / 2.0f) * -1.0f;
	glNormal3f(nVectorX, nVectorY, 0.0f);
	glBegin(GL_QUADS);
	{
		//glEnable(GL_BLEND);
		//glDepthMask(GL_FALSE);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//materialColor(0.2f, 0.2f, 0.2f, 0.3f);

		glVertex3f((-width / 2.0f) + windowFrameWidth, +height, +depth / 2.0f);
		glVertex3f((-width / 2.0f) + windowFrameWidth, +height, -depth / 2.0f);
		glVertex3f(+width / 2.0f, +windowFrameWidth, -depth / 2.0f);
		glVertex3f(+width / 2.0f, +windowFrameWidth, +depth / 2.0f);

		//revertMaterialColor();
		//glDepthMask(GL_TRUE);
		//glDisable(GL_BLEND);
	}
	glEnd();
}

/**
* Draws a house
*
* @param width The width of the house
* @param height The height of the house (including roof)
* @param r Red-channel color value of the house
* @param g Green-channel color value of the house
* @param b Blue-channel color value of the house
*/
void house(GLfloat width, GLfloat height, float r, float g, float b) {

	materialColor(r, g, b, 1.0f);
	cuboid(width, height-2.0f, width);
	revertMaterialColor();

	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, tex_roof);


	// Roof
	materialColor(1.0f, 1.0f, 1.0f, 1.0f);
	//materialColor(0.75f, 0.22f, 0.17f, 1.0f);
	glNormal3f(-2.0f, 1.0f, 0.0f);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.5, 0); glVertex3f(0.0f, height, 0.0f);
	glTexCoord2f(1, 1);  glVertex3f(-width / 2.0f, height-2.0f, width / 2.0f);
	glTexCoord2f(0, 1);  glVertex3f(-width / 2.0f, height-2.0f, -width / 2.0f);
	glEnd();

	glNormal3f(0.0f, 1.0f, 2.0f);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.5, 0); glVertex3f(0.0f, height, 0.0f);
	glTexCoord2f(1, 1);  glVertex3f(width / 2.0f, height - 2.0f, width / 2.0f);
	glTexCoord2f(0, 1);  glVertex3f(-width / 2.0f, height - 2.0f, width / 2.0f);
	glEnd();

	glNormal3f(2.0f, 1.0f, 0.0f);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.5, 0); glVertex3f(0.0f, height, 0.0f);
	glTexCoord2f(0, 1);  glVertex3f(width / 2.0f, height - 2.0f, width / 2.0f);
	glTexCoord2f(1, 1);  glVertex3f(width / 2.0f, height - 2.0f, -width / 2.0f);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	revertMaterialColor();
}