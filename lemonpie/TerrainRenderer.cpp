#include "TerrainRenderer.h"


TerrainRenderer::TerrainRenderer(TerrainShader* shader, mat4 projectionMatrix)
{
	// here these function calls are additional work to ceate the Terrain/+ shader object properly.
	// There should be another way to call these functions from the constructor of the base class
	// Which is possible in JAVA. But in c++ it's not recomended. Keep looking for solution. Ask people.
	shader->bindAttributes();
	shader->getAllUniformLocations();
	this->shader = shader;
	shader->start();
	shader->loadProjectionMatrix(projectionMatrix);
	shader->stop();
	
}

void TerrainRenderer::prepareTerrain(Terrain* terrain)
{
	RawModel* model = terrain->getModel();
	glBindVertexArray(model->getVaoID());
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	shader->loadShineVariables(1, 0);
	shader->loadSampler(0);
	// load shine and reflectivity vars
	//glUniform1f(location_shineDamper, 1);
	//glUniform1f(location_reflectivity, 0);
	/*glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_terrain->getTextureId());*/
	//texture_terrain->bindTexture(0);
	//texture.bindTexture(0);
	//glUniform1i(location_sampler, 0);
	//free(model);

}

void TerrainRenderer::loadTransformationMatrix(void)
{
}

void TerrainRenderer::loadModelMatrix(Terrain* terrain)
{
	mat4 transformationMatrix = mat4::identity();
	transformationMatrix = translate(terrain->getX(), 0.0f, terrain->getZ());
	shader->loadTransformationMatrix(transformationMatrix);
	//glUniformMatrix4fv(location_transformationMatrix, 1, GL_FALSE, transformationMatrix);
}

void TerrainRenderer::unbindTextureModel(void)
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindVertexArray(0);
}

void TerrainRenderer::loadViewMatrix(void)
{

}


void TerrainRenderer::render(Terrain* terrain, Texture* texture)
{
	texture->bindTexture(0);
	prepareTerrain(terrain);
	loadModelMatrix(terrain);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	unbindTextureModel();

}
