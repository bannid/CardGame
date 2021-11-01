#version 330 core

in vec2 texCoordsOut;

out vec4 FragColor;
uniform float iTime;
void main() {
	vec4 texColor = vec4(texCoordsOut.x);
	FragColor = texColor;
} 