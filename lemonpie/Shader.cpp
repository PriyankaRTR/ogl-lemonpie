#include "Shader.h"


GLuint Shader::loadShader(const char* file, GLuint type)
{
	std::string s;
	GLchar* shaderCode;
	GLuint shaderObjId = 0;
	
	if (readFile(file, s))
	{
		shaderCode = (GLchar*)s.c_str();
		shaderObjId = glCreateShader(type);
		glShaderSource(shaderObjId, 1, (GLchar**)&shaderCode, NULL);
		glCompileShader(shaderObjId);
		GLint iInfoLogLength = 0;
		GLint iShaderCompiledStatus = 0;
		char* szInfoLog = NULL;
		glGetShaderiv(shaderObjId, GL_COMPILE_STATUS, &iShaderCompiledStatus);
		if (iShaderCompiledStatus == GL_FALSE)
		{
			glGetShaderiv(shaderObjId, GL_INFO_LOG_LENGTH, &iInfoLogLength);
			if (iInfoLogLength > 0)
			{
				szInfoLog = (char*)malloc(iInfoLogLength);
				if (szInfoLog != NULL)
				{
					GLsizei written;
					glGetShaderInfoLog(shaderObjId, iInfoLogLength, &written, szInfoLog);
					logger.log(LOG_INFO, "Shader Compilation Log");
					logger.log(LOG_ERROR, szInfoLog);
					//either keep a common file pointer and write into a same log file or use seperate file
					free(szInfoLog);
					glDeleteShader(shaderObjId);
					exit(0);
				}
			}
		}

	}


	return shaderObjId;
	
}

bool Shader::readFile(const char* pFileName, std::string& outFile)
{

	std::ifstream f(pFileName);

	bool ret = false;

	if (f.is_open())
	{
		std::string line;

		while (getline(f, line))
		{
			outFile.append(line);
			outFile.append("\n");
		}

		f.close();
		ret = true;
	}
	else
	{
		// report error in logfile. take logfile pointer as an input;
	}

	return ret;
}


Shader::Shader(const char* fileVertexShader,const char* fileFragmentShader)//, FILE* fp)
{
	programID = 0;
	vertexShaderID = 0;
	fragmentShaderID = 0;
	//fpLog = fp;
	

	vertexShaderID = loadShader(fileVertexShader, GL_VERTEX_SHADER);
	fragmentShaderID = loadShader(fileFragmentShader, GL_FRAGMENT_SHADER);

	programID = glCreateProgram();

	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	//bindAttributes();
	glLinkProgram(programID);
	glValidateProgram(programID);
	//createLogFilePtr();
	//getAllUniformLocations();
}

Shader::~Shader()
{
}

void Shader::start(void)
{
	glUseProgram(programID);
}

void Shader::stop(void)
{
	glUseProgram(0);
}

void Shader::cleanUp(void)
{
	stop();
	if (programID)
	{
		glDetachShader(programID, vertexShaderID);
		glDetachShader(programID, fragmentShaderID);
		glDeleteProgram(programID);
	}
	if(vertexShaderID)
	glDeleteShader(vertexShaderID);
	
	if(fragmentShaderID)
	glDeleteShader(fragmentShaderID);

}

GLuint Shader::getUniformLocation(const char* uniformName)
{
	return glGetUniformLocation(programID, uniformName);
}

void Shader::loadMatrix(GLuint location, vmath::mat4 matrix)
{
	glUniformMatrix4fv(location, 1, GL_FALSE, matrix);
}

void Shader::loadVector(GLuint location, vmath::vec3 vector)
{
	glUniform3fv(location, 1, vector);
}

void Shader::loadFloat(GLuint location, GLfloat value)
{
	glUniform1f(location, value);
}

void Shader::loadBoolean(GLuint location, GLboolean value)
{
	float boolVal = 0;
	if (value)
	{
		boolVal = 1;
	}
	glUniform1f(location, boolVal);
}

void Shader::loadUint(GLuint location, GLuint value)
{
	glUniform1i(location, value);
}

void Shader::bindAttribute(GLuint attribute, const char* variableName)
{
	glBindAttribLocation(programID, attribute, variableName);
}

// you always initialize staic member like this since memory is allocated only once.
// Also avoid using pointers for object so that you don't have to manage memory free operation
// refer this good link,
// https://stackoverflow.com/questions/2769588/how-to-free-static-member-variable-in-c
Logger Shader::logger("shadersLog.txt");