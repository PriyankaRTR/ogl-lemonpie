#pragma once
// OGL
#include <gl\glew.h> // for GLSL extensions IMPORTANT : This Line Should Be Before #include<gl\gl.h> And #include<gl\glu.h>
#include <gl/GL.h>
// math
#include "vmath.h"

#include "Light.h"
#include "CameraControl.h"

class TerrainShader
{
private:
	TerrainShader();
	void bindAttributes(void);
	void getAllUniformLocations(GLuint shaderObject);
	GLuint location_transformationMatrix;
	GLuint location_projectionMatrix;
	GLuint location_viewMatrix;
	GLuint location_lightPosition;
	GLuint location_lightColour;
	GLuint location_shineDamper;
	GLuint location_reflectivity;
	GLuint location_sampler;

public:
	void loadShineVariables(float damper, float reflectivity);

	void loadTransformationMatrix(vmath::mat4& matrix);

	void loadLight(Light light);

	void loadViewMatrix(CameraControl camera);

	void loadProjectionMatrix(mat4 projection);


};

