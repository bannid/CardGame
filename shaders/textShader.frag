#version 330 core

in vec2 texCoordsOut;

out vec4 FragColor;

uniform sampler2D ufSampler;
uniform float iTime;
void main() {
	vec4 texColor = texture(ufSampler, texCoordsOut);
    vec3 red = vec3(1.0f, 0.0f, 0.0f) * texColor.w;
	FragColor = vec4(red, texColor.w);
}