#include "Loader.h"


RawModel* Loader::loadToVAO(GLfloat* vArray, GLfloat* nArray, GLfloat* tArray, int* indices, int fSize)
{
	GLuint vaoID = createVAO();
	bindIndicesBuffer(indices, fSize);
	storeDataInAttributeList(VDG_ATTRIBUTE_VERTEX, 3, vArray, fSize);
	storeDataInAttributeList(VDG_ATTRIBUTE_TEXTURE0, 2, tArray, fSize);
	storeDataInAttributeList(VDG_ATTRIBUTE_NORMAL, 3, nArray, fSize);
	unbindVAO();
	return (new RawModel(vaoID, fSize));
	//return vaoID;
}

GLuint Loader::loadToVAO(GLfloat* vArray, GLfloat* nArray, GLfloat* tArray, int fSize)
{
	GLuint vaoID = createVAO();
	storeDataInAttributeList(VDG_ATTRIBUTE_VERTEX, 3, vArray, fSize);
	storeDataInAttributeList(VDG_ATTRIBUTE_TEXTURE0, 2, tArray, fSize);
	storeDataInAttributeList(VDG_ATTRIBUTE_NORMAL, 3, nArray, fSize);
	unbindVAO();
	return vaoID;
}



GLuint Loader::createVAO(void)
{
	GLuint vaoID = 0;
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);
	return vaoID;
}

void Loader::unbindVAO(void)
{
	glBindVertexArray(0);
}

void Loader::bindIndicesBuffer(int* indices, int arraySize)
{
	GLuint vboID = 0;
	//int tempSize = arraySize; // newTerrain->getIndexSize();
	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (arraySize * sizeof(int)), indices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (arraySize * sizeof(int)), indices, GL_STATIC_DRAW);
}

void Loader::storeDataInAttributeList(GLuint attribNumber, GLuint coordinateSize, float* data, int arraySize)
{
	GLuint vboId = 0;
	//int tempSize = newTerrain->getFaceSize();
	glGenBuffers(1, &vboId);
	vbos.push_back(vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	if (coordinateSize == 2) // for texture coords
		glBufferData(GL_ARRAY_BUFFER, (arraySize * 2 * sizeof(GLfloat)), data, GL_STATIC_DRAW);
	else // for vertiex and normals
		glBufferData(GL_ARRAY_BUFFER, (arraySize * 3 * sizeof(GLfloat)), data, GL_STATIC_DRAW);
	glVertexAttribPointer(attribNumber, coordinateSize, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void Loader::cleanUp(void)
{
	for (GLuint vao : vaos)
	{
		glDeleteVertexArrays(1, &vao);
	}
	for (GLuint vbo : vbos)
	{
		glDeleteBuffers(1, &vbo);
	}
}