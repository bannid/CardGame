#version 330 core

uniform vec3 resolution;
uniform float iTime;
out vec4 fragColor;
void main(){
    vec2 uv = gl_FragCoord.xy / resolution.x;
    // uv.x += sin(iTime) * .1f;
    // uv.y += sin(iTime) * .1f;
    float AR = resolution.x / resolution.y;
    vec2 temp = uv - vec2(.5, .5);
    float disp = sin((temp.y + iTime * .1) * 30.);
    float d = length(vec2(.5, .5 / AR) - uv) - disp * .01;;
    d = d <= 0.3 ? 1. : 0.;
    if(d == 0){
        discard;
        return;
    }
    vec3 col = vec3(d);
    // Output to screen
    fragColor = vec4(col,.3f + disp * 0.06f);
}