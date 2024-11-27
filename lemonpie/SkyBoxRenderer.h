#pragma once
#include<windows.h>
// OGL
#include <gl\glew.h> // for GLSL extensions IMPORTANT : This Line Should Be Before #include<gl\gl.h> And #include<gl\glu.h>
#include <gl/GL.h>

#include "Loader.h"
#include "SkyBoxShader.h"
#include "Texture.h"

#include "vmath.h"

class SkyBoxRenderer
{
private:
	
	static const int numberOfVertices = 36;
	const GLfloat SIZE = 800.0f;

	GLfloat VERTICES[numberOfVertices*3]{
		-SIZE,  SIZE, -SIZE,//back
		-SIZE, -SIZE, -SIZE,
		SIZE, -SIZE, -SIZE,
		 SIZE, -SIZE, -SIZE,
		 SIZE,  SIZE, -SIZE,
		-SIZE,  SIZE, -SIZE,


		-SIZE, -SIZE,  SIZE, //left
		-SIZE, -SIZE, -SIZE,
		-SIZE,  SIZE, -SIZE,
		-SIZE,  SIZE, -SIZE,
		-SIZE,  SIZE,  SIZE,
		-SIZE, -SIZE,  SIZE,


		 SIZE, -SIZE, -SIZE, //right
		 SIZE, -SIZE,  SIZE,
		 SIZE,  SIZE,  SIZE,
		 SIZE,  SIZE,  SIZE,
		 SIZE,  SIZE, -SIZE,
		 SIZE, -SIZE, -SIZE,


		-SIZE, -SIZE,  SIZE, //front
		-SIZE,  SIZE,  SIZE,
		 SIZE,  SIZE,  SIZE,
		 SIZE,  SIZE,  SIZE,
		 SIZE, -SIZE,  SIZE,
		-SIZE, -SIZE,  SIZE,


		-SIZE,  SIZE, -SIZE, //top
		 SIZE,  SIZE, -SIZE,
		 SIZE,  SIZE,  SIZE,
		 SIZE,  SIZE,  SIZE,
		-SIZE,  SIZE,  SIZE,
		-SIZE,  SIZE, -SIZE,


		-SIZE, -SIZE, -SIZE, //bottom
		-SIZE, -SIZE,  SIZE,
		 SIZE, -SIZE, -SIZE,
		 SIZE, -SIZE, -SIZE,
		-SIZE, -SIZE,  SIZE,
		 SIZE, -SIZE,  SIZE
	};

	const char* textureFiles[6] = { "Resources/right.jpg", "Resources/left.jpg", "Resources/top.jpg", "Resources/bottom.jpg", "Resources/back.jpg", "Resources/front.jpg" }; //{ "right.jpg", "left.jpg", "top.jpg", "bottom.jpg", "front.jpg", "back.jpg" };{ "tree.png","tree.png","tree.png","tree.png","tree.png","tree.png" };//{ "right.png", "left.png", "top.png", "bottom.png", "up.png", "down.png" }; //

	RawModel* cube;
	SkyBoxShader* shader;
	Texture* textureSkybox;

public:
	SkyBoxRenderer(SkyBoxShader* shader, Loader loader, vmath::mat4 projectionMatrix);
	~SkyBoxRenderer();
	void render(CameraControl* camera);
};

