#pragma once
// c
#include <stdio.h> // for FILE I/O
#include <stdlib.h>
// OGL
#include <gl\glew.h> // for GLSL extensions IMPORTANT : This Line Should Be Before #include<gl\gl.h> And #include<gl\glu.h>
#include <gl/GL.h>
// c++
#include<vector>

#include"RawModel.h"
#include"Loader.h"

#define NR_POINT_COORDS		3 // number of point coordinates
#define NR_TEXTURE_COORDS	2 // number of texture coords
#define NR_NORMAL_COORDS	3 // same for notrmal coords
#define NR_FACE_TOKENS		3 // minimum number of entries in face data
#define BUFFER_SIZE 256
#define S_EQUAL			0
#define PI 3.1415926535898

class OBJLoader
{
private:

	char line[BUFFER_SIZE];
	//for obj loading
	std::vector<std::vector<float>> g_vertices;
	std::vector<std::vector<float>> g_texture;
	std::vector<std::vector<float>> g_normals;
	std::vector<std::vector<int>> objIndices;
	std::vector<std::vector<int>> g_face_tri, g_face_texture, g_face_normal;
	unsigned long long int fSize;
	GLfloat* vertexArray;
	GLfloat* normalsArray;
	GLfloat* textureArray;
	GLfloat* indicess;
	void processVertexData(void);
	void processNormalsData(void);
	void processTextureData(void);
	//float* vertices;
	//float* normals;
	//float* textureCoords;
	//int* indices;


public:
	GLuint loadObjModel(const char* fileName, Loader& loader);
	//float* getVertices(void);
	//float* getNormals(void);
	//float* getTextureCoords(void);
	//int getFaceSize(void);

};

