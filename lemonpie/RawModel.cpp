#include "RawModel.h"

RawModel::RawModel(GLuint vaoID, int vertexCount)
{
    this->vaoID = vaoID;
    this->vertexCount = vertexCount;
    this->faceCount = vertexCount / 3;
}

GLuint RawModel::getVaoID()
{
    return vaoID;
}


int RawModel::getVertexCount(void)
{
    return vertexCount;
}

int RawModel::getFaceCount(void)
{
    return faceCount;
}
