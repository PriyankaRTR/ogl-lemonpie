#pragma once
#include<Windows.h>
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
// stb
#include "stb_image.h"


#define IDBITMAP_KUNDALI 100
#define IDBITMAP_STONE   101
#define IDBITMAP_GRASS   102
#define IDBITMAP_TREE	 103


class Texture
{
public:
	Texture();
	Texture(char* RcId);
	~Texture();
	GLuint getTextureId(void);
	GLuint LoadGLTextures(void); 	// (GLuint* texture, TCHAR imageResourceId[]);
	GLuint LoadGLTextures_stb(const char* filename);
	void cleanUp(void);
	void bindTexture(void);

private:
	char* imageResourceId;
	GLuint textureId;
};  