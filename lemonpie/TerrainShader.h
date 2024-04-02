#pragma once
// OGL
#include <gl\glew.h> // for GLSL extensions IMPORTANT : This Line Should Be Before #include<gl\gl.h> And #include<gl\glu.h>
#include <gl/GL.h>
// math
#include "vmath.h"

#include "Light.h"
#include "CameraControl.h"
#include "Shader.h"

class TerrainShader : public Shader
{

private:
	const char* VERTEX_FILE = "Shaders/simple.vs";
	const char* FRAGMENT_FILE = "Shaders/simple.fs";
	GLuint location_transformationMatrix;
	GLuint location_projectionMatrix;
	GLuint location_viewMatrix;
	GLuint location_lightPosition;
	GLuint location_lightColour;
	GLuint location_shineDamper;
	GLuint location_reflectivity;
	GLuint location_sampler;

public:
	TerrainShader(const char* VERTEX_FILE, const char* FRAGMENT_FILE);
	~TerrainShader();
	void loadShineVariables(float damper, float reflectivity);

	void loadTransformationMatrix(vmath::mat4& matrix);

	void loadLight(Light light);

	void loadViewMatrix(CameraControl* camera);

	void loadProjectionMatrix(mat4 projection);

	void loadSampler(GLuint sampler);

	void bindAttributes(void) override;
	void getAllUniformLocations(void) override;


};

