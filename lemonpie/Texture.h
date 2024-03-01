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

#define IDBITMAP_KUNDALI 100
#define IDBITMAP_STONE   101
#define IDBITMAP_GRASS   102


class Texture
{
public:
	Texture(char* RcId);
	~Texture();
	GLuint getTextureId(void);
	GLuint LoadGLTextures(void); 	// (GLuint* texture, TCHAR imageResourceId[]);
	void cleanUp(void);
	void bindTexture(int index);

private:
	char* imageResourceId;
	std::vector<GLuint> textures;
};  