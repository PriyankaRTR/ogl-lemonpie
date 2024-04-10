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
#include "Loader.h"
#include "RawModel.h"
#include "OBJLoader.h"

#define IMGUI_WRAPPER_CLASS  // need to fix, take expert advice

#ifdef IMGUI_WRAPPER_CLASS
#include "ImGui_Wrapper.h" //Priyanka
#endif
#include "Terrain.h"
#include "TerrainRenderer.h"
//#include "Terrain.h"
#include "Shader.h"
#include "TerrainShader.h"
#include "StaticShader.h"
#include "EntityRenderer.h"

#define ENABLE_CAMERA_YAW_ROTATION 1
#define ENABLE_CAMERA_PITCH_ROTATION 1

#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"opengl32.lib")

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

using namespace vmath;

#define OBJ_FILE_PATH "3DModels\\tree.obj"  //MonkeyHead.obj" //singleAeroplane.obj" MonkeyHead.obj


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





//terrain
Loader terrainLoader;
RawModel* terrainModel;
Terrain* newTerrain;
TerrainShader* terrainShader;
TerrainRenderer* terrainRenderer;

//entity
Loader treeLoader;
RawModel* treeModel;
Texture* texture_tree;
StaticShader* staticShader;
EntityRenderer* entityRenderer;
TexturedModel* texturedModelTree;
//*****************



GLuint gTexture_Kundali;
GLuint gTexture_Stone;

//OBJLoader* objLoader;
Texture* texture_MonkeyHead;
//RawModel MonkeyModel;
GLuint monkeyVao;
Loader monkeyLoader;
RawModel* monkeyModel;
GLuint gLKeyPressedUniform;

mat4 gPerspectiveProjectionMatrix;
mat4 gOrthographicProjectionMatrix;

GLfloat gAngle = 0.0f;

bool gbAnimate;
bool gbLight;

//for obj loading
//std::vector<std::vector<float>> g_vertices;
//std::vector<std::vector<float>> g_texture;
//std::vector<std::vector<float>> g_normals;
//std::vector<std::vector<int>> objIndices;
//std::vector<std::vector<int>> g_face_tri, g_face_texture, g_face_normal;

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



// test

//TerrainShader *terrainShader;
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
		NULL,
		hInstance,
		NULL);

	ghwnd = hwnd;

	ShowWindow(hwnd, iCmdShow);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);



	//objDataLoader();
	//initialize
	initialize();

	// monkey setup
	//objLoader = new ();
	monkeyModel = OBJLoader::loadObjModel(OBJ_FILE_PATH, monkeyLoader);
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
	//glewExperimental = GL_TRUE;
	GLenum glew_error = glewInit();
	if (glew_error != GLEW_OK)
	{
		wglDeleteContext(ghrc);
		ghrc = NULL;
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}


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
	//glBindAttribLocation(gShaderProgramObject, VDG_ATTRIBUTE_VERTEX, "vPosition");
	//glBindAttribLocation(gShaderProgramObject, VDG_ATTRIBUTE_NORMAL, "vNormal");
	//glBindAttribLocation(gShaderProgramObject, VDG_ATTRIBUTE_TEXTURE0, "vTexture0_Coord");

	glBindAttribLocation(gShaderProgramObject, 0, "vPosition");
	glBindAttribLocation(gShaderProgramObject, 2, "vNormal");
	glBindAttribLocation(gShaderProgramObject, 1, "vTexture0_Coord");

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


	// terrain setup
	newTerrain = new Terrain(0, 0, terrainLoader);
	terrainShader = new TerrainShader("Shaders/terrain.vs", "Shaders/terrain.fs");
	terrainRenderer = new TerrainRenderer(terrainShader, gPerspectiveProjectionMatrix);
	texture_MonkeyHead = new Texture(MAKEINTRESOURCE(IDBITMAP_STONE));
	gTexture_Stone = texture_MonkeyHead->LoadGLTextures(); // add error check here FE - future enhancements
	//LoadGLTextures(&gTexture_Kundali, MAKEINTRESOURCE(IDBITMAP_KUNDALI));
	//LoadGLTextures(&gTexture_Stone, MAKEINTRESOURCE(IDBITMAP_STONE));
	texture_terrain = new Texture(MAKEINTRESOURCE(IDBITMAP_GRASS));
	gTexture_terrain = texture_terrain->LoadGLTextures();

	// tree setup
	treeModel = OBJLoader::loadObjModel(OBJ_FILE_PATH, treeLoader);
	staticShader = new StaticShader("Shaders/staticVertShader.vs", "Shaders/staticFragShader.fs");
	entityRenderer = new EntityRenderer(staticShader, gPerspectiveProjectionMatrix);
	texture_tree = new Texture(MAKEINTRESOURCE(IDBITMAP_TREE));
	GLuint gTexture_tree = texture_tree->LoadGLTextures();
	texturedModelTree = new TexturedModel(treeModel, texture_tree);
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


