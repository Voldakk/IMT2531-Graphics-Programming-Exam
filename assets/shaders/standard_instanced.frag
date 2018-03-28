#version 150

uniform vec3 cameraPosition;

// Material settings
uniform sampler2D texture_diffuse1;
uniform float materialShininess;
uniform vec3 materialSpecularColor;

uniform struct Light 
{
   vec4 position;
   vec3 color;
   float attenuation;
   float ambientCoefficient;
} light;

in vec2 fragTexCoord;
in vec3 fragNormal;
in vec3 fragVert;

in mat4 model;

out vec4 finalColor;

void main() 
{
    vec3 normal = normalize(transpose(inverse(mat3(model))) * fragNormal);
    vec3 surfacePos = vec3(model * vec4(fragVert, 1));
    vec4 surfaceColor = texture(texture_diffuse1, fragTexCoord);
    vec3 surfaceToCamera = normalize(cameraPosition - surfacePos);

    vec3 surfaceToLight;
    if(light.position.w == 0.0) // Directional light
    {
      surfaceToLight = normalize(light.position.xyz);
    }
    else // Point light
    {
      surfaceToLight = normalize(light.position.xyz - surfacePos);
    }

    // Ambient
    vec3 ambient = light.ambientCoefficient * surfaceColor.rgb * light.color;

    // Diffuse
    float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight));
    vec3 diffuse = diffuseCoefficient * surfaceColor.rgb * light.color;
    
    // Specular
    float specularCoefficient = 0.0;
    if(diffuseCoefficient > 0.0)
        specularCoefficient = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, normal))), materialShininess);
    vec3 specular = specularCoefficient * materialSpecularColor * light.color;

    // Attenuation
    float attenuation = 1.0;
    if(light.position.w == 1.0) // Point light
    {
        float distanceToLight = length(light.position.xyz - surfacePos);
        attenuation = 1.0 / (1.0 + light.attenuation * pow(distanceToLight, 2));
    }

    // Linear color (color before gamma correction)
    vec3 linearColor = ambient + attenuation*(diffuse + specular);
    
    // Final color (after gamma correction)
    vec3 gamma = vec3(1.0/2.2);
    finalColor = vec4(pow(linearColor, gamma), surfaceColor.a);
}