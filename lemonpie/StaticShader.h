#pragma once
#include "Shader.h"
#include "CameraControl.h"
#include "Light.h"
class StaticShader : public Shader
{
private:
	//const char* VERTEX_FILE = "Shaders/simple.vs";
	//const char* FRAGMENT_FILE = "Shaders/simple.fs";
	GLuint location_transformationMatrix;
	GLuint location_projectionMatrix;
	GLuint location_viewMatrix;
	GLuint location_lightPosition;
	GLuint location_lightColour;
	GLuint location_shineDamper;
	GLuint location_reflectivity;
	GLuint location_sampler;

public:
	StaticShader(const char* VERTEX_FILE, const char* FRAGMENT_FILE);
	~StaticShader();
	void loadShineVariables(float damper, float reflectivity);

	void loadTransformationMatrix(vmath::mat4& matrix);

	void loadLight(Light light);

	void loadViewMatrix(CameraControl* camera);

	void loadProjectionMatrix(mat4 projection);

	void loadSampler(GLuint sampler);

	void bindAttributes(void) override;
	void getAllUniformLocations(void) override;

};

