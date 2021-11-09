#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoords;

out vec2 texCoordsOut;
uniform mat4 uProjectionMat;
uniform mat4 uModelMat;
uniform mat4 uCamMat;
void main() {
	gl_Position = uProjectionMat * uModelMat * vec4(pos.x, pos.y, pos.z, 1.0f);
	texCoordsOut = texCoords;
}