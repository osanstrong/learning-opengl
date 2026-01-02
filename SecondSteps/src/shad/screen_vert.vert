#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

uniform float u_shift;
uniform float u_shiftY;

void main()
{
    float new_x = aPos.x + u_shift;
    gl_Position = vec4(vec2(aPos + vec2(u_shift, u_shiftY)), 0.0, 1.0); 
    TexCoords = aTexCoords;
} 