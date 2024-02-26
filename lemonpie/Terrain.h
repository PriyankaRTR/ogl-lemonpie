#pragma once
#include <stdio.h> // for FILE I/O
#include <stdlib.h>
#include <vector>

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
	void generateTerrain(void);

public:
	Terrain(int gridX, int gridZ);
	~Terrain();

	float* getVertices(void);
	float* getNormals(void);
	float* getTextureCoords(void);
	int* getIndices(void);

	int getFaceSize(void);
	int getIndexSize(void);

	float getX(void);
	float getZ(void);

};

