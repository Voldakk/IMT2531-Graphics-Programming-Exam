#version 330 core

uniform sampler2D texture_diffuse1;

in vec2 uv;
in vec3 diffuse; 

out vec4 outColor;

void main()
{    
     outColor = vec4(texture(texture_diffuse1, uv).rgb * diffuse, 1.0);
}