#pragma once
#include "TexturedModel.h"
#include "vmath.h"
class Entity
{
private:
    TexturedModel* model;
    vmath::vec3 position;
    float rotX, rotY, rotZ;
    float scale;

public:
    Entity(TexturedModel* model, vmath::vec3 position, float rotX, float rotY, float rotZ, float scale);
    void increasePosition(float dx, float dy, float dz);
    void increaseRotation(float dx, float dy, float dz);
    TexturedModel* getModel(void);
    void setModel(TexturedModel* model);
    vmath::vec3 getPosition(void);
   
    void setPosition(vmath::vec3 position);
    float getRotX(void);
    void setRotX(float rotX);
    float getRotY(void);
    void setRotY(float rotY);
    float getRotZ(void);
    void setRotZ(float rotZ);
    float getScale(void);
    void setScale(float scale);
};

