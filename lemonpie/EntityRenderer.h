#pragma once
#include "StaticShader.h"
#include "Terrain.h"
#include "Texture.h"
#include "TexturedModel.h"

class EntityRenderer
{
private:
	StaticShader* shader;
	void prepareTexturedModel(TexturedModel* model);
	void loadTransformationMatrix(void);
	void loadModelMatrix(TexturedModel* model);
	void unbindTextureModel(void);
	void loadViewMatrix(void);

public:
	EntityRenderer(StaticShader* shader, mat4 projectionMatrix);
	void render(TexturedModel* model);

};

