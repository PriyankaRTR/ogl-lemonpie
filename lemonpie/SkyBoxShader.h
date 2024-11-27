#pragma once
// OGL
#include <gl\glew.h> // for GLSL extensions IMPORTANT : This Line Should Be Before #include<gl\gl.h> And #include<gl\glu.h>
#include <gl/GL.h>
#include "Shader.h"
#include "CameraControl.h"

#include "vmath.h"
class SkyBoxShader : public Shader
{
private:
	const char* VERTEX_FILE = "Shaders/skyBox.vs";
	const char* FRAGMENT_FILE = "Shaders/skyBox.fs";
	
	GLuint location_projectionMatrix;
	GLuint location_viewMatrix;
	GLuint location_sampler;


public:
	SkyBoxShader(const char* VERTEX_FILE, const char* FRAGMENT_FILE);
	~SkyBoxShader();

	void loadProjectionMatrix(vmath::mat4 projection); 

	void loadViewMatrix(CameraControl* camera);

	void loadSampler(GLuint sampler);

	void bindAttributes(void) override;
	void getAllUniformLocations(void) override;

};

