#include "TerrainRenderer.h"


TerrainRenderer::TerrainRenderer(TerrainShader shader, mat4 projectionMatrix)
{
	this->shader = shader;
	
}

void TerrainRenderer::prepareTerrain(Terrain terrain, Texture texture)
{
	RawModel* model = terrain.getModel();
	glBindVertexArray(model->getVaoID());
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	shader.loadShineVariables(1, 0);
	// load shine and reflectivity vars
	//glUniform1f(location_shineDamper, 1);
	//glUniform1f(location_reflectivity, 0);
	/*glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_terrain->getTextureId());*/
	//texture_terrain->bindTexture(0);
	texture.bindTexture(0);
	//glUniform1i(location_sampler, 0);
	free(model);

}

void TerrainRenderer::loadTransformationMatrix(void)
{
}

void TerrainRenderer::unbindTextureModel(void)
{
}

void TerrainRenderer::loadViewMatrix(void)
{
}


void TerrainRenderer::render(void)
{
}
