#pragma once
#include "Terrain.h"
#include "RawModel.h"
#include "TerrainShader.h"
#include "Texture.h"

class TerrainRenderer
{

private:
	TerrainShader shader;
	void prepareTerrain(Terrain terrain);
	void loadTransformationMatrix(void);
	void unbindTextureModel(void);
	void loadViewMatrix(void);

public:
	TerrainRenderer(TerrainShader shader, mat4 projectionMatrix);
	void render(void);
};

