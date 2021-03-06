#version 330 core

uniform vec3 resolution;
uniform float iTime;
out vec4 fragColor;
void main(){
	// Normalized pixel coordinates (from 0 to 1)
    vec2 uv = gl_FragCoord.xy/resolution.xy;
    vec2 center = vec2(0.5f);
    float t = sin(iTime) * .1f;
    float t2 = cos(iTime) * .5f;
    float intensity = 1. - length(center- (uv + vec2(t * .3f, t2 * .1f)));
    vec3 col = vec3(.2f, .2f, .5f) * intensity;
    // Output to screen
    fragColor = vec4(col, 1.0f);
}