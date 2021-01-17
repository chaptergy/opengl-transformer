#include <iostream> 
#include <GL/freeglut.h>
#include "shapes.h"
#include "utils.h"
#include "textureVars.h"

#define PI 3.1415927f

static float animSpeedFactor = 1.0f;  /** speed up or slow down the animation */

/** the current status of the transformer */
enum transformerStatus
{
	driving,
	drivingTransition,
	transforming0,
	transforming1,
	transforming2,
	transforming3,
	walkingTransition,
	walking
};

bool reverseAnim = false; /** reverse animation (used when transforming from walking to driving) */
transformerStatus currentTransformerStatus = driving;
bool oppositeLegArmMovement = false; /** whether arms and legs move in the same direction (false) or in opposite directions (true) */

float walkAnimCounter = 0.0f;
int transformFrameCounter = 0;

float legRotation = -90.0f;
float upperArmRotation = 0.0f;
float leftLowerArmRotation = 0.0f;
float rightLowerArmRotation = 0.0f;
float wheelRotation = 180.0f;

float skyTextureOffset = 0.0f;
float streetTextureOffset = 0.0f;



float backWheelRotation = 0.0f;
float backWheelPosition[] = { 0.5f, -2.6f, 0.0f };

float frontWheelPosition[] = { 0.5f, -0.8f, 0.0f };

float bodyRotation = -90.0f;
float bodyPosition[] = { -2.0f, 2.25f, 0.0f };

float cockpitPosition[] = { 0.751f, 3.75f, 0.0f };

float upperArmPosition[] = { 1.25f, 4.5f, 0.499f };
float lowerArmHeight = -1.4f;

float legPosition[] = { 1.25f, 3.0f, 0.5f };


float housePositions[]{ -16.0f, -12.5f, -9.2f, -6.3f, -3.4f, -0.0, 2.8f, 5.6f, 8.9f };



/**
* Initializes OpenGL and loads Textures 
*/
void Init()
{
	sunlight();
	loadAllTextureFiles();

	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);
}

