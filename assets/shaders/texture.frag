#version 420

in vec2 Texcoord;
out vec4 outColor;

layout(binding = 0) uniform sampler2D tex;
layout(binding = 1) uniform sampler2D tintMask;
uniform vec4 tint = vec4(1.0);

void main()
{
	vec4 mask = texture(tintMask, Texcoord);
	if(mask.r > 0.5) {
		outColor = texture(tex, Texcoord); 
	}
	else {
		outColor = texture(tex, Texcoord) * tint; 
	}
}