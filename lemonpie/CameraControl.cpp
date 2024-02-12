#include "CameraControl.h"




CameraControl::CameraControl(float posX, float posY, float posZ, float tragetX, float tragetY, float tragetZ, float upDirX, float upDirY, float upDirZ)
{
	cameraPos = vmath::vec3(posX, posY, posZ);
	cameraFront = vmath::vec3(tragetX, tragetY, tragetZ);
	cameraUp = vmath::vec3(upDirX, upDirY, upDirZ);
	cameraYaw = -90.0f;
	cameraPitch = 0.0;
	cameraSpeed = 10.0f;
	sensitivity = 0.005f;
	fov = 45.0f;
}

CameraControl::~CameraControl()
{

}


void CameraControl::processKeyInputs(cameraMovements dir, float deltaTime)
{
	switch (dir)
	{
		case FORWARD:
			cameraPos += (cameraSpeed * deltaTime) * cameraFront;
			break;

		case BACKWARD:
			cameraPos -= (cameraSpeed * deltaTime) * cameraFront;
			break;

		case RIGHT:
			cameraPos += vmath::normalize(vmath::cross(cameraFront, cameraUp)) * (cameraSpeed * deltaTime);
			break;

		case LEFT:
			cameraPos -= vmath::normalize(vmath::cross(cameraFront, cameraUp)) * (cameraSpeed * deltaTime);
			break;

		default:
			break;
	}

}

void CameraControl::processMouseDrag(ImVec2 mouseOffset) // ImGui datatype)
{
	// ref. https://learnopengl.com/Getting-started/Camera
	// Note that we multiply the offset values by a sensitivity value.If we omit this multiplication the mouse movement would be way too strong; 
	// fiddle around with the sensitivity value to your liking.
	mouseOffset.x *= sensitivity;
	mouseOffset.y *= sensitivity;
	mouseOffset.y = -mouseOffset.y;

	cameraYaw += mouseOffset.x;
	cameraPitch += mouseOffset.y;

	if (cameraPitch > 89.0f)
		cameraPitch = 89.0f;
	if (cameraPitch < -89.0f)
		cameraPitch = -89.0f;
	//fprintf(gpFile, "Yaw: %f, Pitch: %f \n", cameraYaw, cameraPitch);
	vmath::vec3 direction;
	direction[0] = cos(vmath::radians(cameraYaw)) * cos(vmath::radians(cameraPitch));
	direction[1] = sin(vmath::radians(cameraPitch));
	direction[2] = sin(vmath::radians(cameraYaw)) * cos(vmath::radians(cameraPitch));

	cameraFront = normalize(direction);
}

void CameraControl::processMouseWheel(float Offset)
{
	updatefov(Offset);


}

vmath::mat4 CameraControl::getLookAtMatrix(void)
{

	lookAtMatrix = lookat(cameraPos, cameraPos + cameraFront, cameraUp);
	return lookAtMatrix;
}

vmath::mat4 CameraControl::getPerspectiveProjMatrix(void)
{

	return perspectiveProjMatrix;
}


void CameraControl::updatefov(float Offset)
{
	if (Offset != 0.0)
	{
		fov = Offset; // yOffset
		if (fov < 1.0)
			fov = 1.0;
		else if (fov > 45.0)
			fov = 45.0;
	}
	
}