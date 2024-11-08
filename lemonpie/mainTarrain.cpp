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
#define STB_IMAGE_IMPLEMENTATION
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
#include "Display.h"
#include "MasterRenderer.h"

#define ENABLE_CAMERA_YAW_ROTATION 1
#define ENABLE_CAMERA_PITCH_ROTATION 1

#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"opengl32.lib")

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

using namespace vmath;

#define OBJ_FILE_PATH "3DModels\\tree.obj" //MonkeyHead.obj"// //MonkeyHead.obj" //singleAeroplane.obj" MonkeyHead.obj


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


Light sun(vec3(1000.0f, 1000.0f, 1000.0f), vec3(1.0, 1.0, 1.0));
//terrain
GLuint gTexture_terrain;
Texture* texture_terrain;
Loader terrainLoader;
RawModel* terrainModel;
Terrain* terrain1;
Terrain* terrain2;
TerrainShader* terrainShader;
TerrainRenderer* terrainRenderer;

//entity
Loader treeLoader;
RawModel* treeModel;
Texture* texture_tree;
StaticShader* staticShader;
EntityRenderer* entityRenderer;
TexturedModel* texturedModelTree;
std::list<Entity> entities;
//*****************
//master renderer
MasterRenderer* renderer;


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

float fov = 70.0f;
CameraControl* camera;

#ifdef IMGUI_WRAPPER_CLASS
ImGui_Wrapper* ImGuiWrapper;
ImGuiContext* ImGui_ctx;
float deltaTime;
float xMouseOffset;
float yMouseOffset;
ImVec2 mouseOffset = { 0.0,0.0 }; // ImGui datatype
#endif


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
	//monkeyModel = OBJLoader::loadObjModel(OBJ_FILE_PATH, monkeyLoader);
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

	camera = new CameraControl(0.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

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

	renderer->cleanup();
	terrainLoader.cleanUp();
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
	terrain1 = new Terrain(0, 0, terrainLoader);
	terrain2 = new Terrain(-1, 0, terrainLoader);

	/*terrainShader = new TerrainShader("Shaders/terrain.vs", "Shaders/terrain.fs");
	terrainRenderer = new TerrainRenderer(terrainShader, gPerspectiveProjectionMatrix);*/
	//LoadGLTextures(&gTexture_Kundali, MAKEINTRESOURCE(IDBITMAP_KUNDALI));
	//LoadGLTextures(&gTexture_Stone, MAKEINTRESOURCE(IDBITMAP_STONE));
	/*texture_terrain = new Texture(MAKEINTRESOURCE(IDBITMAP_GRASS));
	if (texture_terrain->LoadGLTextures())
	{
		texture loaded successfully
	}*/

	// tree setup
	treeModel = OBJLoader::loadObjModel(OBJ_FILE_PATH, treeLoader);
	//staticShader = new StaticShader("Shaders/staticVertShader.vs", "Shaders/staticFragShader.fs");
	//entityRenderer = new EntityRenderer(staticShader, gPerspectiveProjectionMatrix);
	texture_tree = new Texture(MAKEINTRESOURCE(IDBITMAP_TREE));
	if (texture_tree->LoadGLTextures_stb("tree.png"))
	{
		// texture successfully loaded
	}		
	texturedModelTree = new TexturedModel(treeModel, texture_tree);

	for(int i =0; i<100;i++)
	{
		float x_coord = (float)(rand()%100 -50);
		float z_coord = (float)(rand() % 100 - 100);
		entities.push_back(Entity(texturedModelTree, vec3(x_coord, 0.0f, z_coord), 0.0f, 0.0f, 0.0f, 1.0f));
	}

	//entities.push_back(Entity(texturedModelTree, vec3(3.0f, 0.0f, -10.0f), 0.0f, 0.0f, 0.0f, 1.0f));
	//entities.push_back(Entity(texturedModelTree, vec3(0.0f, 2.0f, -10.0f), 0.0f, 0.0f, 0.0f, 1.0f));
	renderer = new MasterRenderer();



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


void renderEntityTest(void)
{
	renderer->processTerrain(terrain1);
	renderer->processTerrain(terrain2);
	

	for (Entity entity : entities)
	{
		renderer->processEntity(entity);
	}
	renderer->render(sun, camera);

	//mat4 projMatrixTree = mat4::identity();
	//Light lightForTree(vec3(1000.0f, 1000.0f, 1000.0f), vec3(1.0, 1.0, 1.0));
	//staticShader->start();
	//staticShader->loadLight(lightForTree);
	//projMatrixTree = perspective(fov, (GLfloat)currentWidth / (GLfloat)currentHeight, 0.1f, 1000.0f);
	//staticShader->loadProjectionMatrix(projMatrixTree);
	//staticShader->loadViewMatrix(camera);
	//entityRenderer->render(texturedModelTree);
	//staticShader->stop();
}

void display(void)
{
	//code
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.49f, 89.0f, 0.98f, 1);
	
	renderEntityTest();

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
	Display::winWidth = width;
	Display::winHeight = height;
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

	if (terrain1)
	{
		delete terrain1;
		terrain1 = NULL;

	}

	if (terrain2)
	{
		delete terrain2;
		terrain2 = NULL;

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

	if (renderer)
		delete renderer;
}
