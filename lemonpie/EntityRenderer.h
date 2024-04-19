#pragma once
#include "StaticShader.h"
#include "Terrain.h"
#include "Texture.h"
#include "TexturedModel.h"
#include <map>
#include <list>
#include "Entity.h"

class EntityRenderer
{
private:
	StaticShader* shader;
	void prepareTexturedModel(TexturedModel* model);
	void prepareInstance(Entity entity);
	void loadTransformationMatrix(void);
	void loadModelMatrix(TexturedModel* model);
	void unbindTextureModel(void);
	void loadViewMatrix(void);

public:
	EntityRenderer(StaticShader* shader, mat4 projectionMatrix);
	~EntityRenderer();
	void render(std::map<TexturedModel*, std::list<Entity>> &entities);

};

