#pragma once
// OGL
#include <gl\glew.h> // for GLSL extensions IMPORTANT : This Line Should Be Before #include<gl\gl.h> And #include<gl\glu.h>
#include <gl/GL.h>
class RawModel
{
private:
	GLuint vaoID;
	int vertexCount;
	int faceCount;

public:
	RawModel(GLuint vaoID, int vertexCount);
	//~RawModel();
	GLuint getVaoID();
	int getVertexCount(void);
	int getFaceCount(void);
};

