#pragma once

#include <windows.h>
#include "gl\glew.h"
#include "gl/GL.h"

#define IDBITMAP_KUNDALI 100
#define IDBITMAP_STONE   101
#define IDBITMAP_GRASS   102


class Texture
{
public:
	Texture(GLuint id, char* RcId);
	~Texture();
	GLuint getTextureId(void);
	int LoadGLTextures(void); 	// (GLuint* texture, TCHAR imageResourceId[]);

private:
	GLuint textureId;
	char* imageResourceId;
	
};  