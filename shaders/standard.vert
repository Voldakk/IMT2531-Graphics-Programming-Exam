#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 vertexUV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat3 normalMatrix; 

uniform float attenuationA;
uniform float attenuationB;
uniform float attenuationC;

uniform vec3 lightColor;
uniform vec3 lightSourcePosition;
uniform vec3 CamPos;

out vec3 diffuse;        

out vec2 uv;

  //the simplest function to calculate lighting 
vec3 doColor(){
	
	//Ambient 
	float ambientStrength = 0.9;
    vec3 ambient = ambientStrength * lightColor;

	 // diffuse 
    //vec3 norm = normalize(aNormal);
	vec3 norm  = normalize( normalMatrix * normalize(aNormal) );
	vec3 FragPos = vec3(model * vec4(aPos, 1.0));
    vec3 lightDirection = normalize(lightSourcePosition-FragPos);
    vec3 diffuse = max(dot(norm, lightDirection), 0.0) * lightColor;

	    // specular
    float specularStrength = 0.9;
    vec3 viewDir = normalize(CamPos - FragPos);
    vec3 reflectDir = reflect(-lightDirection, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor; 


	float dist = distance(lightSourcePosition, (normalMatrix*aPos));
	float attenuation = 1.0f / (attenuationA + (attenuationB * dist) + (attenuationC * dist * dist));
    
	vec3 Resultant=(ambient+diffuse*(attenuation+specular));

    return Resultant;
  }

void main()
{
	uv = vertexUV;    
	diffuse = doColor();
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}