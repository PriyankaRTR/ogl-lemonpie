#pragma once


class TextureData
{

private:
	int width;
	int height;
	unsigned char* buffer;


public:
	TextureData(unsigned char* buffer, int width, int height);
	int getWidth();
	int getHeight();
	unsigned char* getBuffer();

};

