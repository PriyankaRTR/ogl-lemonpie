#include "MasterRenderer.h"

MasterRenderer::MasterRenderer()
{
	createProjectionMatrix();
	shader = new StaticShader("Shaders/staticVertShader.vs", "Shaders/staticFragShader.fs");
	renderer = new EntityRenderer(shader, projectionMatrix);

	terrainShader = new TerrainShader("Shaders/terrain.vs", "Shaders/terrain.fs");
	terrainRenderer = new TerrainRenderer(terrainShader, projectionMatrix);

	terrainTexture = new Texture(MAKEINTRESOURCE(IDBITMAP_GRASS));
	if (terrainTexture->LoadGLTextures())
	{
		//texture loaded successfully
	}

	////entities = new std::map<TexturedModel*, std::list<Entity>>;
	//terrains = new std::list<Terrain>;
}

MasterRenderer::~MasterRenderer()
{
	if (shader)
	{
		shader->cleanUp();
		delete shader;
	}
	if (renderer)
		delete renderer;
	
	if (terrainShader)
	{
		terrainShader->cleanUp();
		delete terrainShader;
	}
	if (terrainRenderer)
		delete terrainRenderer;

	//if (terrains)
	//	delete terrains;

}

void MasterRenderer::render(Light sun, CameraControl *camera)
{
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//prepare();
	shader->start();
	shader->loadLight(sun);
	shader->loadViewMatrix(camera);
	renderer->render(entities);
	shader->stop();

	terrainShader->start();
	terrainShader->loadLight(sun);
	terrainShader->loadViewMatrix(camera);
	terrainRenderer->render(terrains, terrainTexture);
	terrainShader->stop();
	terrains.clear();
	entities.clear();

}

void MasterRenderer::processTerrain(Terrain* terrain)
{
	terrains.push_back(terrain);
}

void MasterRenderer::processEntity(Entity entity)
{
	TexturedModel* entityModel = entity.getModel();
	std::list<Entity> batch;
	if (entities.find(entityModel) == entities.end())
	{
		// do nothing
	}
	else
	{
		batch = entities.find(entityModel)->second;
	}
	//std::list<Entity> batch = entities.find(entityModel)->second;

	if (!batch.empty())
	{
		batch.push_back(entity);
		entities[entityModel] = batch;
	}
	else
	{
		std::list<Entity> newBatch;
		newBatch.push_back(entity);
		entities.emplace(entityModel, newBatch);
	}

	//delete(entityModel);
}

void MasterRenderer::prepare(void)
{
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.49f, 89.0f, 0.98f, 1.0);
}

void MasterRenderer::cleanup(void)
{
	entities.clear();
	terrains.clear();
}

//void MasterRenderer::updateDisplayDimensions(int w, int h)
//{
//	width = w;
//	height = h;
//}

void MasterRenderer::createProjectionMatrix(void)
{

	projectionMatrix = vmath::perspective(FOV, Display::getAspectRatio(), NEAR_PLANE, FAR_PLANE);

}

int Display::winWidth = 800;
int Display::winHeight = 600;
