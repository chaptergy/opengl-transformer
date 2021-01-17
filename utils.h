#ifndef UTILS_HEADER_FILE_H
#define UTILS_HEADER_FILE_H

void sunlight();
void materialColor(float r, float g, float b, float a);
void revertMaterialColor();
float easeInOut(float t);

float customCos(int step);
float customSin(int step);

GLuint loadTexture(const char * bildpfad, GLint bildModus, GLint wrap_s, GLint wrap_t);
void loadAllTextureFiles();

static float previousMaterialColor[4] = { -1, -1, -1, -1 };
static float currentMaterialColor[4] = { -1, -1, -1, -1 };

static float sincos[] = { 1.0f, 0.95106f, 0.80902f, 0.58778f, 0.30901f, 0.0f };
#endif