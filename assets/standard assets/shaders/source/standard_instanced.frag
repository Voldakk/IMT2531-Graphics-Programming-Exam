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
   sampler2D shadowMap;
   mat4 lightSpaceMatrix;
   float farPlane;

} allLights[MAX_LIGHTS];

uniform samplerCube shadowCubeMap;

in vec4 allFragPosLightSpace [MAX_LIGHTS];

// Model
in vec3 fragVert;
in vec2 fragTexCoord;
in vec3 fragNormal;
in mat3 fragTBN;
in mat4 model;

// Out color
out vec4 finalColor;

float ShadowCalculation(vec3 normal, vec3 lightDir, sampler2D shadowMap, vec4 fragPosLightSpace)
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

vec3 sampleOffsetDirections[20] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
); 

float ShadowCubeCalculation(vec3 normal, vec3 fragPos, vec3 lightPos, float farPlane)
{
    // Get vector between fragment position and light position
    vec3 fragToLight = fragPos - lightPos;

    // Now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);

	float shadow = 0.0;
	float bias   = 0.15;
	int samples  = 20;
	float viewDistance = length(cameraPosition - fragPos);
	float diskRadius = 0.05;
	for(int i = 0; i < samples; ++i)
	{
		float closestDepth = texture(shadowCubeMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
		closestDepth *= farPlane;   // Undo mapping [0;1]
		if(currentDepth - bias > closestDepth)
			shadow += 1.0;
	}
	shadow /= float(samples);  

    return shadow;
}

vec3 ApplyLight(Light light, vec3 normal, vec3 surfacePos, vec3 surfaceToCamera, vec3 diffuseMap, vec3 specularMap, int lightIndex)
{
    float attenuation = 1.0;
    vec3 surfaceToLight = normalize(light.position.xyz);;
	float shadow = 0;

	// Point light
    if(light.position.w == 1.0)
    {
        surfaceToLight = normalize(light.position.xyz - surfacePos);
        float distanceToLight = length(light.position.xyz - surfacePos);
        attenuation = 1.0 / (1.0 + light.attenuation * pow(distanceToLight, 4));

		//shadow = ShadowCubeCalculation(normal, surfacePos, light.position.xyz, light.farPlane);
    }
	else
	{
		shadow = ShadowCalculation(normal, surfaceToLight, light.shadowMap, allFragPosLightSpace[lightIndex]);
	}

    // Ambient
    vec3 ambient = light.ambientCoefficient * diffuseMap.rgb * light.color;

    // Diffuse
    float diff = max(0.0, dot(normal, surfaceToLight));
    vec3 diffuse = diff * diffuseMap.rgb * light.color;

    // Specular
    float spec = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, normal))), material.shininess);
    vec3 specular = spec * specularMap.rgb * light.color;

    return attenuation * ambient + (1.0 - shadow) * (attenuation * ( diffuse + specular));
}

void main()
{
	// Normal map
    vec3 normal = texture(material.texture_normal, fragTexCoord).rgb;
    normal = normalize(normal * 2.0 - 1.0);   
	normal = normalize(fragTBN * normal); 

	// Emission
    vec3 linearColor = texture(material.texture_emission, fragTexCoord).rgb;

	// Read diffuse and specular maps
    vec4 diffuseMap = texture(material.texture_diffuse, fragTexCoord) * material.tint_diffuse;
    vec3 specularMap = texture(material.texture_specular, fragTexCoord).rgb;

	// Lights
    vec3 surfaceToCamera = normalize(cameraPosition - fragVert);
    for(int i = 0; i < numLights; ++i)
    {
        linearColor += ApplyLight(allLights[i], normal, fragVert, surfaceToCamera, diffuseMap.rgb, specularMap, i);
    }
    
    // Final color (after gamma correction)
    vec3 gamma = vec3(1.0/2.2);
    finalColor = vec4(pow(linearColor, gamma), diffuseMap.a);
}