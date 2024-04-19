#pragma once
#include "Display.h"


float Display::getAspectRatio(void)
{
	float aspectRatio;
	aspectRatio = (float)winWidth / (float)winHeight;
	return (aspectRatio);
}

float Display::getWidth(void)
{
	return ((float)winWidth);
}

float Display::getHeight(void)
{
	return ((float)winHeight);
}
