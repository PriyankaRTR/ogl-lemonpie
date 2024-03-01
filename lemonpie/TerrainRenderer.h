#pragma once
class TerrainRenderer
{

private:
	void prepareTerrain(void);
	void loadTransformationMatrix(void);
	void unbindTextureModel(void);
	void loadViewMatrix(void);

public:
	void render(void);
};

