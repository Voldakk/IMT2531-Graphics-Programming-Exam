#version 150 core

// Camera
uniform vec3 cameraPosition;

// Material settings
uniform struct Material
{
   sampler2D texture_diffuse;
   sampler2D texture_specular;
   sampler2D texture_emission;

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
in vec2 fragTexCoord;
in vec3 fragNormal;
uniform mat4 model;

// Out color
out vec4 finalColor;

vec3 ApplyLight(Light light, vec3 normal, vec3 surfacePos, vec3 surfaceToCamera, vec3 diffuseMap, vec3 specularMap, int lightIndex)
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
    vec3 ambient = light.ambientCoefficient * diffuseMap.rgb * light.color;

    // Diffuse
    float diff = max(0.0, dot(normal, surfaceToLight));
    vec3 diffuse = diff * diffuseMap.rgb * light.color;

    // Specular
    float spec = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, normal))), material.shininess);
    vec3 specular = spec * specularMap.rgb * light.color;

    return attenuation * ambient + attenuation * ( diffuse + specular);
}

void main()
{
    // Normal
    vec3 normal = fragNormal;

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