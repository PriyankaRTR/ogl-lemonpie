#pragma once

#include<string>
#include<fstream>

#include<gl/glew.h>
#include<gl\GL.h>

#include"vmath.h"
class Shader
{
private:

	GLuint programID;
	GLuint vertexShaderID;
	GLuint fragmentShaderID;
	

	GLuint loadShader(char* file, GLuint type);
	bool readFile(const char* pFileName, std::string& outFile);
	
public:
	Shader(char* fileVertexShader, char* fileFragmentShader);
	void start(void);
	void stop(void);
	void cleanUp(void);


protected:
	//it's a pure virtual function since the implmentation will be different in each derived class
	// to be implemented as 'override' in derived class
	// check difference between pure virtual function in c++ and abstract method in java => it's the same. just different terminology :)
	virtual void getAllUniformLocations(void) = 0;

	int getUniformLocation(char* uniformName);
	void loadMatrix(GLuint location, vmath::mat4 matrix);
	void loadVector(GLuint location, vmath::vec3 vector);
	void loadFloat(GLuint location, GLfloat value);
	void loadBoolean(GLuint location, GLboolean value);
	void bindAttribute(GLuint attribute, char* variableName);

};

