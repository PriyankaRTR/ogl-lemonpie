#include <windows.h>
#include <stdio.h> // for FILE I/O
#include <stdlib.h>

#include <gl\glew.h> // for GLSL extensions IMPORTANT : This Line Should Be Before #include<gl\gl.h> And #include<gl\glu.h>

#include <gl/GL.h>

#include "vmath.h"

// c++ header
//#include <iostream>
#include<vector>
#include<fstream>
#include<string>

// ImGui headers
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"

#include "CameraControl.h"
#include "Texture.h";

#define IMGUI_WRAPPER_CLASS  // need to fix, take expert advice

#ifdef IMGUI_WRAPPER_CLASS
#include "ImGui_Wrapper.h" //Priyanka
#endif
#include "Terrain.h"
//#include "Terrain.h"

#define ENABLE_CAMERA_YAW_ROTATION 1
#define ENABLE_CAMERA_PITCH_ROTATION 1

#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"opengl32.lib")

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

using namespace vmath;

#define OBJ_FILE_PATH "3DModels\\MonkeyHead.obj" //singleAeroplane.obj" MonkeyHead.obj


enum
{
	VDG_ATTRIBUTE_VERTEX = 0,
	VDG_ATTRIBUTE_COLOR,
	VDG_ATTRIBUTE_NORMAL,
	VDG_ATTRIBUTE_TEXTURE0,
};

#define BUFFER_SIZE 256
#define S_EQUAL			0
#define PI 3.1415926535898

#define NR_POINT_COORDS		3 // number of point coordinates
#define NR_TEXTURE_COORDS	2 // number of texture coords
#define NR_NORMAL_COORDS	3 // same for notrmal coords
#define NR_FACE_TOKENS		3 // minimum number of entries in face data


//Prototype Of WndProc() declared Globally
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//Global variable declarations
FILE* gpFile = NULL;

HWND ghwnd = NULL;
HDC ghdc = NULL;
HGLRC ghrc = NULL;

DWORD dwStyle;
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) };

bool gbActiveWindow = false;
bool gbEscapeKeyIsPressed = false;
bool gbFullscreen = false;


GLfloat currentWidth;
GLfloat currentHeight;

GLuint gVertexShaderObject;
GLuint gFragmentShaderObject;
GLuint gShaderProgramObject;


GLuint gVao_cube;
GLuint gVbo_cube_position;
GLuint gVbo_cube_normal;
GLuint gVbo_cube_texture;



GLuint gModelViewMatrixUniform, gProjectionMatrixUniform;
GLuint gLdUniform, gKdUniform, gLightPositionUniform;
GLuint gTextureSamplerUniform;

//******** terrain *********
GLuint gVertexShaderObjectTerrain;
GLuint gFragmentShaderObjectTerrain;
GLuint gShaderProgramObjectTerrain;

GLuint gVao_terrain;
GLuint gVbo_terrain_position;
GLuint gVbo_terrain_normal;
GLuint gVbo_terrain_texture;

GLuint location_transformationMatrix;
GLuint location_projectionMatrix;
GLuint location_viewMatrix;
GLuint location_lightPosition;
GLuint location_lightColour;
GLuint location_shineDamper;
GLuint location_reflectivity;
GLuint location_sampler;

GLuint gTexture_terrain;

Texture* texture_terrain;
//*****************



GLuint gTexture_Kundali;
GLuint gTexture_Stone;

Texture* texture_MonkeyHead;

GLuint gLKeyPressedUniform;

mat4 gPerspectiveProjectionMatrix;
mat4 gOrthographicProjectionMatrix;

GLfloat gAngle = 0.0f;

bool gbAnimate;
bool gbLight;

//for obj loading
std::vector<std::vector<float>> g_vertices;
std::vector<std::vector<float>> g_texture;
std::vector<std::vector<float>> g_normals;
std::vector<std::vector<int>> objIndices;
std::vector<std::vector<int>> g_face_tri, g_face_texture, g_face_normal;

std::vector<float > g_test_obj_vertices;

// Input obj file
FILE* g_fp_objFile = NULL;

char line[BUFFER_SIZE];
char filePath[BUFFER_SIZE];

GLfloat* vertexArray;
GLfloat* normalsArray;
GLfloat* textureArray;

unsigned long long int fSize;

float fov = 45.0f;
CameraControl* camera;

#ifdef IMGUI_WRAPPER_CLASS
ImGui_Wrapper* ImGuiWrapper;
ImGuiContext* ImGui_ctx;
float deltaTime;
float xMouseOffset;
float yMouseOffset;
ImVec2 mouseOffset = { 0.0,0.0 }; // ImGui datatype
#endif

//entities
Terrain* newTerrain;

void objDataLoader(void)
{
	void uninitialize(void);
	void processVertexData(void);

	g_fp_objFile = fopen(OBJ_FILE_PATH, "r"); //TriangleModel5.obj //cubeTrangulated1.obj // MonkeyHead.obj

	if (!g_fp_objFile)
		uninitialize();
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
	fprintf(gpFile, "g_vertices:%llu g_texture:%llu g_normals:%llu g_face_tri:%llu\n",
		vSize, tSize, nSize, fSize);



}

void processVertexData(void)
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


void processNormalsData(void)
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

void processTextureData(void)
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

