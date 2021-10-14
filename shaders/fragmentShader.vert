#version 330 core

in vec2 texCoordsOut;

out vec4 FragColor;

uniform sampler2D sampler;

void main() {
	FragColor = texture(sampler, texCoordsOut);
}