#pragma once
// c
#include <stdio.h> // for FILE I/O
#include <stdlib.h>
// OGL
#include <gl\glew.h> // for GLSL extensions IMPORTANT : This Line Should Be Before #include<gl\gl.h> And #include<gl\glu.h>
#include <gl/GL.h>
// c++
#include<vector>
#include<fstream>
#include<string>

#include"RawModel.h"

class Loader
{
private:
	std::vector<GLuint> vaos;
	std::vector<GLuint> vbos;
	GLuint createVAO(void);
	void bindIndicesBuffer(int* indices, int arraySize);
	void storeDataInAttributeList(GLuint attribNumber, GLuint coordinateSize, float* data, int arraySize);
	void unbindVAO(void);

	enum
	{
		VDG_ATTRIBUTE_VERTEX = 0,
		VDG_ATTRIBUTE_TEXTURE0,
		VDG_ATTRIBUTE_NORMAL,
		VDG_ATTRIBUTE_COLOR
	};

public:
	// to load model with indices
	RawModel* loadToVAO(GLfloat* vArray, GLfloat* nArray, GLfloat* tArray, int* indices, int fSize);
	// to load model with all data processed
	GLuint loadToVAO(GLfloat* vArray, GLfloat* nArray, GLfloat* tArray, int fSize);
	void cleanUp(void);
};

