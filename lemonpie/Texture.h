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
#include "TextureData.h"


#define IDBITMAP_KUNDALI 100
#define IDBITMAP_STONE   101
#define IDBITMAP_GRASS   102
#define IDBITMAP_TREE	 103


class Texture
{
public:
	Texture();
	Texture(char* RcId);
	Texture(const char* filename[6]);
	Texture(const char* filename);
	~Texture();
	GLuint getTextureId(void);
	GLuint LoadGLTextures(void); 	// (GLuint* texture, TCHAR imageResourceId[]);
	GLuint LoadGLTextures_stb(const char* filename);
	GLuint loadCubMap_stb(const char** textureFiles);
	void cleanUp(void);
	void bindTexture(void);

	typedef struct
	{
		int w;
		int h;
		int channels;
		unsigned char* data;

	}heightMapData;


	heightMapData* loadHeightMap_stb(const char* heightMapFile);

	heightMapData hmData;

private:
	char* imageResourceId;
	char* filename[6];
	GLuint textureId;
};  