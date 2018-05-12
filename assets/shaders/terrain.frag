#version 150 core

// Camera
uniform vec3 cameraPosition;

// Material settings
uniform struct Material
{
   vec4 tint_diffuse;
   float shininess;

} material;

// Lights
#define MAX_LIGHTS 10
uniform int numLights;
uniform struct Light
{
   vec4 position;
   vec3 color;
   float attenuation;
   float ambientCoefficient;
   mat4 lightSpaceMatrix;
   float farPlane;

} allLights[MAX_LIGHTS];

in vec4 allFragPosLightSpace [MAX_LIGHTS];

// Model
in vec3 fragVert;
in vec3 fragColor;
in vec3 fragNormal;
in float fragHeight;
uniform mat4 model;

// Out color
out vec4 finalColor;

// Regions
uniform float regionBlendAmount;
uniform int numRegions;
#define MAX_REGIONS 10
uniform struct Region
{
   float minHeight;
   float maxHeight;
   vec3 color;
} regions[MAX_REGIONS];

uniform bool contourLines;

vec3 ApplyLight(Light light, vec3 normal, vec3 surfacePos, vec3 surfaceToCamera, vec3 diffuseColor, int lightIndex)
{
    float attenuation = 1.0;
    vec3 surfaceToLight = normalize(light.position.xyz);;

	// Point light
    if(light.position.w == 1.0)
    {
        surfaceToLight = normalize(light.position.xyz - surfacePos);
        float distanceToLight = length(light.position.xyz - surfacePos);
        attenuation = 1.0 / (1.0 + light.attenuation * pow(distanceToLight, 4));
    }

    // Ambient
    vec3 ambient = light.ambientCoefficient * diffuseColor * light.color;

    // Diffuse
    float diff = max(0.0, dot(normal, surfaceToLight));
    vec3 diffuse = diff * diffuseColor * light.color;

    // Specular
    float spec = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, normal))), material.shininess);

    return attenuation * ambient + attenuation * diffuse;
}

void main()
{
	vec3 surfaceColor;

	// Regions
	for(int i = 0; i < numRegions; ++i)
    {
		if(fragHeight < regions[i].maxHeight && fragHeight > regions[i].minHeight)
		{		
			float dist = min(abs(regions[i].maxHeight - fragHeight), abs(fragHeight - regions[i].minHeight)) * regionBlendAmount;
			float weight = clamp(dist, 0, 1);
			surfaceColor += regions[i].color * weight;
		}
	}

	if(contourLines)
	{
		for(int i = 0; i < numRegions; ++i)
		{
			if(abs(fragHeight - regions[i].maxHeight) <= 0.001)
				surfaceColor = regions[i].color;
			else if(abs(fragHeight - regions[i].minHeight) <= 0.001)
				surfaceColor = regions[i].color;
		}
	}

    // Normal
    vec3 normal = fragNormal;


	// Lights
	vec3 linearColor = vec3(0);
    vec3 surfaceToCamera = normalize(cameraPosition - fragVert);
    for(int i = 0; i < numLights; ++i)
    {
        linearColor += ApplyLight(allLights[i], normal, fragVert, surfaceToCamera, surfaceColor, i);
    }
    
    // Final color (after gamma correction)
    vec3 gamma = vec3(1.0/2.2);
    finalColor = vec4(pow(linearColor, gamma), 1.0);
}