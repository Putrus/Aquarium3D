#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube textureMap0;

void main()
{    
    FragColor = texture(textureMap0, TexCoords);
}