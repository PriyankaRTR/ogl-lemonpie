#pragma once
#include <stdio.h> // for FILE I/O
#include <stdlib.h>
#include <vector>
#include"RawModel.h"
#include"Loader.h"

class Terrain
{
private:
	const float SIZE = 800;
	const int VERTEX_COUNT = 128;
	float* vertices;
	float* normals;
	float* textureCoords;
	int* indices;
	int fSize;
	int iSize;
	float x;
	float z;
	RawModel* generateTerrain(Loader& loader);
	RawModel* model;

public:
	Terrain(int gridX, int gridZ, Loader& loader);
	~Terrain();
	
	RawModel* getModel(void);

	float* getVertices(void);
	float* getNormals(void);
	float* getTextureCoords(void);
	int* getIndices(void);

	int getFaceSize(void);
	int getIndexSize(void);

	float getX(void);
	float getZ(void);

};