void processObjData(void)
{
	//unsigned long int size, mallocSize;
	//int vi, ni, ti;
	//size = g_face_tri.size();
	//mallocSize = sizeof(GLfloat) * size * 3 * 3;
	//vertexArray = (GLfloat *)malloc(mallocSize);
	//size = g_face_normal.size();
	//mallocSize = sizeof(GLfloat) * size * 3 * 3;
	//normalsArray = (GLfloat*)malloc(mallocSize);
	//size = g_face_texture.size();
	//mallocSize = sizeof(GLfloat) * size * 3 * 2;
	//textureArray = (GLfloat*)malloc(mallocSize);

	//int nArrayIndex = 0;
	//GLfloat arr[9];

	//if (size)
	//{
	//	for (int i = 0; i < g_face_tri.size(); i++)
	//	{
	//		for (int j = 0; j < g_face_tri[i].size(); j++)
	//		{
	//			vi = g_face_tri[i][j] - 1;
	//			ni = g_face_normal[i][j] - 1;
	//			ti = g_face_texture[i][j] - 1;

	//			vertexArray[nArrayIndex++] = g_vertices[vi][0];
	//			normalsArray[nArrayIndex] = g_normals[ni][0];
	//			textureArray[nArrayIndex] = g_texture[ti][0];

	//			vertexArray[nArrayIndex++] = g_vertices[vi][1];
	//			normalsArray[nArrayIndex] = g_normals[ni][1];
	//			textureArray[nArrayIndex] = g_texture[ti][1];

	//			vertexArray[nArrayIndex++] = g_vertices[vi][2];
	//			normalsArray[nArrayIndex] = g_normals[ni][2];				

	//		}
	//	}
	//}

}

//main()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	//function prototype
	void initialize(void);
	void uninitialize(void);
	void display(void);
	void spin(void);
	void updatefov(float yOffset);

	//variable declaration
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szClassName[] = TEXT("OpenGLPP");
	bool bDone = false;

	newTerrain = new Terrain(0, 0);
	//GetModuleFileName(NULL, filePath, BUFFER_SIZE); // to read fully qualified path of .exe. Not used.

	//code
	// create log file
	if (fopen_s(&gpFile, "Log.txt", "w") != 0)
	{
		MessageBox(NULL, TEXT("Log File Can Not Be Created\nExitting ..."), TEXT("Error"), MB_OK | MB_TOPMOST | MB_ICONSTOP);
		exit(0);
	}
	else
	{
		fprintf(gpFile, "Log File Is Successfully Opened.\n");
	}

	//initializing members of struct WNDCLASSEX
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = szClassName;
	wndclass.lpszMenuName = NULL;
	//Registering Class
	RegisterClassEx(&wndclass);

	//Create Window
	//Parallel to glutInitWindowSize(), glutInitWindowPosition() and glutCreateWindow() all three together
	hwnd = CreateWindow(szClassName,
		TEXT("OpenGL Programmable Pipeline Window"),
		WS_OVERLAPPEDWINDOW,
		100,
		100,
		WIN_WIDTH,
		WIN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	ghwnd = hwnd;

	ShowWindow(hwnd, iCmdShow);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);


	objDataLoader();
	//initialize
	initialize();

	/*initImGui(hwnd);*/


	mouseOffset.x = 0.0;
	mouseOffset.y = 0.0;
#ifdef IMGUI_WRAPPER_CLASS  // IMGUI Initialization
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui_ctx = ImGui::CreateContext();
	ImGuiIO& imguiIORef = ImGui::GetIO();
	imguiIORef.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
	imguiIORef.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls
	ImGuiWrapper = new ImGui_Wrapper(hwnd, imguiIORef); // init
	ImGuiWrapper->setupImGui(); // additional settings for my project
	//setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_InitForOpenGL(hwnd);
	ImGui_ImplOpenGL3_Init();

#endif

	camera = new CameraControl(0.0f, 0.1f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);


	//Message Loop
	while (bDone == false) //Parallel to glutMainLoop();
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				bDone = true;
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
#ifdef IMGUI_WRAPPER_CLASS
			// update deltaTime
			// To make camera movement independant of the processing power of the machine
			deltaTime = ImGuiWrapper->getDeltaTime();

			// considering mouse drag delta instead of only mouse delta, in development phase
			ImGuiWrapper->getDeltaMouse(mouseOffset);
			camera->processMouseDrag(mouseOffset);

#endif
			camera->processMouseWheel(ImGuiWrapper->getDeltaMouseWheel());
			updatefov(ImGuiWrapper->getDeltaMouseWheel()); // for zoom-in/ zoom-out
			//fprintf(gpFile, "MouseWheelOffset: %f\n", ImGuiWrapper->getDeltaMouseWheel());
			// rendring function
			display();
#ifdef IMGUI_WRAPPER_CLASS
			ImGuiWrapper->gameLoopUIUpdatesImGui(ImGui_ctx);
#endif
			SwapBuffers(ghdc);
			//fprintf(gpFile, "mouseOffset Log : %f, %f\n", mouseOffset.offsetX, mouseOffset.offsetY);

			if (gbAnimate == true)
				spin();

			if (gbActiveWindow == true)
			{
				if (gbEscapeKeyIsPressed == true) //Continuation to glutLeaveMainLoop();
					bDone = true;
			}
		}



	}

	uninitialize();
#ifdef IMGUI_WRAPPER_CLASS
	//clean up ImGui context
	ImGuiWrapper->cleanUpImGui();
#endif

	return((int)msg.wParam);
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


