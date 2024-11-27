#version 400

in vec3 textureCoords;
out vec4 FragColor;

uniform samplerCube cubeMapTexture; 

void main()
{
    FragColor = texture(cubeMapTexture, textureCoords); //vec4(1.0,0.0,0.0,1.0);//
}