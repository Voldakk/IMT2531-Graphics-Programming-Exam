#version 330 core
out vec4 color;

in vec2 uv;
uniform sampler2D texture_diffuse1;

void main()
{    
    color = vec4(texture(texture_diffuse1, uv).rgb, 1.0);
}