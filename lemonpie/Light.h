#pragma once
#include"vmath.h"

using namespace vmath;
class Light
{
private:
	vec3 position;
	vec3 color;

public:
	Light(vec3 position, vec3 color);
	vec3 getColor(void);
	vec3 getPosition(void);

	void setPosition(vec3 position);
	void setColor(vec3 color);


};

