#include "Light.h"

Light::Light(vec3 position, vec3 color)
{
	this->position = position;
	this->color = color;
}

vec3 Light::getColor(void)
{
	return color;
}

vec3 Light::getPosition(void)
{
	return position;
}

void Light::setPosition(vec3 position)
{
	this->position = position;
}

void Light::setColor(vec3 color)
{
	this->color = color;
}
