#include "Texture.h"




Texture::Texture(const char* filename[6])
{
	this->imageResourceId = 0;
	this->textureId = 0;
	textureId = loadCubMap_stb(filename);
}

Texture::Texture(char* RcId)
{
	imageResourceId = RcId;
	textureId = 0;
}

Texture::~Texture()
{
}



GLuint Texture::LoadGLTextures()//(GLuint* texture, TCHAR imageResourceId[])
{
	//GLuint textureId = 0;
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
	//textures.push_back(textureId);
	return(true);
}

GLuint Texture::LoadGLTextures_stb(const char *filename)
{

	GLuint textureID;
	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels;
	unsigned char* textureData = stbi_load(filename, &width, &height, &nrChannels, 0);

	if (textureData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	stbi_image_free(textureData);
	textureId = textureID;
	return textureID;
}


GLuint Texture::loadCubMap_stb(const char** textureFiles)
{
	GLuint texID;
	glGenTextures(1, &texID);
	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texID);

	//stbi_set_flip_vertically_on_load(true);

	for (int i = 0; i < 6; i++)
	{
		const char* name = textureFiles[i];
		int width, height, nrChannels;
		unsigned char* buffer = stbi_load(textureFiles[i], &width, &height, &nrChannels, 0);

		TextureData textureData(buffer, width, height);

		if (buffer)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, textureData.getWidth(), textureData.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, textureData.getBuffer());
			// for some images the setting has to be GL_RGBA for appropriate texturing
			// not all .png files have 4 channels, some have 3 channels only.
			// In this case, for uniformity, I have converted all 6 files to .jpg and then it worked

			stbi_image_free(buffer);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


	return texID;
}



GLuint Texture::getTextureId(void)
{
	return textureId;
	//return 0;
}

void Texture::cleanUp(void)
{
	//for (GLuint texture : textures)
	{
		glDeleteTextures(1, &textureId);
	}
	//textures.clear();
}

void Texture::bindTexture(void)
{
	glActiveTexture(GL_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, textureId);
}