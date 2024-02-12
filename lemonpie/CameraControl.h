#pragma once
#include "vmath.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"

// Camera movement options possible with key press
enum cameraMovements
{
	FORWARD,
	BACKWARD,
	RIGHT,
	LEFT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class CameraControl
{
private:
	vmath::vec3 cameraPos;
	vmath::vec3 cameraFront;
	vmath::vec3 cameraUp;
	float cameraYaw;
	float cameraPitch;
	float cameraSpeed;
	float prevXMouseOffset;
	float prevYMouseOffset;
	float sensitivity;
	float fov;
	vmath::mat4 lookAtMatrix;
	vmath::mat4 perspectiveProjMatrix;


public:
	CameraControl(float posX, float posY, float posZ, float tragetX, float tragetY, float tragetZ, float upDirX, float upDirY, float upDirZ);
	~CameraControl();
	void processKeyInputs(cameraMovements dir, float Offset); // camera position 
	void processMouseDrag(ImVec2 mouseOffset); // for Yaw and pitch
	void processMouseWheel(float Offset); // for zoom-in /zoom-out
	void updatefov(float Offset);
	vmath::mat4 getLookAtMatrix(void);
	vmath::mat4 getPerspectiveProjMatrix(void);

};

