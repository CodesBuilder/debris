#version 330 core

layout (location = 0) in vec4 pos;
layout (location = 1) in vec2 texturePos;
layout (location = 2) in vec3 rot;
uniform vec2 scale;
uniform vec2 origin;

out vec2 textureCoord;

void main()
{
	gl_Position = vec4(
		origin.x + (pos.x * cos(angle) - pos.y * sin(angle)) * scale.x,
		origin.y + (pos.x * sin(angle) + pos.y * cos(angle)) * scale.y,
		pos.z,
		1.0f);
	textureCoord = texturePos;
}
