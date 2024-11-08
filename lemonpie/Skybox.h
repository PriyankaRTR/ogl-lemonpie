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
class Skybox
{


public:

	Skybox();
	~Skybox();

	GLuint loadTextureData(char* filename);

	

	///direction coords


};

