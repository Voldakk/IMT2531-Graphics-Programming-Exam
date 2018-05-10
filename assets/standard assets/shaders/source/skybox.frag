#version 150

uniform samplerCube skytexture;

in vec3 fragTexCoord;

out vec4 finalColor;

void main()
{
	finalColor = texture(skytexture, fragTexCoord); 
}