/**
* Draws the scene each frame
*/
void RenderScene()
{

	glLoadIdentity();

	glClearColor(0.333f, 0.662f, 0.968f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gluLookAt(3.0f, 6.0f, 12.0f, 0.8f, 4.0f, 0.0f, 0.0f, 1.0f, 0.0f); // OpenGL corrects the Up-Vector

	// Sky
	materialColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex_sky);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0.0f + skyTextureOffset, 0.0f); glVertex3f(-18.0f, 14.0f, -13.0f);
		glTexCoord2f(0.0f + skyTextureOffset, 1.0f); glVertex3f(-18.0f, -1.0f, -13.0f);
		glTexCoord2f(1.0f + skyTextureOffset, 1.0f); glVertex3f(14.0f, -1.0f, -13.0f);
		glTexCoord2f(1.0f + skyTextureOffset, 0.0f); glVertex3f(14.0f, 14.0f, -13.0f);
	}
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);


	// Ground / Street
	glNormal3f(0.0f, 1.0f, 0.0f);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex_road);

	glBegin(GL_QUADS);
	{
		glTexCoord2f(0.0f + streetTextureOffset, -1.05f); glVertex3f(-12.0f, 0.0f, -12.0f);
		glTexCoord2f(4.0f + streetTextureOffset, -1.05f); glVertex3f(8.0f, 0.0f, -12.0f);
		glTexCoord2f(4.0f + streetTextureOffset, 1.42f); glVertex3f(8.0f, 0.0f, 4.0f);
		glTexCoord2f(0.0f + streetTextureOffset, 1.42f); glVertex3f(-12.0f, 0.0f, 4.0f);
	}
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	materialColor(1.0f, 1.0f, 1.0f, 1.0f);

	// Body
	glPushMatrix();
	glTranslatef(bodyPosition[0], bodyPosition[1], bodyPosition[2]);
	glRotatef(bodyRotation, 0.0f, 0.0f, 1.0f);
	cuboid(1.5f, 3.0f, 2.0f);

	// Legs
	{
		// left
		glPushMatrix();
		glRotatef(-bodyRotation, 0.0f, 0.0f, 1.0f);
		glRotatef(legRotation, 0.0f, 0.0f, 1.0f);

		glTranslatef(legPosition[0], legPosition[1], -legPosition[2]);
		cuboid(1.0f, -3.0f, 1.0f);

		// Wheel
		glTranslatef(backWheelPosition[0], backWheelPosition[1], backWheelPosition[2]);
		glRotated(backWheelRotation, 1.0f, 0.0f, 0.0f);
		glRotatef(wheelRotation, 0.0f, 0.0f, 1.0f);
		wheel(0.4f, 0.25f);

		glPopMatrix();
	}
	{
		// right
		glPushMatrix();
		glRotatef(-bodyRotation, 0.0f, 0.0f, 1.0f);
		glRotatef(oppositeLegArmMovement ? -legRotation : legRotation, 0.0f, 0.0f, 1.0f);

		glTranslatef(legPosition[0], legPosition[1], legPosition[2]);
		cuboid(1.0f, -3.0f, 1.0f);

		// Wheel
		glTranslatef(backWheelPosition[0], backWheelPosition[1], backWheelPosition[2]);
		glRotated(backWheelRotation, 1.0f, 0.0f, 0.0f);
		glRotatef(wheelRotation, 0.0f, 0.0f, 1.0f);
		wheel(0.4f, 0.25f);

		glPopMatrix();
	}

	// Arms
	{
		// left upper arm
		glPushMatrix();
		glTranslatef(upperArmPosition[0], upperArmPosition[1], -upperArmPosition[2]);
		glRotatef(oppositeLegArmMovement ? -upperArmRotation : upperArmRotation, 0.0f, 0.0f, 1.0f);
		cuboid(1.0f, -1.4f, 1.0f);

		// Wheel
		glPushMatrix();
		glTranslatef(frontWheelPosition[0], frontWheelPosition[1], frontWheelPosition[2]);
		glRotatef(wheelRotation, 0.0f, 0.0f, 1.0f);
		wheel(0.4f, 0.25f);
		glPopMatrix();

		// left lower arm
		glTranslatef(0.0f, lowerArmHeight, 0.0f);
		glRotatef(leftLowerArmRotation, 0.0f, 0.0f, 1.0f);
		cuboid(1.0f, -1.4f, 1.0f);

		glPopMatrix();
	}
	{
		// right upper arm
		glPushMatrix();
		glTranslatef(upperArmPosition[0], upperArmPosition[1], upperArmPosition[2]);
		glRotatef(upperArmRotation, 0.0f, 0.0f, 1.0f);
		cuboid(1.0f, -1.4f, 1.0f);

		// Wheel
		glPushMatrix();
		glTranslatef(frontWheelPosition[0], frontWheelPosition[1], frontWheelPosition[2]);
		glRotatef(wheelRotation, 0.0f, 0.0f, 1.0f);
		wheel(0.4f, 0.25f);
		glPopMatrix();

		// right lower arm
		glTranslatef(0.0f, lowerArmHeight, 0.0f);
		glRotatef(rightLowerArmRotation, 0.0f, 0.0f, 1.0f);
		cuboid(1.0f, -1.4f, 1.0f);

		glPopMatrix();
	}

	// Cockpit
	glPushMatrix();
	glTranslatef(cockpitPosition[0], cockpitPosition[1], cockpitPosition[2]);
	glRotatef(-bodyRotation, 0.0f, 0.0f, 1.0f);
	cockpit(1.5f, 1.5f, 2.0f, 0.3f);
	glPopMatrix();


	glPopMatrix();

	// Houses
	{
		glPushMatrix();
		glTranslatef(housePositions[0], 0.0f, -9.0f);
		house(3.0f, 4.8f, 0.988f, 0.976f, 0.890f);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(housePositions[1], 0.0f, -9.0f);
		house(3.4f, 5.3f, 0.647f, 0.850f, 0.933f);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(housePositions[2], 0.0f, -9.0f);
		house(2.4f, 4.2f, 0.792f, 0.956f, 0.525f);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(housePositions[3], 0.0f, -9.0f);
		house(2.7f, 5.8f, 0.992f, 0.890f, 0.945f);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(housePositions[4], 0.0f, -9.0f);
		house(3.2f, 4.9f, 0.309f, 0.321f, 0.933f);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(housePositions[5], 0.0f, -9.0f);
		house(2.9f, 4.2f, 0.956f, 0.941f, 0.945f);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(housePositions[6], 0.0f, -9.0f);
		house(2.1f, 4.9f, 0.913f, 0.898f, 0.525f);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(housePositions[7], 0.0f, -9.0f);
		house(3.1f, 4.5f, 0.619f, 0.419f, 0.658f);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(housePositions[8], 0.0f, -9.0f);
		house(2.8f, 4.1f, 0.615f, 0.784f, 0.615f);
		glPopMatrix();
	}

	glutSwapBuffers();
}