void loadTransformationMatrix(void)
{
	mat4 transformationMatrix = mat4::identity();
	transformationMatrix = translate(newTerrain->getX(), 0.0f, newTerrain->getZ());
	glUniformMatrix4fv(location_transformationMatrix, 1, GL_FALSE, transformationMatrix);
}


void renderTerrainTest(void)
{
	mat4 projMatrixTerrain = mat4::identity();
	Light lightForTarrian(vec3(1000.0f, 1000.0f, 1000.0f), vec3(1.0,1.0,1.0));
	terrainShader->start();
	terrainShader->loadLight(lightForTarrian);
	projMatrixTerrain = perspective(fov, (GLfloat)currentWidth / (GLfloat)currentHeight, 0.1f, 1000.0f);
	terrainShader->loadProjectionMatrix(projMatrixTerrain);
	terrainShader->loadViewMatrix(camera);
	//texture_terrain->bindTexture(0);
	//glUniform1i(location_sampler, 0);
	terrainRenderer->render(newTerrain, texture_terrain);
	terrainShader->stop();
}

void renderEntityTest(void)
{
	mat4 projMatrixTree = mat4::identity();
	Light lightForTree(vec3(1000.0f, 1000.0f, 1000.0f), vec3(1.0, 1.0, 1.0));
	staticShader->start();
	staticShader->loadLight(lightForTree);
	projMatrixTree = perspective(fov, (GLfloat)currentWidth / (GLfloat)currentHeight, 0.1f, 1000.0f);
	staticShader->loadProjectionMatrix(projMatrixTree);
	staticShader->loadViewMatrix(camera);
	entityRenderer->render(texturedModelTree);
	staticShader->stop();
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
	//modelMatrix = translate(0.0f, 0.0f, -5.0f);

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

	//texture_MonkeyHead->bindTexture(0);
	///*glActiveTexture(GL_TEXTURE);
	//glBindTexture(GL_TEXTURE_2D, texture_MonkeyHead->getTextureId());*/
	//glUniform1i(gTextureSamplerUniform, 0);


	//// *** bind vao ***
	//glBindVertexArray(monkeyModel->getVaoID());// gVao_cube);
	//glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);
	//glEnableVertexAttribArray(2);
	//// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()

	////glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	////glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
	////glDrawArrays(GL_TRIANGLE_FAN, 8, 4);
	////glDrawArrays(GL_TRIANGLE_FAN, 12, 4);
	////glDrawArrays(GL_TRIANGLE_FAN, 16, 4);
	////glDrawArrays(GL_TRIANGLE_FAN, 20, 4);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glDrawArrays(GL_TRIANGLES, 0, (968 * 3 * 3)); // 9  for triangle 1*3*3 // 108 for cube 12*3*3 // 968*3*3 = 8712 for monkey head

	//// *** unbind vao ***
	//glBindVertexArray(0);

	// //stop using OpenGL program object
	//glUseProgram(0);

	renderEntityTest();

	renderTerrainTest();
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

	if (monkeyModel)
	{
		delete(monkeyModel);
		monkeyModel = NULL;
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

	if (newTerrain)
	{
		delete newTerrain;
		newTerrain = NULL;

	}

	if (terrainShader)
	{
		delete(terrainShader);
		terrainShader = NULL;
	}


	if (terrainRenderer)
	{
		delete(terrainRenderer);
		terrainRenderer = NULL;
	}

	if (gTexture_terrain)
	{
		glDeleteTextures(1, &gTexture_terrain);
		gTexture_terrain = NULL;
	}
	
	if (texture_terrain)
	{
		free(texture_terrain);
		texture_terrain = NULL;
	}
	if (&terrainLoader)
	{
		terrainLoader.cleanUp();
		//free(&terrainLoader);
		//terrainLoader = NULL;
	}

	if (terrainModel)
	{
		free(terrainModel);
		terrainModel = NULL;
	}



	if (&treeLoader)
	{
		treeLoader.cleanUp();
		//free(&terrainLoader);
		//terrainLoader = NULL;
	}

	if (treeModel)
	{
		free(treeModel);
		treeModel = NULL;
	}
	texture_tree->cleanUp();

	if (texture_tree)
	{
		free(texture_tree);
		texture_tree = NULL;
	}

	if (staticShader)
	{
		free(staticShader);
		staticShader = NULL;
	}

	if (entityRenderer)
	{
		free(entityRenderer);
		entityRenderer = NULL;
	}


	if (texturedModelTree)
	{
		free(texturedModelTree);
		texturedModelTree = NULL;
	}

}
