#include "OBJLoader.h"

void OBJLoader::processVertexData(void)
{
	unsigned long int size, mallocSize;
	size = g_face_tri.size();
	mallocSize = sizeof(GLfloat) * size * 3 * 3;
	int vArrayIndex = 0;
	//GLfloat arr[40000];
	//int arrIndex=0;
	if (size)
	{
		vertexArray = (GLfloat*)malloc(mallocSize);

		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < g_face_tri[i].size(); j++)
			{
				int vi = g_face_tri[i][j] - 1;

				//arr[arrIndex++] = g_vertices[vi][0];
				//arr[arrIndex++] = g_vertices[vi][1];
				//arr[arrIndex++] = g_vertices[vi][2];

				// vertex1
				vertexArray[vArrayIndex++] = g_vertices[vi][0];
				vertexArray[vArrayIndex++] = g_vertices[vi][1];
				vertexArray[vArrayIndex++] = g_vertices[vi][2];

			}
		}
	}
}

void OBJLoader::processNormalsData(void)
{
	unsigned long int size, mallocSize;
	size = g_face_normal.size();
	mallocSize = sizeof(GLfloat) * size * 3 * 3;
	int nArrayIndex = 0;
	GLfloat arr[9];
	if (size)
	{
		normalsArray = (GLfloat*)malloc(mallocSize);

		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < g_face_normal[i].size(); j++)
			{
				int vi = g_face_normal[i][j] - 1;

				arr[0] = g_normals[vi][0];
				arr[1] = g_normals[vi][1];
				arr[2] = g_normals[vi][2];

				// vertex1
				normalsArray[nArrayIndex++] = g_normals[vi][0];
				normalsArray[nArrayIndex++] = g_normals[vi][1];
				normalsArray[nArrayIndex++] = g_normals[vi][2];

			}

		}

	}
}

void OBJLoader::processTextureData(void)
{
	unsigned long int size, mallocSize;
	size = g_face_texture.size();
	mallocSize = sizeof(GLfloat) * size * 3 * 2;
	int nArrayIndex = 0;
	GLfloat arr[9];
	if (size)
	{
		textureArray = (GLfloat*)malloc(mallocSize);

		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < g_face_texture[i].size(); j++)
			{
				int vi = g_face_texture[i][j] - 1;

				arr[0] = g_texture[vi][0];
				arr[1] = g_texture[vi][1];


				// textures
				textureArray[nArrayIndex++] = g_texture[vi][0];
				textureArray[nArrayIndex++] = g_texture[vi][1];

			}

		}

	}
}

GLuint OBJLoader::loadObjModel(const char* fileName, Loader& loader)
{
	FILE* g_fp_objFile = NULL;
	g_fp_objFile = fopen(fileName, "r"); //TriangleModel5.obj //cubeTrangulated1.obj // MonkeyHead.obj

	if (!g_fp_objFile)
		exit(-1);
	//if file not found and fp is NULL then we need to have a logic to exit the code safely after uninitialize()

	//tokens
	const char* sep_space = " ";
	const char* sep_fslash = "/";
	/// to hold first word in the line 
	char* first_token = NULL;
	/// to hold next word seperated by strtok
	char* token = NULL;
	char* face_tokens[NR_FACE_TOKENS];

	// non-null tokens  
	int nr_tokens;

	// to hold string associated with each entity
	char* token_vertex_index = NULL;
	char* token_texture_index = NULL;
	char* token_normal_index = NULL;

	while (fgets(line, BUFFER_SIZE, g_fp_objFile) != NULL)
	{
		first_token = strtok(line, sep_space);

		if (strcmp(first_token, "v") == S_EQUAL)
		{
			// create a vector of NR_POINT_CORDS number of floats
			// to hold coordinates
			std::vector<float> vec_point_coord(NR_POINT_COORDS);

			// loop to tokenize further
			for (int i = 0; i != NR_POINT_COORDS; i++)
			{
				vec_point_coord[i] = atof(strtok(NULL, sep_space));
			}
			g_vertices.push_back(vec_point_coord);
		}
		else if (strcmp(first_token, "vt") == S_EQUAL)
		{
			std::vector<float> vec_texture_coord(NR_TEXTURE_COORDS);

			for (int i = 0; i != NR_TEXTURE_COORDS; i++)
				vec_texture_coord[i] = atof(strtok(NULL, sep_space));
			g_texture.push_back(vec_texture_coord);
		}
		else if (strcmp(first_token, "vn") == S_EQUAL)
		{
			std::vector<float> vec_normal_coord(NR_NORMAL_COORDS);

			for (int i = 0; i != NR_NORMAL_COORDS; i++)
				vec_normal_coord[i] = atof(strtok(NULL, sep_space));
			g_normals.push_back(vec_normal_coord);
		}
		else if (strcmp(first_token, "f") == S_EQUAL)
		{
			std::vector<int> triangle_vertex_indices(3), texture_vertex_indices(3), normal_vertex_indices(3);

			// INITIALISE all char pointer
			memset((void*)face_tokens, 0, NR_FACE_TOKENS);

			nr_tokens = 0;
			while (token = strtok(NULL, sep_space))
			{
				if (strlen(token) < 3)
					break;
				if (nr_tokens == 3)
					break;
				face_tokens[nr_tokens] = token;
				nr_tokens++;
			}

			// fetch traingle, texture and normal coordinate index data
			// from every face data entry 
			for (int i = 0; i != NR_FACE_TOKENS; ++i)
			{
				token_vertex_index = strtok(face_tokens[i], sep_fslash);
				token_texture_index = strtok(NULL, sep_fslash);
				token_normal_index = strtok(NULL, sep_fslash);

				triangle_vertex_indices[i] = atoi(token_vertex_index);
				texture_vertex_indices[i] = atoi(token_texture_index);
				normal_vertex_indices[i] = atoi(token_normal_index);
			}

			// add constructed vectors to global face vectors
			g_face_tri.push_back(triangle_vertex_indices);
			g_face_texture.push_back(texture_vertex_indices);
			g_face_normal.push_back(normal_vertex_indices);
		}

		memset((void*)line, (int)'\0', BUFFER_SIZE);

	}

	fclose(g_fp_objFile);
	g_fp_objFile = NULL;

	unsigned long long int vSize, tSize, nSize;
	vSize = g_vertices.size();
	tSize = g_texture.size();
	nSize = g_normals.size();
	fSize = g_face_tri.size();
	//fprintf(gpFile, "g_vertices:%llu g_texture:%llu g_normals:%llu g_face_tri:%llu\n",
	//	vSize, tSize, nSize, fSize);

	processVertexData();
	processTextureData();
	processNormalsData();
		
	// here write a new method to load the model in conventional way
	return (loader.loadToVAO(vertexArray, normalsArray, textureArray, (fSize * 3)));
}

//
//float* OBJLoader::getVertices(void)
//{
//	return vertexArray;
//}
//
//float* OBJLoader::getNormals(void)
//{
//	return normalsArray;
//}
//
//float* OBJLoader::getTextureCoords(void)
//{
//	return textureArray;
//}
////
////int* OBJLoader::getIndices(void)
////{
////	return indices;
////}
//
//int OBJLoader::getFaceSize(void)
//{
//	return fSize;
//}