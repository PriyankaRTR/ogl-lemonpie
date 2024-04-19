#pragma once
#include "vmath.h"
#include "StaticShader.h"
#include "EntityRenderer.h"
#include "TerrainShader.h"
#include "TerrainRenderer.h"
#include <map>
#include "Entity.h"
#include <list>
#include "Display.h"
class MasterRenderer
{

private:
	const float FOV = 70.0f;
	const float NEAR_PLANE = 0.1f;
	const float FAR_PLANE = 1000.0f;

	vmath::mat4 projectionMatrix;

	StaticShader* shader;
	EntityRenderer* renderer;

	//TerrainShader* terrainShader;
	//TerrainRenderer* terrainRenderer;

	std::map<TexturedModel*, std::list<Entity>> entities;

	//int width;
	//int height;
	//std::list<Terrain>* terrains;

public:

	MasterRenderer();
	~MasterRenderer();
		
	//void processTerrain(Terrain terrain);
	void processEntity(Entity entity);
	void prepare(void);
	void render(Light sun, CameraControl* camera);
	void createProjectionMatrix(void);
	void cleanup(void);
	//void updateDisplayDimensions(int, int);


};

