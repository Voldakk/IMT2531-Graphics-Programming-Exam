#version 150

uniform samplerCube skytexture;

uniform vec4 skyTint;

in vec3 fragTexCoord;

out vec4 finalColor;

void main()
{
	finalColor = texture(skytexture, fragTexCoord) * skyTint; 
}