//WndProc()
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	//function prototype
	void resize(int, int);
	void ToggleFullscreen(void);
	void uninitialize(void);

	//variable declarations
	static bool bIsAKeyPressed = false;
	static bool bIsLKeyPressed = false;

	if (ImGui_ImplWin32_WndProcHandler(hwnd, iMsg, wParam, lParam))
		return true;

	//code
	switch (iMsg)
	{
	case WM_ACTIVATE:
		if (HIWORD(wParam) == 0) //if 0, the window is active
			gbActiveWindow = true;
		else //if non-zero, the window is not active
			gbActiveWindow = false;
		break;
	case WM_ERASEBKGND:
		return(0);
	case WM_SIZE: //Parallel to glutReshapeFunc();
		resize(LOWORD(lParam), HIWORD(lParam)); //Parallel to glutReshapeFunc(resize);
		break;
	case WM_KEYDOWN: //Parallel to glutKeyboardFunc();
		switch (wParam)
		{
		case VK_ESCAPE: //case 27
			if (gbEscapeKeyIsPressed == false)
				gbEscapeKeyIsPressed = true; //Parallel to glutLeaveMainLoop();
			break;
		case 0x46: //for 'f' or 'F'
			if (gbFullscreen == false)
			{
				ToggleFullscreen();
				gbFullscreen = true;
			}
			else
			{
				ToggleFullscreen();
				gbFullscreen = false;
			}
			break;
		case 0x41: // for 'A' or 'a'
			if (bIsAKeyPressed == false)
			{
				gbAnimate = true;
				bIsAKeyPressed = true;
			}
			else
			{
				gbAnimate = false;
				bIsAKeyPressed = false;
			}
			break;
		case 0x4C: // for 'L' or 'l'
			if (bIsLKeyPressed == false)
			{
				gbLight = true;
				bIsLKeyPressed = true;
			}
			else
			{
				gbLight = false;
				bIsLKeyPressed = false;
			}
			break;

			//here delta time is used to get the same performance independant of the processing power of the machine
			//ref - https://learnopengl.com/Getting-started/Camera 
			//"Graphics applications and games usually keep track of a deltatime variable that stores the time it took to render the last frame. 
			// We then multiply all velocities with this deltaTime value. The result is that when we have a large deltaTime in a frame, 
			// meaning that the last frame took longer than average, the velocity for that frame will also be a bit higher to balance it all out. 
			// When using this approach it does not matter if you have a very fast or slow pc, the velocity of the camera will be balanced out accordingly so each user will have the same experience."
		case VK_UP:
			camera->processKeyInputs(FORWARD, deltaTime);//cameraPos += (cameraSpeed * deltaTime) * cameraFront;
			break;

		case VK_DOWN:
			camera->processKeyInputs(BACKWARD, deltaTime);//cameraPos -= (cameraSpeed * deltaTime) * cameraFront;
			break;

		case VK_RIGHT:
			camera->processKeyInputs(RIGHT, deltaTime);//cameraPos += vmath::normalize(vmath::cross(cameraFront, cameraUp)) * (cameraSpeed * deltaTime);
			break;

		case VK_LEFT:
			camera->processKeyInputs(LEFT, deltaTime);//cameraPos -= vmath::normalize(vmath::cross(cameraFront, cameraUp)) * (cameraSpeed * deltaTime);
			break;

		default:
			break;
		}
		break;
	case WM_LBUTTONDOWN:  //Parallel to glutMouseFunc();
		break;
	case WM_CLOSE: //Parallel to glutCloseFunc();
		uninitialize(); //Parallel to glutCloseFunc(uninitialize);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

void ToggleFullscreen(void)
{
	//variable declarations
	MONITORINFO mi;

	//code
	if (gbFullscreen == false)
	{
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);
		if (dwStyle & WS_OVERLAPPEDWINDOW)
		{
			mi = { sizeof(MONITORINFO) };
			if (GetWindowPlacement(ghwnd, &wpPrev) && GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi))
			{
				SetWindowLong(ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
				SetWindowPos(ghwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOZORDER | SWP_FRAMECHANGED);
			}
		}
		ShowCursor(FALSE);
	}

	else
	{
		//code
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);

		ShowCursor(TRUE);
	}
}

