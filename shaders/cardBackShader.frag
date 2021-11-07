#version 330 core

in vec2 texCoordsOut;

out vec4 FragColor;
uniform float iTime;
void main() {
    float k = smoothstep(0.0, 0.2, (sin(texCoordsOut.x * 30.0f) + 1.0f) * .5f);
    float k2 = smoothstep(0.0, 0.2, (sin(texCoordsOut.y * 30.0f) + 1.0f) * .5f);
	vec4 texColor = vec4(k);
    texColor += vec4(k2);
    vec3 color = vec3(1.0f, 0.3f, 0.3f);
	FragColor = vec4(texColor.xyz * color, 1.0f);
} 