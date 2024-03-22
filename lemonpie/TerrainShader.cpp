#include "TerrainShader.h"



void TerrainShader::bindAttributes(void) // to be overloaded with main shader class
{
	/*bindAttributes(0, "position");
	bindAttributes(1, "textureCoordinates");
	bindAttributes(2, "normal");*/
}

void TerrainShader::getAllUniformLocations(GLuint shaderObject)
{
	location_transformationMatrix = glGetUniformLocation(shaderObject, "transformationMatrix");
	location_projectionMatrix = glGetUniformLocation(shaderObject, "projectionMatrix");
	location_viewMatrix = glGetUniformLocation(shaderObject, "viewMatrix");
	location_lightPosition = glGetUniformLocation(shaderObject, "lightPosition");
	location_lightColour = glGetUniformLocation(shaderObject, "lightColour");
	location_shineDamper = glGetUniformLocation(shaderObject, "shineDamper");
	location_reflectivity = glGetUniformLocation(shaderObject, "reflectivity");
	location_sampler = glGetUniformLocation(shaderObject, "modelTexture");
}

void TerrainShader::loadShineVariables(float damper, float reflectivity) {
	//super.loadFloat(location_shineDamper, damper);
	//super.loadFloat(location_reflectivity, reflectivity);
	// load shine and reflectivity vars
	glUniform1f(location_shineDamper, damper);
	glUniform1f(location_reflectivity, reflectivity);
}

void TerrainShader::loadTransformationMatrix(vmath::mat4& matrix) {
	/*mat4 transformationMatrix = mat4::identity();
	transformationMatrix = translate(newTerrain->getX(), 0.0f, newTerrain->getZ());*/
	glUniformMatrix4fv(location_transformationMatrix, 1, GL_FALSE, matrix);
}

void TerrainShader::loadLight(Light light) {
	//super.loadVector(location_lightPosition, light.getPosition());
	//super.loadVector(location_lightColour, light.getColour());
	vec3 pos = light.getPosition();
	vec3 color = light.getColor();
		//load light
	glUniform3f(location_lightPosition, pos[0], pos[1], pos[2]);// 0.0f, 0.0f, 0.0f);
	glUniform3f(location_lightColour, color[0], color[1], color[2]);// 1.0f, 1.0f, 1.0f);
}

void TerrainShader::loadViewMatrix(CameraControl camera) {
	//Matrix4f viewMatrix = Maths.createViewMatrix(camera);
	//super.loadMatrix(location_viewMatrix, viewMatrix);
	//load view matrix
	glUniformMatrix4fv(location_viewMatrix, 1, GL_FALSE, camera.getLookAtMatrix());
}

void TerrainShader::loadProjectionMatrix(mat4 projection) {
	/*super.loadMatrix(location_projectionMatrix, projection);*/
	glUniformMatrix4fv(location_projectionMatrix, 1, GL_FALSE, projection);
}