//FUNCTION DEFINITIONS
void initialize(void)
{
	//function prototypes
	void uninitialize(void);
	void resize(int, int);
	int LoadGLTextures(GLuint * texture, TCHAR imageResourceId[]);
	void cleanUp(void);
	GLuint loadToVAO(GLfloat * vArray, GLfloat * nArray, GLfloat * tArray, int* indices);
	GLuint loadShader(const GLchar * shaderSource, GLenum type);
	void createShaderProgram(const GLchar * vShader, const GLchar * fShader);

	//variable declarations
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex;

	//code
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	//Initialization of structure 'PIXELFORMATDESCRIPTOR'
	//Parallel to glutInitDisplayMode()
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cRedBits = 8;
	pfd.cGreenBits = 8;
	pfd.cBlueBits = 8;
	pfd.cAlphaBits = 8;
	pfd.cDepthBits = 32;

	ghdc = GetDC(ghwnd);

	//choose a pixel format which best matches with that of 'pfd'
	iPixelFormatIndex = ChoosePixelFormat(ghdc, &pfd);
	if (iPixelFormatIndex == 0)
	{
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}

	//set the pixel format chosen above
	if (SetPixelFormat(ghdc, iPixelFormatIndex, &pfd) == false)
	{
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}

	//create OpenGL rendering context
	ghrc = wglCreateContext(ghdc);
	if (ghrc == NULL)
	{
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}

	//make the rendering context created above as current n the current hdc
	if (wglMakeCurrent(ghdc, ghrc) == false)
	{
		wglDeleteContext(ghrc);
		ghrc = NULL;
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}

	// GLEW Initialization Code For GLSL ( IMPORTANT : It Must Be Here. Means After Creating OpenGL Context But Before Using Any OpenGL Function )
	GLenum glew_error = glewInit();
	if (glew_error != GLEW_OK)
	{
		wglDeleteContext(ghrc);
		ghrc = NULL;
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}

	// *** TERRAIN SHADER AND UNIFORM LOCATIONS ***
	// *** VERTEX SHALDER ***
	const GLchar* vertexShaderTerrain = 
		"#version 400" \
		"\n" \
		"in vec3 position;" \
		"in vec2 textureCoordinates;" \
		"in vec3 normal;" \
		"out vec2 pass_textureCoordinates;" \
		"out vec3 surfaceNormal;" \
		"out vec3 toLightVector;" \
		"out vec3 toCameraVector;" \
		"uniform mat4 transformationMatrix;" \
		"uniform mat4 projectionMatrix;" \
		"uniform mat4 viewMatrix;" \
		"uniform vec3 lightPosition;" \
		"void main(void)" \
		"{" \
		"vec4 worldPosition = transformationMatrix * vec4(position,1.0);" \
		"gl_Position = projectionMatrix * viewMatrix * worldPosition;" \
		"pass_textureCoordinates = textureCoordinates * 40.0;" \
		"" \
		"surfaceNormal = (transformationMatrix * vec4(normal,0.0)).xyz;" \
		"toLightVector = lightPosition - worldPosition.xyz;" \
		"toCameraVector = (inverse(viewMatrix) * vec4(0.0,0.0,0.0,1.0)).xyz - worldPosition.xyz;" \
		"}";

	const GLchar* fragmentShaderTerrain =
		"#version 400" \
		"\n" \
		"in vec2 pass_textureCoordinates;" \
		"in vec3 surfaceNormal;" \
		"in vec3 toLightVector;" \
		"in vec3 toCameraVector;" \
		"out vec4 out_Color;" \
		"" \
		"uniform sampler2D modelTexture;" \
		"uniform vec3 lightColour;" \
		"uniform float shineDamper;" \
		"uniform float reflectivity;" \
		"" \
		"void main(void)" \
		"{" \
		"" \
		"vec3 unitNormal = normalize(surfaceNormal);" \
		"vec3 unitLightVector = normalize(toLightVector);" \
		"" \
		"float nDotl = dot(unitNormal,unitLightVector);" \
		"float brightness = max(nDotl,0.2);" \
		"vec3 diffuse = brightness * lightColour;" \
		"" \
		"vec3 unitVectorToCamera = normalize(toCameraVector);" \
		"vec3 lightDirection = -unitLightVector;" \
		"vec3 reflectedLightDirection = reflect(lightDirection,unitNormal);" \
		"" \
		"float specularFactor = dot(reflectedLightDirection , unitVectorToCamera);" \
		"specularFactor = max(specularFactor,0.0);" \
		"float dampedFactor = pow(specularFactor,shineDamper);" \
		"vec3 finalSpecular = dampedFactor * reflectivity * lightColour;" \
		"" \
		"out_Color =  vec4(diffuse,1.0) * texture(modelTexture,pass_textureCoordinates) + vec4(finalSpecular,1.0);" \
		"}";


	createShaderProgram(vertexShaderTerrain, fragmentShaderTerrain);


	// *** VERTEX SHADER ***
	// create shader
	gVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	// provide source code to shader
	const GLchar* vertexShaderSourceCode =
		"#version 130" \
		"\n" \
		"in vec4 vPosition;" \
		"in vec3 vNormal;" \
		"in vec2 vTexture0_Coord;" \
		"uniform mat4 u_model_view_matrix;" \
		"uniform mat4 u_projection_matrix;" \
		"uniform int u_LKeyPressed;" \
		"uniform vec3 u_Ld;" \
		"uniform vec3 u_Kd;" \
		"uniform vec4 u_light_position;" \
		"out vec3 diffuse_light;" \
		"out vec2 out_texture0_coord;" \
		"void main(void)" \
		"{" \
		"if (u_LKeyPressed == 1)" \
		"{" \
		"vec4 eyeCoordinates = u_model_view_matrix * vPosition;" \
		"vec3 tnorm = normalize(mat3(u_model_view_matrix) * vNormal);" \
		"vec3 s = normalize(vec3(u_light_position - eyeCoordinates));" \
		"diffuse_light = u_Ld * u_Kd * max(dot(s, tnorm), 0.0);" \
		"}" \
		"gl_Position = u_projection_matrix * u_model_view_matrix * vPosition;" \
		"out_texture0_coord = vTexture0_Coord;"\
		"}";

	glShaderSource(gVertexShaderObject, 1, (const GLchar**)&vertexShaderSourceCode, NULL);

	// compile shader
	glCompileShader(gVertexShaderObject);
	GLint iInfoLogLength = 0;
	GLint iShaderCompiledStatus = 0;
	char* szInfoLog = NULL;
	glGetShaderiv(gVertexShaderObject, GL_COMPILE_STATUS, &iShaderCompiledStatus);
	if (iShaderCompiledStatus == GL_FALSE)
	{
		glGetShaderiv(gVertexShaderObject, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		if (iInfoLogLength > 0)
		{
			szInfoLog = (char*)malloc(iInfoLogLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(gVertexShaderObject, iInfoLogLength, &written, szInfoLog);
				fprintf(gpFile, "Vertex Shader Compilation Log : %s\n", szInfoLog);
				free(szInfoLog);
				uninitialize();
				exit(0);
			}
		}
	}

	// *** FRAGMENT SHADER ***
	// create shader
	gFragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	// provide source code to shader
	const GLchar* fragmentShaderSourceCode =
		"#version 130" \
		"\n" \
		"in vec3 diffuse_light;" \
		"in vec2 out_texture0_coord;" \
		"out vec4 FragColor;" \
		"uniform int u_LKeyPressed;" \
		"uniform sampler2D u_texture0_sampler;"\
		"void main(void)" \
		"{" \
		"vec4 color;" \
		"if (u_LKeyPressed == 1)" \
		"{" \
		"color = vec4(diffuse_light,1.0);" \
		"}" \
		"else" \
		"{" \
		"color = vec4(1.0, 1.0, 1.0, 1.0);" \
		"}" \
		"FragColor = color * texture(u_texture0_sampler, out_texture0_coord);" \
		"}";

	glShaderSource(gFragmentShaderObject, 1, (const GLchar**)&fragmentShaderSourceCode, NULL);

	// compile shader
	glCompileShader(gFragmentShaderObject);
	glGetShaderiv(gFragmentShaderObject, GL_COMPILE_STATUS, &iShaderCompiledStatus);
	if (iShaderCompiledStatus == GL_FALSE)
	{
		glGetShaderiv(gFragmentShaderObject, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		if (iInfoLogLength > 0)
		{
			szInfoLog = (char*)malloc(iInfoLogLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(gFragmentShaderObject, iInfoLogLength, &written, szInfoLog);
				fprintf(gpFile, "Fragment Shader Compilation Log : %s\n", szInfoLog);
				free(szInfoLog);
				uninitialize();
				exit(0);
			}
		}
	}

	// *** SHADER PROGRAM ***
	// create
	gShaderProgramObject = glCreateProgram();

	// attach vertex shader to shader program
	glAttachShader(gShaderProgramObject, gVertexShaderObject);

	// attach fragment shader to shader program
	glAttachShader(gShaderProgramObject, gFragmentShaderObject);

	// pre-link binding of shader program object with vertex shader position attribute
	glBindAttribLocation(gShaderProgramObject, VDG_ATTRIBUTE_VERTEX, "vPosition");
	glBindAttribLocation(gShaderProgramObject, VDG_ATTRIBUTE_NORMAL, "vNormal");
	glBindAttribLocation(gShaderProgramObject, VDG_ATTRIBUTE_TEXTURE0, "vTexture0_Coord");

	// link shader
	glLinkProgram(gShaderProgramObject);
	GLint iShaderProgramLinkStatus = 0;
	glGetProgramiv(gShaderProgramObject, GL_LINK_STATUS, &iShaderProgramLinkStatus);
	if (iShaderProgramLinkStatus == GL_FALSE)
	{
		glGetProgramiv(gShaderProgramObject, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		if (iInfoLogLength > 0)
		{
			szInfoLog = (char*)malloc(iInfoLogLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetProgramInfoLog(gShaderProgramObject, iInfoLogLength, &written, szInfoLog);
				fprintf(gpFile, "Shader Program Link Log : %s\n", szInfoLog);
				free(szInfoLog);
				uninitialize();
				exit(0);
			}
		}
	}

	// get uniform locations
	gModelViewMatrixUniform = glGetUniformLocation(gShaderProgramObject, "u_model_view_matrix");
	gProjectionMatrixUniform = glGetUniformLocation(gShaderProgramObject, "u_projection_matrix");

	gLKeyPressedUniform = glGetUniformLocation(gShaderProgramObject, "u_LKeyPressed");

	gLdUniform = glGetUniformLocation(gShaderProgramObject, "u_Ld");
	gKdUniform = glGetUniformLocation(gShaderProgramObject, "u_Kd");
	gLightPositionUniform = glGetUniformLocation(gShaderProgramObject, "u_light_position");

	gTextureSamplerUniform = glGetUniformLocation(gShaderProgramObject, "u_texture0_sampler");

	// *** vertices, colors, shader attribs, vbo, vao initializations ***

	processVertexData();
	processNormalsData();
	processTextureData();

	//processObjData();

	// CUBE CODE
	// vao
	glGenVertexArrays(1, &gVao_cube);
	glBindVertexArray(gVao_cube);

	// position vbo
	glGenBuffers(1, &gVbo_cube_position);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_cube_position);
	glBufferData(GL_ARRAY_BUFFER, (fSize * 3 * 3 * 4), vertexArray, GL_STATIC_DRAW); //432 for cube = 12*3*3*4  // 36 for triangle // monkey head 968*3*3*4 = 34848



	glVertexAttribPointer(VDG_ATTRIBUTE_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(VDG_ATTRIBUTE_VERTEX);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// normal vbo
	glGenBuffers(1, &gVbo_cube_normal);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_cube_normal);
	glBufferData(GL_ARRAY_BUFFER, (fSize * 3 * 3 * 4), normalsArray, GL_STATIC_DRAW);

	glVertexAttribPointer(VDG_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(VDG_ATTRIBUTE_NORMAL);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	// texture vbo
	glGenBuffers(1, &gVbo_cube_texture);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_cube_texture);
	glBufferData(GL_ARRAY_BUFFER, (fSize * 3 * 2 * 4), textureArray, GL_STATIC_DRAW);

	glVertexAttribPointer(VDG_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(VDG_ATTRIBUTE_TEXTURE0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	gVao_terrain = loadToVAO(newTerrain->getVertices(), newTerrain->getNormals(), newTerrain->getTextureCoords(), newTerrain->getIndices());

	//cleanUp(); // free heap memory after pushing data to the GPU

	glShadeModel(GL_SMOOTH);
	// set-up depth buffer
	glClearDepth(1.0f);
	// enable depth testing
	glEnable(GL_DEPTH_TEST);
	// depth test to do
	glDepthFunc(GL_LEQUAL);
	// set really nice percpective calculations ?
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	// We will always cull back faces for better performance
	//glEnable(GL_CULL_FACE); //////////////////////////////////COMMENTED

	texture_MonkeyHead = new Texture(gTexture_Stone, MAKEINTRESOURCE(IDBITMAP_STONE));
	texture_MonkeyHead->LoadGLTextures(); // add error check here FE - future enhancements
	//LoadGLTextures(&gTexture_Kundali, MAKEINTRESOURCE(IDBITMAP_KUNDALI));
	//LoadGLTextures(&gTexture_Stone, MAKEINTRESOURCE(IDBITMAP_STONE));
	texture_terrain = new Texture(gTexture_terrain, MAKEINTRESOURCE(IDBITMAP_GRASS));
	texture_terrain->LoadGLTextures();

	// set background color
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // black

	// set perspective matrix to identitu matrix
	gPerspectiveProjectionMatrix = mat4::identity();
	gOrthographicProjectionMatrix = mat4::identity();

	gbAnimate = false;
	gbLight = false;

	// resize
	resize(WIN_WIDTH, WIN_HEIGHT);
}



void createShaderProgram(const GLchar* vShader, const GLchar* fShader)
{
	void bindAttributes(GLuint attribute, const char* varName);
	void getAllUniformLocations(void);
	GLuint loadShader(const GLchar * shaderSource, GLenum type);

	gVertexShaderObjectTerrain = loadShader(vShader, GL_VERTEX_SHADER);
	gFragmentShaderObjectTerrain = loadShader(fShader, GL_FRAGMENT_SHADER);
	gShaderProgramObjectTerrain = glCreateProgram();

	glAttachShader(gShaderProgramObjectTerrain, gVertexShaderObjectTerrain);
	glAttachShader(gShaderProgramObjectTerrain, gFragmentShaderObjectTerrain);

	bindAttributes(0, "position");
	bindAttributes(1, "textureCoordinates");
	bindAttributes(2, "normal");


	glLinkProgram(gShaderProgramObjectTerrain);
	glValidateProgram(gShaderProgramObjectTerrain);

	getAllUniformLocations();

}


void bindAttributes(GLuint attribute, const char* varName)
{
	glBindAttribLocation(gShaderProgramObjectTerrain, attribute, varName);
}


void getAllUniformLocations(void)
{
	location_transformationMatrix = glGetUniformLocation(gShaderProgramObjectTerrain, "transformationMatrix");
	location_projectionMatrix = glGetUniformLocation(gShaderProgramObjectTerrain, "projectionMatrix");
	location_viewMatrix = glGetUniformLocation(gShaderProgramObjectTerrain, "viewMatrix");
	location_lightPosition = glGetUniformLocation(gShaderProgramObjectTerrain, "lightPosition");
	location_lightColour = glGetUniformLocation(gShaderProgramObjectTerrain, "lightColour");
	location_shineDamper = glGetUniformLocation(gShaderProgramObjectTerrain, "shineDamper");
	location_reflectivity = glGetUniformLocation(gShaderProgramObjectTerrain, "reflectivity");
	location_sampler = glGetUniformLocation(gShaderProgramObjectTerrain, "modelTexture");
}

GLuint loadShader(const GLchar* shaderSource, GLenum type)
{
	void uninitialize(void);

	GLuint shaderId = glCreateShader(type);
	glShaderSource(shaderId, 1, (const GLchar**)&shaderSource, NULL);
	glCompileShader(shaderId);
	GLint iInfoLogLength = 0;
	GLint iShaderCompiledStatus = 0;
	char* szInfoLog = NULL;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &iShaderCompiledStatus);
	if (iShaderCompiledStatus == GL_FALSE)
	{
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		if (iInfoLogLength > 0)
		{
			szInfoLog = (char*)malloc(iInfoLogLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(shaderId, iInfoLogLength, &written, szInfoLog);
				fprintf(gpFile, "Terrain shader compilation log : %s\n", szInfoLog);
				free(szInfoLog);
				uninitialize();
				exit(0);
			}
		}
		
	}
	return shaderId;
}


GLuint loadToVAO(GLfloat* vArray, GLfloat* nArray, GLfloat* tArray, int* indices)
{
	GLuint createVAO(void);
	void bindIndicesBuffer(int* indices);
	void storeDataInAttributeList(GLuint attribNumber, GLuint coordinateSize, float* data);
	void unbindVAO(void);

	GLuint vaoID = createVAO();
	bindIndicesBuffer(indices);
	storeDataInAttributeList(0, 3, vArray);
	storeDataInAttributeList(1, 2, tArray);
	storeDataInAttributeList(2, 3, nArray);
	unbindVAO();



	return vaoID;
}

GLuint createVAO(void)
{
	GLuint vaoID = 0;
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);
	return vaoID;
}

void unbindVAO(void) {
	glBindVertexArray(0);
}

void bindIndicesBuffer(int* indices)
{
	GLuint vboID = 0;
	int tempSize = newTerrain->getIndexSize();
	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (tempSize * sizeof(int)), indices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (tempSize * sizeof(int)), indices, GL_STATIC_DRAW);
}

void storeDataInAttributeList(GLuint attribNumber, GLuint coordinateSize, float* data)
{
	GLuint vboId = 0;
	int tempSize = newTerrain->getFaceSize();
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	if (coordinateSize == 2) // for texture coords
		glBufferData(GL_ARRAY_BUFFER, (tempSize * 2 * sizeof(GLfloat)), data, GL_STATIC_DRAW);
	else // for vertiex and normals
		glBufferData(GL_ARRAY_BUFFER, (tempSize * 3 * sizeof(GLfloat)), data, GL_STATIC_DRAW);
	glVertexAttribPointer(attribNumber, coordinateSize, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}


void prepareTerrain(void)
{
	glBindVertexArray(gVao_terrain);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	// load shine and reflectivity vars
	glUniform1f(location_shineDamper, 1);
	glUniform1f(location_reflectivity, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_terrain->getTextureId());
	glUniform1i(location_sampler, 0);

}

void loadTransformationMatrix(void)
{
	mat4 transformationMatrix = mat4::identity();
	transformationMatrix = translate(newTerrain->getX(), 0.0f, newTerrain->getZ());
	glUniformMatrix4fv(location_transformationMatrix, 1, GL_FALSE, transformationMatrix);
}

void loadViewMatrix(void)
{
	mat4 transformationMatrix = mat4::identity();
	transformationMatrix = translate(newTerrain->getX(), 0.0f, newTerrain->getZ());
	glUniformMatrix4fv(location_transformationMatrix, 1, GL_FALSE, transformationMatrix);
}


void unbindTextureModel(void)
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindVertexArray(0);

}

void renderTerrain(void)
{
	void prepareTerrain(void);
	void loadTransformationMatrix(void);
	void unbindTextureModel(void);
	mat4 projMatrixTerrain = mat4::identity();
	//start rendering
	glUseProgram(gShaderProgramObjectTerrain);
	//load light
	glUniform3f(location_lightPosition, 10.0f, 10.0f, 10.0f);
	glUniform3f(location_lightColour, 1.0f, 1.0f, 1.0f);
	//load view matrix
	glUniformMatrix4fv(location_viewMatrix, 1, GL_FALSE, camera->getLookAtMatrix());
	projMatrixTerrain = perspective(fov, (GLfloat)currentWidth / (GLfloat)currentHeight, 0.1f, 1000.0f);
	glUniformMatrix4fv(location_projectionMatrix, 1, GL_FALSE, projMatrixTerrain);
	prepareTerrain();
	loadTransformationMatrix();
	loadViewMatrix();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	unbindTextureModel();
	glUseProgram(0);
}

void display(void)
{
	//code
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClearColor(0.49f, 89.0f, 0.98f, 1);
	glClearColor(0.0f, 0.0f, 0.0f, 1);

	//// start using OpenGL program object
	//glUseProgram(gShaderProgramObject);

	//if (gbLight == true)
	//{
	//	glUniform1i(gLKeyPressedUniform, 1);

	//	glUniform3f(gLdUniform, 1.0f, 1.0f, 1.0f);
	//	glUniform3f(gKdUniform, 1.0f, 1.0f, 1.0f);//0.5f, 0.5f, 0.5f);

	//	float lightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f };
	//	glUniform4fv(gLightPositionUniform, 1, (GLfloat*)lightPosition);
	//}
	//else
	//{
	//	glUniform1i(gLKeyPressedUniform, 0);
	//}

	//// OpenGL Drawing
	//// set all matrices to identity
	//mat4 modelMatrix = mat4::identity();
	//mat4 viewMatrix = mat4::identity();
	//mat4 modelViewMatrix = mat4::identity();
	//mat4 rotationMatrix = mat4::identity();

	////modelMatrix = scale(10.0, 10.0, 10.0);
	//// apply z axis translation to go deep into the screen by -5.0,
	//// so that triangle with same fullscreen co-ordinates, but due to above translation will look small
	//modelMatrix = translate(0.0f, 0.0f, 0.0f);

	////input from ImGui
 //  //gAngle = ImGuiWrapper->getInputCameraAngle();
	//float radius = 5.0f; // 5.0 MonkeyHead // 50.0 singleAeroplane
	//float y_coord = radius * cos(radians(gAngle));
	//float z_coord = radius * sin(radians(gAngle));

	////this is to calculate tangent OTG. since the camera rotation plane is Y-Z & as per our requirement we want to move around the object keeping Y- as up direction at start
	////this up direction needs to be updated as we move ahead.
	////Hence we calculate tangent vector to the circle by taking cross product of the camera vector(normalized) and positive X axis as right direction. 
	//vec3 f = normalize(vec3(0.0, y_coord, z_coord));
	//vec3 up_direction = cross(f, vec3(1.0, 0.0, 0.0));

	//viewMatrix = camera->getLookAtMatrix();
	////viewMatrix = lookat(cameraPos, cameraPos + cameraFront, cameraUp);
	////viewMatrix = lookat(vec3(0.0, y_coord, z_coord), vec3(0.0, 0.0, 0.0), up_direction);
	////viewMatrix = lookat(vec3(z_coord, y_coord, 0.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0));
	//// all axes rotation by gAngle angle
	//rotationMatrix = rotate(0.0f, gAngle, 0.0f);



	//// multiply rotation matrix and model matrix to get modelView matrix
	////modelViewMatrix = modelMatrix * rotationMatrix; // ORDER IS IMPORTANT
	//modelMatrix = modelMatrix * rotationMatrix;
	//modelViewMatrix = viewMatrix * modelMatrix;
	//ImGuiWrapper->setMatrix(modelViewMatrix);
	//// pass modelview matrix to the vertex shader in 'u_model_view_matrix' shader variable
	//// whose position value we already calculated in initialize() by using glGetUniformLocation()
	//glUniformMatrix4fv(gModelViewMatrixUniform, 1, GL_FALSE, modelViewMatrix);

	//// pass projection matrix to the vertex shader in 'u_projection_matrix' shader variable
	//// whose position value we already calculated in initialize() by using glGetUniformLocation()
	//gPerspectiveProjectionMatrix = perspective(fov, (GLfloat)currentWidth / (GLfloat)currentHeight, 0.1f, 1000.0f);
	//glUniformMatrix4fv(gProjectionMatrixUniform, 1, GL_FALSE, gPerspectiveProjectionMatrix);
	////glUniformMatrix4fv(gProjectionMatrixUniform, 1, GL_FALSE, gOrthographicProjectionMatrix);

	//glActiveTexture(GL_TEXTURE);
	//glBindTexture(GL_TEXTURE_2D, texture_MonkeyHead->getTextureId());
	//glUniform1i(gTextureSamplerUniform, 0);


	//// *** bind vao ***
	//glBindVertexArray(gVao_cube);

	//// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()

	////glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	////glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
	////glDrawArrays(GL_TRIANGLE_FAN, 8, 4);
	////glDrawArrays(GL_TRIANGLE_FAN, 12, 4);
	////glDrawArrays(GL_TRIANGLE_FAN, 16, 4);
	////glDrawArrays(GL_TRIANGLE_FAN, 20, 4);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glDrawArrays(GL_TRIANGLES, 0, (fSize * 3 * 3)); // 9  for triangle 1*3*3 // 108 for cube 12*3*3 // 968*3*3 = 8712 for monkey head

	//// *** unbind vao ***
	//glBindVertexArray(0);

	// //stop using OpenGL program object
	//glUseProgram(0);

	renderTerrain();
	//gAngle++;

}



void updatefov(float yOffset)
{
	if (yOffset != 0.0)
	{
		fov = yOffset;
		if (fov < 1.0)
			fov = 1.0;
		else if (fov > 45.0)
			fov = 45.0;
	}
}

void resize(int width, int height)
{
	//code
	if (height == 0)
		height = 1;
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	currentWidth = (GLfloat)width;
	currentHeight = (GLfloat)height;
	fprintf(gpFile, "size w- %d, h- %d\n", width, height);
	gPerspectiveProjectionMatrix = perspective(fov, (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f);

}

void spin(void)
{
	// code
	//gAngle = gAngle + 1.0f;
	//if (gAngle >= 360.0f)
	//	gAngle = gAngle - 360.0f;
	gAngle = gAngle - 1.0f;
	if (gAngle <= 0.0f)
		gAngle = gAngle + 360.0f;
}

void uninitialize(void)
{
	void cleanUp(void);
	//UNINITIALIZATION CODE
	if (gbFullscreen == true)
	{
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);

		ShowCursor(TRUE);

	}

	if (gVao_terrain)
	{
		glDeleteBuffers(1, &gVao_terrain);
		gVao_terrain = 0;
	}

	

	// destroy vao
	if (gVao_cube)
	{
		glDeleteVertexArrays(1, &gVao_cube);
		gVao_cube = 0;
	}

	// destroy position vbo
	if (gVbo_cube_position)
	{
		glDeleteBuffers(1, &gVbo_cube_position);
		gVbo_cube_position = 0;
	}

	// destroy normal vbo
	if (gVbo_cube_normal)
	{
		glDeleteBuffers(1, &gVbo_cube_normal);
		gVbo_cube_normal = 0;
	}

	if (gShaderProgramObject)
	{
		// detach vertex shader from shader program object
		glDetachShader(gShaderProgramObject, gVertexShaderObject);
		// detach fragment  shader from shader program object
		glDetachShader(gShaderProgramObject, gFragmentShaderObject);

		// delete vertex shader object
		glDeleteShader(gVertexShaderObject);
		gVertexShaderObject = 0;
		// delete fragment shader object
		glDeleteShader(gFragmentShaderObject);
		gFragmentShaderObject = 0;

		// delete shader program object
		glDeleteProgram(gShaderProgramObject);
		gShaderProgramObject = 0;
	}


	if (texture_MonkeyHead)
	{
		free(texture_MonkeyHead);
		texture_MonkeyHead = NULL;
	}
	//Deselect the rendering context
	wglMakeCurrent(NULL, NULL);

	//Delete the rendering context
	wglDeleteContext(ghrc);
	ghrc = NULL;

	//Delete the device context
	ReleaseDC(ghwnd, ghdc);
	ghdc = NULL;

	cleanUp();


	if (gpFile)
	{
		fprintf(gpFile, "Log File Is Successfully Closed.\n");
		fclose(gpFile);
		gpFile = NULL;
	}
}


void cleanUp(void)
{
	if (vertexArray)
	{
		// free up memory allocated using malloc or calloc function
		// free memory used for vertex array
		free(vertexArray);
		vertexArray = NULL;
	}

	if (normalsArray)
	{
		// free memory used for normals array
		free(normalsArray);
		normalsArray = NULL;
	}

	if (textureArray)
	{
		// free memory used for normals array
		free(textureArray);
		textureArray = NULL;
	}

	delete newTerrain;
	newTerrain = NULL;
}