#version 330 core

uniform vec4 colorIn;
uniform sampler2D textureSampler;

in vec2 textureCoord;

out vec4 colorOut;

void main()
{
	colorOut = texture(textureSampler, textureCoord);
	colorOut *= colorIn;
}
