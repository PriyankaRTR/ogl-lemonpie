#include "Texture.h"



Texture::Texture(char* RcId)
{
	imageResourceId = RcId;
}

Texture::~Texture()
{
}



GLuint Texture::LoadGLTextures()//(GLuint* texture, TCHAR imageResourceId[])
{
	GLuint textureId = 0;
	// variable declarations
	HBITMAP hBitmap;
	BITMAP bmp;
	int iStatus = FALSE;
	// code
	glGenTextures(1, &textureId);// 1 image
	hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), imageResourceId, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
	if (hBitmap)// if bitmap exists ( means hBitmap is not null )
	{
		iStatus = TRUE;
		GetObject(hBitmap, sizeof(bmp), &bmp);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);// set 1 rather than default 4, for better performance
		glBindTexture(GL_TEXTURE_2D, textureId);// bind texture
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexImage2D(GL_TEXTURE_2D,
			0,
			GL_RGB,
			bmp.bmWidth,
			bmp.bmHeight,
			0,
			GL_BGR,
			GL_UNSIGNED_BYTE,
			bmp.bmBits);

		// Create mipmaps for this texture for better image quality
		glGenerateMipmap(GL_TEXTURE_2D);

		DeleteObject(hBitmap);// delete unwanted bitmap handle
	}
	textures.push_back(textureId);
	return(textureId);
}

GLuint Texture::getTextureId(void)
{
	//return textureId;
	return 0;
}

void Texture::cleanUp(void)
{
	for (GLuint texture : textures)
	{
		glDeleteTextures(1, &texture);
	}
	textures.clear();
}

void Texture::bindTexture(int index)
{
	glActiveTexture(GL_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, textures[index]);
}