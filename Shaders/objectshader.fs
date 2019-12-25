#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;  
uniform vec3 viewPos;

void main()
{
    // Adding ambient light.
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    

    // Adding diffuse light.
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);  
    // Dot product of normal surface to light direction to find diffusion intensity.
    // Negative dot products are set as 0.
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Adding Specular light.
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); 
    // We first calculate the dot product between the view direction and the reflect direction (and make sure it's not negative) 
    // and then raise it to the power of 32. This 32 value is the shininess value of the highlight.
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor; 

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result , 1.0);
}