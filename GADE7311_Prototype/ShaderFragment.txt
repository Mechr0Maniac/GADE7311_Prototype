#version 330 core
out vec4 FragColour;

in vec3 myColour;
in vec2 textures;

uniform sampler2D myTex;

void main()
{
    FragColour = texture(myTex, textures);
}