#version 150 core

// Camera
uniform vec3 cameraPosition;

// Material settings
uniform struct Material
{
   sampler2D texture_diffuse;
   sampler2D texture_specular;
   sampler2D texture_emission;
   sampler2D texture_normal;
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

} allLights[MAX_LIGHTS];

// Model
in vec3 fragVert;
in vec2 fragTexCoord;
in vec3 fragNormal;

in mat3 fragTBN;

uniform mat4 model;

// Shadow
uniform sampler2D shadowMap;
in vec4 fragPosLightSpace;

// Out color
out vec4 finalColor;

float ShadowCalculation(vec3 normal, vec3 lightDir)
{
	// Perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 

    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

    // Check whether current frag pos is in shadow
    float bias = max(0.01 * (1.0 - dot(normal, lightDir)), 0.005);
	
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
		}    
	}
	shadow /= 9.0;

	if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

vec3 ApplyLight(Light light, vec3 normal, vec3 surfacePos, vec3 surfaceToCamera, vec3 diffuseMap, vec3 specularMap)
{
    float attenuation = 1.0;
    vec3 surfaceToLight = normalize(light.position.xyz);;

	// Point light
    if(light.position.w == 1.0)
    {
        surfaceToLight = normalize(light.position.xyz - surfacePos);
        float distanceToLight = length(light.position.xyz - surfacePos);
        attenuation = 1.0 / (1.0 + light.attenuation * pow(distanceToLight, 2));
    }

    // Ambient
    vec3 ambient = light.ambientCoefficient * diffuseMap.rgb * light.color;

    // Diffuse
    float diff = max(0.0, dot(normal, surfaceToLight));
    vec3 diffuse = diff * diffuseMap.rgb * light.color;

    // Specular
    float spec = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, normal))), material.shininess);
    vec3 specular = spec * specularMap.rgb * light.color;

	float shadow = ShadowCalculation(normal, surfaceToLight);
    return attenuation * ambient + (1.0 - shadow) * (attenuation * ( diffuse + specular));
}

void main()
{
    vec3 normal = normalize(transpose(inverse(mat3(model))) * fragNormal);
    vec3 surfacePos = fragVert;
    vec3 surfaceToCamera = normalize(cameraPosition - surfacePos);

    vec4 diffuseMap = texture(material.texture_diffuse, fragTexCoord);
    vec3 specularMap = texture(material.texture_specular, fragTexCoord).rgb;
    vec3 emissionMap = texture(material.texture_emission, fragTexCoord).rgb;

	// obtain normal from normal map in range [0,1]
    normal = texture(material.texture_normal, fragTexCoord).rgb;
    // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);   
	normal = normalize(fragTBN * normal); 

	// Emission
    vec3 linearColor = emissionMap;

    for(int i = 0; i < numLights; ++i)
    {
        linearColor += ApplyLight(allLights[i], normal, surfacePos, surfaceToCamera, diffuseMap.rgb, specularMap);
    }
    
    // Final color (after gamma correction)
    vec3 gamma = vec3(1.0/2.2);
    finalColor = vec4(pow(linearColor, gamma), diffuseMap.a);
}