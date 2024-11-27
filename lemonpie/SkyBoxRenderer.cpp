#include "SkyBoxRenderer.h"


SkyBoxRenderer::SkyBoxRenderer(SkyBoxShader* shader, Loader loader, vmath::mat4 projectionMatrix)
{
	textureSkybox = new Texture(textureFiles);
	this->shader = shader;
	cube = loader.loadToVAO(VERTICES, 3, 36);
	//texture = loader.loadCubMap_stb(tectureFiles);
	shader->bindAttributes();
	shader->getAllUniformLocations();
	shader->start();//shader start();
	shader->loadProjectionMatrix(projectionMatrix);
	shader->stop();
	//load modelmatrix
	//load view matrix
	//active texture
	//bind texture

}


void SkyBoxRenderer::render(CameraControl* camera)
{
	shader->start();
	shader->loadViewMatrix(camera);
	glBindVertexArray(cube->getVaoID());
	glEnableVertexAttribArray(0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureSkybox->getTextureId());
	glDrawArrays(GL_TRIANGLES, 0, cube->getVertexCount());
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	shader->stop();

}