/**
* React to window resize
*/
void Reshape(int width, int height)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, width, height);
	gluPerspective(40.0f, 1.0f, 0.1, 100.0f);
	glMatrixMode(GL_MODELVIEW);
}

/**
* Calculate object positions and rotations for animation.
* It is called every 1000ms
*/
void Animate(int value)
{
	glutPostRedisplay();
	int wait_msec = 10;
	glutTimerFunc(wait_msec, Animate, ++value);

	if (currentTransformerStatus == driving || currentTransformerStatus == drivingTransition) {
		float transitionMultiplier = 1.0f; // for a smooth transition between statuses

		if (currentTransformerStatus == drivingTransition) {
			// for a smooth transition between statuses
			transitionMultiplier = transformFrameCounter / (50 / animSpeedFactor);
			if (reverseAnim) {
				if (transitionMultiplier >= 0.99) {
					currentTransformerStatus = driving;
					reverseAnim = false;
					transformFrameCounter = 0;
				}
			}
			else {
				transitionMultiplier = 1 - transitionMultiplier;
				if (transitionMultiplier <= 0.01) {
					wheelRotation = walkAnimCounter = 0.0f;
					bodyPosition[1] = 2.25f;
					backWheelPosition[0] = frontWheelPosition[0] = 0.5f;
					currentTransformerStatus = transforming0;
					transformFrameCounter = 0;
				}
			}
			transformFrameCounter++;
		}

		wheelRotation = walkAnimCounter / PI * -360.0f * transitionMultiplier;
		bodyPosition[1] = 2.25f + sin(walkAnimCounter) * 0.1f * transitionMultiplier;
		backWheelPosition[0] = frontWheelPosition[0] = 0.5f + sin(walkAnimCounter) * 0.1f * transitionMultiplier;

		walkAnimCounter = walkAnimCounter + 0.05f * animSpeedFactor;
		if (walkAnimCounter > 2 * PI) {
			walkAnimCounter -= 2 * PI;
		}

		for (int i = 0; i < sizeof(housePositions) / sizeof(housePositions[0]); i++) {
			housePositions[i] -= 0.1f * transitionMultiplier * animSpeedFactor;
			if (housePositions[i] < -16.0f) {
				housePositions[i] += 28.0f;
			}
		}
		skyTextureOffset += 0.0001f * transitionMultiplier * animSpeedFactor;
		streetTextureOffset += 0.02f * transitionMultiplier * animSpeedFactor;
	}
	else if (currentTransformerStatus == transforming0) {
		float t = easeInOut(transformFrameCounter / (160 / animSpeedFactor));
		if (reverseAnim) {
			if (t >= 0.99) {
				upperArmPosition[1] = 4.5f;
				legPosition[1] = 3.0f;

				frontWheelPosition[1] = -0.8f;

				currentTransformerStatus = drivingTransition;
				transformFrameCounter = 0;
			}
			else {
				upperArmPosition[1] = (1 - t)* 5.25f + t * 4.5f;
				legPosition[1] = (1 - t)* 2.5f + t * 3.0f;

				frontWheelPosition[1] = (1 - t)* -2.8f + t * -0.8f;
			}
		}
		else {
			if (t >= 0.99) {
				upperArmPosition[1] = 5.25f;
				legPosition[1] = 2.5f;

				frontWheelPosition[1] = -2.8f;

				currentTransformerStatus = transforming1;
				transformFrameCounter = 0;
			}
			else {
				upperArmPosition[1] = (1 - t)* 4.5f + t * 5.25f;
				legPosition[1] = (1 - t)* 3.0f + t * 2.5f;

				frontWheelPosition[1] = (1 - t)* -0.8f + t * -2.8f;
			}
		}

		transformFrameCounter++;
	}
	else if (currentTransformerStatus == transforming1) {
		float t = easeInOut(transformFrameCounter / (160 / animSpeedFactor));
		if (reverseAnim) {
			if (t >= 0.99) {
				bodyPosition[0] = -2.0f;
				bodyPosition[1] = 2.25f;

				upperArmPosition[0] = 1.25f;
				upperArmPosition[1] = 5.25f;
				upperArmRotation = 0.0f;

				legPosition[0] = 1.25f;
				legPosition[1] = 2.5f;
				legRotation = -90.0f;

				currentTransformerStatus = transforming0;
				transformFrameCounter = 0;
			}
			else {
				bodyPosition[0] = (1 - t)* 0.0f + t * -2.0f;
				bodyPosition[1] = (1 - t)* 3.0f + t * 2.25f;

				upperArmPosition[0] = (1 - t)* 0.0f + t * 1.25f;
				upperArmPosition[1] = (1 - t)* 3.0f + t * 5.25f;
				upperArmRotation = (1 - t)* 80.0f + t * 0.0f;

				legPosition[0] = (1 - t)* 0.0f + t * 1.25f;
				legPosition[1] = (1 - t)* 0.0f + t * 2.5f;
				legRotation = (1 - t)* 0.0f + t * -90.0f;
			}
		}
		else {
			if (t >= 0.99) {
				bodyPosition[0] = 0.0f;
				bodyPosition[1] = 3.0f;

				upperArmPosition[0] = 0.0f;
				upperArmPosition[1] = 3.0f;
				upperArmRotation = 80.0f;

				legPosition[0] = 0.0f;
				legPosition[1] = 0.0f;
				legRotation = 0.0f;

				currentTransformerStatus = transforming2;
				transformFrameCounter = 0;
			}
			else {
				bodyPosition[0] = (1 - t)* -2.0f + t * 0.0f;
				bodyPosition[1] = (1 - t)* 2.25f + t * 3.0f;

				upperArmPosition[0] = (1 - t)* 1.25f + t * 0.0f;
				upperArmPosition[1] = (1 - t)* 5.25f + t * 3.0f;
				upperArmRotation = (1 - t)* 0.0f + t * 80.0f;

				legPosition[0] = (1 - t)* 1.25f + t * 0.0f;
				legPosition[1] = (1 - t)* 2.5f + t * 0.0f;
				legRotation = (1 - t)* -90.0f + t * 0.0f;
			}
		}

		transformFrameCounter++;
	}
	else if (currentTransformerStatus == transforming2) {
		oppositeLegArmMovement = false;
		float t = easeInOut(transformFrameCounter / (160 / animSpeedFactor));
		if (reverseAnim) {
			if (t >= 0.99) {
				bodyRotation = -90.0f;
				cockpitPosition[0] = 0.75f;
				cockpitPosition[1] = 3.75f;

				upperArmPosition[2] = 0.499f;
				upperArmRotation = 80.0f;

				currentTransformerStatus = transforming1;
				transformFrameCounter = 0;
			}
			else {
				bodyRotation = (1 - t)* 0.0f + t * -90.0f;
				cockpitPosition[0] = (1 - t)* 0.0f + t * 0.75f;
				cockpitPosition[1] = (1 - t)* 3.001f + t * 3.75f;

				upperArmPosition[2] = (1 - t)* 1.5f + t * 0.499f;
				upperArmRotation = (1 - t)* 0.0f + t * 80.0f;
			}
		}
		else {
			if (t >= 0.99) {
				bodyRotation = 0.0f;
				cockpitPosition[0] = 0.0f;
				cockpitPosition[1] = 3.001f;

				upperArmPosition[2] = 1.5f;
				upperArmRotation = 0.0f;

				currentTransformerStatus = transforming3;
				transformFrameCounter = 0;
			}
			else {
				bodyRotation = (1 - t)* -90.0f + t * 0.0f;
				cockpitPosition[0] = (1 - t)* 0.75f + t * 0.0f;
				cockpitPosition[1] = (1 - t)* 3.75f + t * 3.001f;

				upperArmPosition[2] = (1 - t)* 0.499f + t * 1.5f;
				upperArmRotation = (1 - t)* 80.0f + t * 0.0f;
			}
		}

		transformFrameCounter++;
	}
	else if (currentTransformerStatus == transforming3) {
		float t = easeInOut(transformFrameCounter / (160 / animSpeedFactor));
		if (reverseAnim) {
			if (t >= 0.99) {
				backWheelRotation = 0.0f;
				backWheelPosition[1] = -2.6f;

				lowerArmHeight = -1.4f;
				frontWheelPosition[0] = 0.5f;
				frontWheelPosition[1] = -2.8f;

				currentTransformerStatus = transforming2;
				transformFrameCounter = 0;
			}
			else {
				backWheelRotation = 90 * (1 - t);
				backWheelPosition[1] = (1 - t)*-2.75f + t * -2.6f;

				lowerArmHeight = (1 - t)*-1.6f + t * -1.4f;
				frontWheelPosition[0] = (1 - t)* 0.0f + t * 0.5f;
				frontWheelPosition[1] = (1 - t)*-1.5f + t * -2.8f;
			}
		}
		else {
			if (t >= 0.99) {
				backWheelRotation = 90.0f;
				backWheelPosition[1] = -2.75f;

				lowerArmHeight = -1.6f;
				frontWheelPosition[0] = 0.0f;
				frontWheelPosition[1] = -1.5f;

				currentTransformerStatus = walkingTransition;
				transformFrameCounter = 0;
			}
			else {
				backWheelRotation = 90 * (t);
				backWheelPosition[1] = (1 - t)*-2.6f + t * -2.75f;

				lowerArmHeight = (1 - t)*-1.4f + t * -1.6f;
				frontWheelPosition[0] = (1 - t)*0.5f + t * 0.0f;
				frontWheelPosition[1] = (1 - t)*-2.8f + t * -1.5f;
			}
		}

		transformFrameCounter++;
	}
	else if (currentTransformerStatus == walking || currentTransformerStatus == walkingTransition) {
		oppositeLegArmMovement = true;
		float transitionMultiplier = 1.0f; // for a smooth transition between statuses

		if (currentTransformerStatus == walkingTransition) {
			// for a smooth transition between statuses
			transitionMultiplier = transformFrameCounter / (50 / animSpeedFactor);
			if (reverseAnim) {
				transitionMultiplier = 1 - transitionMultiplier;
				if (transitionMultiplier <= 0.01) {
					walkAnimCounter = legRotation = upperArmRotation = leftLowerArmRotation = rightLowerArmRotation = 0.0f;
					bodyPosition[1] = 3.0f;
					currentTransformerStatus = transforming3;
					transformFrameCounter = 0;
				}
			}
			else {
				if(transitionMultiplier >= 0.99) {
					currentTransformerStatus = walking;
					reverseAnim = true;
					transformFrameCounter = 0;
				}
			}
			transformFrameCounter++;
		}

		bodyPosition[1] = 3.0f + sin(2*walkAnimCounter-0.2f) * 0.05f * transitionMultiplier;
		legRotation = sin(walkAnimCounter) * 45.0f * transitionMultiplier;
		upperArmRotation = sin(walkAnimCounter) * 40.0f * transitionMultiplier;
		rightLowerArmRotation = (sin(walkAnimCounter) * 40.0f) + 40.0f;
		leftLowerArmRotation = (80.0f - rightLowerArmRotation) * transitionMultiplier;
		rightLowerArmRotation *= transitionMultiplier;

		walkAnimCounter = walkAnimCounter + 0.05f * animSpeedFactor;
		if (walkAnimCounter > 2 * PI) {
			walkAnimCounter -= 2 * PI;
		}

		for (int i = 0; i < sizeof(housePositions) / sizeof(housePositions[0]); i++) {
			housePositions[i] -= 0.1f * transitionMultiplier * animSpeedFactor;
			if (housePositions[i] < -16.0f) {
				housePositions[i] += 28.0f;
			}
		}
		skyTextureOffset += 0.0001f * transitionMultiplier * animSpeedFactor;
		streetTextureOffset += 0.02f * transitionMultiplier * animSpeedFactor;
	}

	skyTextureOffset += 0.00001f * animSpeedFactor;
	if (skyTextureOffset > 1.0f) skyTextureOffset -= 1.0f;
	if (streetTextureOffset > 1.0f) streetTextureOffset -= 1.0f;

}


/**
* handle key presses (and transform on spacebar press)
*/
void keyPressed(unsigned char key, int x, int y) {
	if (key == ' ') {
		if (currentTransformerStatus == driving) {
			currentTransformerStatus = drivingTransition;
		}
		else if (currentTransformerStatus == walking) {
			currentTransformerStatus = walkingTransition;
		}
	}
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv); // initialize GLUT
	glutSetOption(GLUT_MULTISAMPLE, 8);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Transformer");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(keyPressed);
	glutTimerFunc(10, Animate, 0);
	Init();

	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "Press space to toggle the transformer" << std::endl;

	glutMainLoop();
	return 0;
}