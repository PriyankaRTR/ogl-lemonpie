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


	//static void processVertexData(void);
	//static void processNormalsData(void);
	static void processTextureData(void);
	static float* processVertexData(std::vector<std::vector<float>>& g_vertices, std::vector<std::vector<int>>& g_face_tri);
	static float* processNormalsData(std::vector<std::vector<float>>& g_normals, std::vector<std::vector<int>>& g_face_normal);
	static float* processTextureData(std::vector<std::vector<float>>& g_texture, std::vector<std::vector<int>>& g_face_texture);

	//float* vertices;
	//float* normals;
	//float* textureCoords;
	//int* indices;


public:
	static RawModel* loadObjModel(const char* fileName, Loader& loader);
};

