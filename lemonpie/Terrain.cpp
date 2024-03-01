#include "Terrain.h"

RawModel* Terrain::generateTerrain(Loader& loader)
{
	int count = VERTEX_COUNT * VERTEX_COUNT;
	vertices = (float*)malloc(count * 3 * 4);
	normals = (float*)malloc(count * 3 * 4);
	textureCoords = (float*)malloc(count * 2 * 4);
	int vertexPointer = 0;
	float temp = 0;
	temp = 6 * (VERTEX_COUNT - 1) * (VERTEX_COUNT - 1);
	indices = (int*)malloc((6 * (VERTEX_COUNT - 1) * (VERTEX_COUNT - 1)) * sizeof(int));

	for (int i = 0; i < VERTEX_COUNT; i++) {
		for (int j = 0; j < VERTEX_COUNT; j++) {
			vertices[vertexPointer * 3] = (float)j / ((float)VERTEX_COUNT - 1) * SIZE;
			vertices[vertexPointer * 3 + 1] = 0;
			vertices[vertexPointer * 3 + 2] = (float)i / ((float)VERTEX_COUNT - 1) * SIZE;
			normals[vertexPointer * 3] = 0;  
			normals[vertexPointer * 3 + 1] = 1;
			normals[vertexPointer * 3 + 2] = 0;
			textureCoords[vertexPointer * 2] = (float)j / ((float)VERTEX_COUNT - 1);
			textureCoords[vertexPointer * 2 + 1] = (float)i / ((float)VERTEX_COUNT - 1);
			vertexPointer++;
		}
	}

	fSize = vertexPointer;

	int pointer = 0;
	for (int gz = 0; gz < VERTEX_COUNT - 1; gz++) {
		for (int gx = 0; gx < VERTEX_COUNT - 1; gx++) {
			int topLeft = (gz * VERTEX_COUNT) + gx;
			int topRight = topLeft + 1;
			int bottomLeft = ((gz + 1) * VERTEX_COUNT) + gx;
			int bottomRight = bottomLeft + 1;
			indices[pointer++] = topLeft;
			indices[pointer++] = bottomLeft;
			indices[pointer++] = topRight;
			indices[pointer++] = topRight;
			indices[pointer++] = bottomLeft;
			indices[pointer++] = bottomRight;
		}
	}

	iSize = pointer;

	return (loader.loadToVAO(vertices, textureCoords, normals, indices, fSize));
}

Terrain::Terrain(int gridX, int gridZ, Loader& loader)
{
	x = gridX * SIZE;
	z = gridZ * SIZE;
	vertices = NULL;
	normals = NULL;
	textureCoords = NULL;
	indices = NULL;
	fSize = 0;
	iSize = 0;
	model = generateTerrain(loader);
}

Terrain::~Terrain()
{
	free(vertices);
	vertices = NULL;
	free(normals);
	normals = NULL;
	free(textureCoords);
	textureCoords = NULL;
}

RawModel* Terrain::getModel(void)
{
	return model;
}

float* Terrain::getVertices(void)
{
	return vertices;
}

float* Terrain::getNormals(void)
{
	return normals;
}

float* Terrain::getTextureCoords(void)
{
	return textureCoords;
}

int* Terrain::getIndices(void)
{
	return indices;
}

int Terrain::getFaceSize(void)
{
	return fSize;
}

int Terrain::getIndexSize(void)
{
	return iSize;
}
float Terrain::getX() {
	return x;
}

float Terrain::getZ() {
	return z;
}