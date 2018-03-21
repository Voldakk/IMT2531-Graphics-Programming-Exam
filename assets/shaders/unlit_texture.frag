#version 150

uniform sampler2D texture_diffuse1;

in vec2 fragTexCoord;

out vec4 finalColor;

void main()
{
	finalColor = texture(texture_diffuse1, fragTexCoord); 
}