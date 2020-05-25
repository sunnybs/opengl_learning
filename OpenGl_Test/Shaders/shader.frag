#version 330 core
out vec4 color;

in vec3 Normal;  
in vec3 FragPos; 
in vec2 TexCoord;

uniform vec3 lightPos; 
uniform vec3 lightColor;

uniform sampler2D ourTexture;

void main()
{
	// Ambient
    float ambientStrength = 0.3f;
    vec3 ambient = ambientStrength * lightColor;

	// Diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

	vec3 result = (ambient + diffuse) * texture(ourTexture, TexCoord).rgb;
	color = vec4(result, 1.0f);
}