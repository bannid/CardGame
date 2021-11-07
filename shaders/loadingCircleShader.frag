#version 330 core

#define EPS 0.002
#define PI 3.141592653589793238


in vec2 texCoordsOut;
out vec4 FragColor;

uniform float iTime;
uniform float timeLeftNormalized;
uniform sampler2D ufSampler;

float angleBwTwoVectors(vec2 first, vec2 second){
    float angle = acos(dot(first, second));
    if(second.y < 0. && second.x < 0.){
       angle = 2. * PI - angle;
    }
    else if(second.y < 0. && second.x > 0.){
        angle = 2. * PI - angle;
    }
    return angle;
}

void main()
{
    float timeLeft = 2. * PI * min(1.0f, timeLeftNormalized);
    vec2 uv = texCoordsOut - vec2(.5f, .5f);
    float d = length(uv);
    vec2 right = normalize(vec2(1.0f, .0f));
    vec2 second = normalize(uv);
    float angle = angleBwTwoVectors(right, second);
    vec4 col = vec4(0.0f);
    float onDisc = d - 0.2;
    float width = 0.005;
    if(angle > timeLeft){
        float alpha = smoothstep(0.1 - width, 0.1, d);
        col = texture(ufSampler, texCoordsOut);
        col.w = 1.0f;
    }
    FragColor = col;
}