#version 330 core

in vec2 texCoordsOut;

out vec4 FragColor;

uniform sampler2D ufSampler;
void main() {
	vec4 texColor = texture(ufSampler, texCoordsOut);
	FragColor = pow(texColor, vec4(1.0f));
}