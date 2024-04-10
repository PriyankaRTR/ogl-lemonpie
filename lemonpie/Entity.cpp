#include "Entity.h"


Entity:: Entity(TexturedModel* model, vmath::vec3 position, float rotX, float rotY, float rotZ, float scale) {
    this->model = model;
    this->position = position;
    this->rotX = rotX;
    this->rotY = rotY;
    this->rotZ = rotZ;
    this->scale = scale;
}

void Entity::increasePosition(float dx, float dy, float dz) {
    this->position[0] += dx;
    this->position[1] += dy;
    this->position[2] += dz;
}

void Entity::increaseRotation(float dx, float dy, float dz) {
    this->rotX += dx;
    this->rotY += dy;
    this->rotZ += dz;
}

TexturedModel* Entity::getModel() {
    return this->model;
}

void Entity::setModel(TexturedModel* model) {
    this->model = model;
}

vmath::vec3 Entity::getPosition() {
    return this->position;
}

void Entity::setPosition(vmath::vec3 position) {
    this->position = position;
}

float Entity::getRotX() {
    return this->rotX;
}

void Entity::setRotX(float rotX) {
    this->rotX = rotX;
}

float Entity::getRotY() {
    return this->rotY;
}

void Entity::setRotY(float rotY) {
    this->rotY = rotY;
}

float Entity::getRotZ() {
    return this->rotZ;
}

void Entity::setRotZ(float rotZ) {
    this->rotZ = rotZ;
}

float Entity::getScale() {
    return this->scale;
}

void Entity::setScale(float scale) {
    this->scale = scale;
}

