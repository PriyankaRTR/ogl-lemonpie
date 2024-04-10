#pragma once
#include "RawModel.h"
#include "Texture.h"
class TexturedModel
{
private:
	RawModel* rawModel;
	Texture* texture;

public:
	TexturedModel(RawModel* rawModel, Texture* texture);
	RawModel* getRawModel(void);
	Texture* getTexture(void);
};

