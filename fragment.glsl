#version 440

struct Material
{
	vec3 ambident;
	vec3 diffuse;
	vec3 specular;
	sampler2D diffuseTex;
	sampler2D specularTex;
};

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord;
in vec3 vs_normal;

out vec4 fs_color;

uniform Material material;

uniform sampler2D texture0;
uniform sampler2D texture1;

uniform vec3 lightPos0;
uniform vec3 camPos;

vec3 AmbidentLight(Material material) { return material.ambident; }

vec3 DiffuseLight(Material material, vec3 vs_position, vec3 vs_normal, vec3 lightPos0)
{
	vec3 diffLightDir = normalize(lightPos0 - vs_position);
	float diffConst = clamp(dot(diffLightDir, vs_normal), 0, 1);
	vec3 diffuseLight = material.diffuse * diffConst;
	
	return diffuseLight;
}

vec3 SpecularLight(Material material, vec3 vs_position, vec3 vs_normal, vec3 lightPos0, vec3 camPos)
{
	vec3 specLightDir = normalize(vs_position - lightPos0);
	vec3 reflectionSrc = normalize(reflect(specLightDir,vs_normal)); // Light Coming From Source
	vec3 reflectOnEye = normalize(camPos - vs_position); // reflection on eye
	float specularConst = pow(max(dot(reflectOnEye, reflectionSrc), 0), 100);
	vec3 specularLight = material.specular * specularConst * texture(material.specularTex, vs_texcoord).rgb;
	
	return specularLight;
}

void main()
{ 
	vec3 normalDir = normalize(vs_normal);
	
	//Ambient light
	vec3 ambientLight = AmbidentLight(material);

	//Diffuse light
	vec3 diffuseLight = DiffuseLight(material, vs_position, normalDir, lightPos0);

	//Specular light
	vec3 specularLight = SpecularLight(material, vs_position, normalDir, lightPos0, camPos);

	//Atenuation light

	//Final OutPut
	fs_color = texture(material.diffuseTex, vs_texcoord) 
				* vec4(vs_color, 1.0f) 
				* (vec4(ambientLight, 1.0f) 
				+ vec4(diffuseLight, 1.0f) 
				+ vec4(specularLight, 1.0f)
				);
}