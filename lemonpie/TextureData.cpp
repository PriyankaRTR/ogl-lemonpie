#include "TextureData.h"

TextureData::TextureData(unsigned char* buffer, int width, int height)
{
	this->buffer = buffer;
	this->width = width;
	this->height = height;
}

int TextureData::getWidth()
{
	return width;
}

int TextureData::getHeight()
{
	return height;
}

unsigned char* TextureData::getBuffer()
{
	return buffer;
}
