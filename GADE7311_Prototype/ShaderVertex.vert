#version 330 core
layout (location = 0) in vec3 thisPos;
layout (location = 1) in vec3 thisColour;
layout (location = 2) in vec2 thisTexture;

out vec3 myColour;
out vec2 textures;

uniform mat4 object;
uniform mat4 camera;
uniform mat4 perspective;

void main()
{
   gl_Position = perspective * camera * object * vec4(thisPos, 1.0);
   myColour = thisColour;
   textures = thisTexture;
}