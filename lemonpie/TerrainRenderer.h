#pragma once
#include "Terrain.h"
#include "RawModel.h"
#include "TerrainShader.h"
#include "Texture.h"
#include "vmath.h"
#include <list>

class TerrainRenderer
{

private:
	TerrainShader* shader;
	void prepareTerrain(Terrain* terrain);
	void loadTransformationMatrix(void);
	void loadModelMatrix(Terrain* terrain);
	void unbindTextureModel(void);
	void loadViewMatrix(void);

public:
	TerrainRenderer(TerrainShader* shader, mat4 projectionMatrix);
	void render(std::list<Terrain*> terrains, Texture* texture);
};

