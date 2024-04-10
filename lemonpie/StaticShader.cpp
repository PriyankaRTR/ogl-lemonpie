#include "StaticShader.h"

StaticShader::StaticShader(const char* VERTEX_FILE, const char* FRAGMENT_FILE) : Shader(VERTEX_FILE, FRAGMENT_FILE)
{
	location_transformationMatrix = 0;
	location_projectionMatrix = 0;
	location_viewMatrix = 0;
	location_lightPosition = 0;
	location_lightColour = 0;
	location_shineDamper = 0;
	location_reflectivity = 0;
	location_sampler = 0;
}

StaticShader::~StaticShader()
{

}


void StaticShader::bindAttributes(void)// to be overloaded with main shader class
{
	Shader::bindAttribute(0, "position");
	Shader::bindAttribute(1, "textureCoordinates");
	Shader::bindAttribute(2, "normal");
}

void StaticShader::getAllUniformLocations(void)
{
	location_transformationMatrix = Shader::getUniformLocation("transformationMatrix");
	if (location_transformationMatrix == 0xffffffff) {
		fprintf(stderr, "Warning! Unable to get the location of uniform '%s'\n", "transformationMatrix");
	}
	location_projectionMatrix = Shader::getUniformLocation("projectionMatrix");
	location_viewMatrix = Shader::getUniformLocation("viewMatrix");
	location_lightPosition = Shader::getUniformLocation("lightPosition");
	location_lightColour = Shader::getUniformLocation("lightColour");
	location_shineDamper = Shader::getUniformLocation("shineDamper");
	location_reflectivity = Shader::getUniformLocation("reflectivity");
	location_sampler = Shader::getUniformLocation("modelTexture");
}

void StaticShader::loadShineVariables(float damper, float reflectivity) {
	//super.loadFloat(location_shineDamper, damper);
	//super.loadFloat(location_reflectivity, reflectivity);
	// load shine and reflectivity vars
	//glUniform1f(location_shineDamper, damper);
	//glUniform1f(location_reflectivity, reflectivity);

	Shader::loadFloat(location_shineDamper, damper);
	Shader::loadFloat(location_reflectivity, reflectivity);
}

void StaticShader::loadTransformationMatrix(vmath::mat4& matrix) {
	/*mat4 transformationMatrix = mat4::identity();
	transformationMatrix = translate(newTerrain->getX(), 0.0f, newTerrain->getZ());*/
	//glUniformMatrix4fv(location_transformationMatrix, 1, GL_FALSE, matrix);
	Shader::loadMatrix(location_transformationMatrix, matrix);
}

void StaticShader::loadLight(Light light) {
	//super.loadVector(location_lightPosition, light.getPosition());
	//super.loadVector(location_lightColour, light.getColour());
	vec3 pos = light.getPosition();
	vec3 color = light.getColor();
	//load light
//glUniform3f(location_lightPosition, pos[0], pos[1], pos[2]);// 0.0f, 0.0f, 0.0f);
//glUniform3f(location_lightColour, color[0], color[1], color[2]);// 1.0f, 1.0f, 1.0f);
	Shader::loadVector(location_lightPosition, pos);
	Shader::loadVector(location_lightColour, color);
}

void StaticShader::loadViewMatrix(CameraControl* camera) {
	//Matrix4f viewMatrix = Maths.createViewMatrix(camera);
	//super.loadMatrix(location_viewMatrix, viewMatrix);
	//load view matrix
	//glUniformMatrix4fv(location_viewMatrix, 1, GL_FALSE, camera.getLookAtMatrix());
	Shader::loadMatrix(location_viewMatrix, camera->getLookAtMatrix());
}

void StaticShader::loadProjectionMatrix(mat4 projection) {
	/*super.loadMatrix(location_projectionMatrix, projection);*/
	//glUniformMatrix4fv(location_projectionMatrix, 1, GL_FALSE, projection);
	Shader::loadMatrix(location_projectionMatrix, projection);
}

void StaticShader::loadSampler(GLuint sampler)
{
	Shader::loadUint(location_sampler, sampler);
}

