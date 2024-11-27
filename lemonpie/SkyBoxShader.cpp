#include "SkyBoxShader.h"

SkyBoxShader::SkyBoxShader(const char* VERTEX_FILE, const char* FRAGMENT_FILE) : Shader(VERTEX_FILE, FRAGMENT_FILE)
{

	location_projectionMatrix = 0;
	location_viewMatrix = 0;
	location_sampler = 0;
}

SkyBoxShader::~SkyBoxShader()
{

}



void SkyBoxShader::loadViewMatrix(CameraControl* camera)
{
	Shader::loadMatrix(location_viewMatrix, camera->getLookAtMatrix());
}

void SkyBoxShader::loadProjectionMatrix(vmath::mat4 projection)
{
	Shader::loadMatrix(location_projectionMatrix, projection);
}

void SkyBoxShader::loadSampler(GLuint sampler)
{
	Shader::loadUint(location_sampler, 0);
	
}

void SkyBoxShader::bindAttributes(void)
{
	Shader::bindAttribute(0, "Position"); 
	Shader::bindAttribute(1, "textureCoords");
	
}

void SkyBoxShader::getAllUniformLocations(void)
{
	location_sampler = getUniformLocation("cubeMapTexture");
	location_projectionMatrix = getUniformLocation("projectionMatrix");
	location_viewMatrix = getUniformLocation("viewMatrix");
}
