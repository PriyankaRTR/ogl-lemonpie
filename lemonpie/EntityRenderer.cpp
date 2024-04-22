#include "EntityRenderer.h"



EntityRenderer::EntityRenderer(StaticShader* shader, mat4 projectionMatrix)
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

EntityRenderer::~EntityRenderer()
{
	/*if (shader)
		delete shader;*/
}

void EntityRenderer::prepareTexturedModel(TexturedModel* model)
{
	RawModel* rawModel = model->getRawModel();
	glBindVertexArray(rawModel->getVaoID());
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	shader->loadShineVariables(1, 0);
	Texture* texture = model->getTexture();
	texture->bindTexture();
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

void EntityRenderer::prepareInstance(Entity entity)
{
	vmath::mat4 transformationMatrix = mat4::identity();
	transformationMatrix = translate(entity.getPosition());
	shader->loadTransformationMatrix(transformationMatrix);
}

void EntityRenderer::loadTransformationMatrix(void)
{
}

void EntityRenderer::loadModelMatrix(TexturedModel* model)
{
	mat4 transformationMatrix = mat4::identity();
	//transformationMatrix = translate(0.0f, 0.0f, -10.0f);
	shader->loadTransformationMatrix(transformationMatrix);
	//glUniformMatrix4fv(location_transformationMatrix, 1, GL_FALSE, transformationMatrix);
}

void EntityRenderer::unbindTextureModel(void)
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindVertexArray(0);
}

void EntityRenderer::loadViewMatrix(void)
{

}


void EntityRenderer::render(std::map<TexturedModel*, std::list<Entity>> &entities)
{
	for (auto& newSet : entities)
	{
		TexturedModel* model = newSet.first;
		RawModel* tempRawModel = model->getRawModel();
		prepareTexturedModel(model);
		std::list<Entity> batch = newSet.second;
		for (Entity entity : batch)
		{
			//loadModelMatrix(model);
			prepareInstance(entity);
			glDrawArrays(GL_TRIANGLES, 0, (tempRawModel->getFaceCount() * 3 * 3));

		}
		unbindTextureModel();
		
	}
}


//void EntityRenderer::render(TexturedModel* model)
//{
//	RawModel *tempRawModel = model->getRawModel();
//	//texture->bindTexture(0);
//	prepareTexturedModel(model);
//	loadModelMatrix(model);
//	glDrawArrays(GL_TRIANGLES, 0, (tempRawModel->getFaceCount() * 3 * 3));// or
//	//glDrawElements(GL_TRIANGLES, tempRawModel->getVertexCount(), GL_UNSIGNED_INT, 0);
//	unbindTextureModel();
//
//	// IMP to delete explicitly since the memory allocated on heap/free store
//	//delete(tempRawModel);
//}
