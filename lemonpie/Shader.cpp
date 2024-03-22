#include "Shader.h"


GLuint Shader::loadShader(char* file, GLuint type)
{
	std::string s;
	GLchar* shaderCode;
	GLuint shaderObjId;
	
	shaderObjId = glCreateShader(type);

	////assign shader object ids
	//if (type == GL_VERTEX_SHADER)
	//	vertexShaderID = shaderObjId;
	//else if (type == GL_FRAGMENT_SHADER)
	//	fragmentShaderID = shaderObjId;

	if (readFile(file, s))
	{
		shaderCode = (GLchar*)s.c_str();
		glShaderSource(shaderObjId, 1, (GLchar**)shaderCode, NULL);
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
					//fprintf(gpFile, "Vertex Shader Compilation Log : %s\n", szInfoLog); either keep a common file pointer and write into a same log file or use seperate file
					free(szInfoLog);
					glDeleteShader(shaderObjId);
					exit(0);
				}
			}
		}

	}
	else
	{
		glDeleteShader(shaderObjId);
	}

	return shaderObjId;
	
}

bool Shader::readFile(const char* pFileName, std::string& outFile)
{

	std::ifstream f{};

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




Shader::Shader(char* fileVertexShader, char* fileFragmentShader)
{
	programID = 0;
	vertexShaderID = 0;
	fragmentShaderID = 0;

	vertexShaderID = loadShader(fileVertexShader, GL_VERTEX_SHADER);
	fragmentShaderID = loadShader(fileFragmentShader, GL_FRAGMENT_SHADER);

	programID = glCreateProgram();

	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);

	glLinkProgram(programID);

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

int Shader::getUniformLocation(char* uniformName)
{
	return 0;
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

void Shader::bindAttribute(GLuint attribute, char* variableName)
{
	glBindAttribLocation(programID, attribute, variableName);
}
