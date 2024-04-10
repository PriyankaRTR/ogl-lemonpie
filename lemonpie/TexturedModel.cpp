#include "TexturedModel.h"

TexturedModel::TexturedModel(RawModel* rawModel, Texture* texture)
{
	this->rawModel = rawModel;
	this->texture = texture;
}

RawModel* TexturedModel::getRawModel(void)
{
	return rawModel;
}

Texture* TexturedModel::getTexture(void)
{
	return texture;
}
