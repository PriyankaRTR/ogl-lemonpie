#include "Terrain.h"

RawModel* Terrain::generateTerrain(Loader& loader)
{
	const int VERTEX_COUNT = hmImageData->h;
	int count = VERTEX_COUNT * VERTEX_COUNT;
	vertices = (float*)malloc(count * 3 * 4);
	normals = (float*)malloc(count * 3 * 4);
	textureCoords = (float*)malloc(count * 2 * 4);
	int vertexPointer = 0;
	float temp = 0;
	temp = 6 * (VERTEX_COUNT - 1) * (VERTEX_COUNT - 1);
	indices = (int*)malloc((6 * (VERTEX_COUNT - 1) * (VERTEX_COUNT - 1)) * sizeof(int));

	for (int i = 0; i < VERTEX_COUNT; i++) { // z
		for (int j = 0; j < VERTEX_COUNT; j++) { // x
			vertices[vertexPointer * 3] = (float)j / ((float)VERTEX_COUNT - 1) * SIZE;
			vertices[vertexPointer * 3 + 1] = getHeight(j,i);
			vertices[vertexPointer * 3 + 2] = -((float)i / ((float)VERTEX_COUNT - 1) * SIZE);
			normals[vertexPointer * 3] = 0.0f;  
			normals[vertexPointer * 3 + 1] = 1.0f;
			normals[vertexPointer * 3 + 2] = 0.0f;
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

Terrain::Terrain(int gridX, int gridZ, Loader& loader, const char* heightMap)
{
	x = gridX * SIZE;
	z = gridZ * SIZE;
	vertices = NULL;
	normals = NULL;
	textureCoords = NULL;
	indices = NULL;
	fSize = 0;
	iSize = 0;
	
	texture = new Texture();
	hmImageData = texture->loadHeightMap_stb(heightMap);
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
	free(indices);
	indices = NULL;
	free(texture);
	texture = NULL;
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

float Terrain::getHeight(int x, int z)
{
	// w => x, h => z

	int bytesPerPixel = hmImageData->channels;
	int pixelDataOffset = bytesPerPixel * hmImageData->h * z + bytesPerPixel * x;
	unsigned char r = hmImageData->data[pixelDataOffset + 0];
	unsigned char g = hmImageData->data[pixelDataOffset + 1];
	unsigned char b = hmImageData->data[pixelDataOffset + 2];
	unsigned char a = hmImageData->data[pixelDataOffset + 3];

	float val = (0xff & b) | ((0xff & g) << 8) | ((0xff & r) << 16);

	val -= MAX_PIXEL_COLOR / 2.0;
	val /= MAX_PIXEL_COLOR / 2.0;

	val *= MAX_HEIGHT;
	
	return val;
}