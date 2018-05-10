#version 330 core

in vec2 fragUV;
out vec4 color;

uniform sampler2D text;
uniform vec3 tint;

void main()
{    
    color = vec4(tint, 1.0) * texture(text, fragUV);
}  