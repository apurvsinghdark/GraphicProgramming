#version 440

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord;
in vec3 vs_normal;

out vec4 fs_color;

uniform sampler2D texture0;
uniform sampler2D texture1;

uniform vec3 lightPos0;
uniform vec3 camPos;

void main()
{ 
	vec3 normalDir = normalize(vs_normal);
	
	//Ambient Light
	vec3 ambientLight = vec3(0.1f, 0.1f, 0.1f);

	//Diffuse light
	vec3 diffLightDir = normalize(vs_position - lightPos0);
	vec3 diffColor = vec3(1.0f, 1.0f, 1.0f);
	float diffDir = clamp(dot(normalDir, diffLightDir), 0, 1);
	vec3 diffuseLight = diffColor * diffDir;

	//Specular light
	vec3 specularLightDir = normalize(lightPos0 - vs_position);
	vec3 reflectLightDir = normalize(reflect(specularLightDir, normalDir));
	vec3 reflectionOnCam = normalize(vs_position - camPos);
	float specularConst = pow(max(dot(reflectionOnCam, reflectLightDir), 0), 100);
	vec3 specularLight = vec3(1.0f, 1.0f, 1.0f) * specularConst;

	//fs_color = texture(texture0, vs_texcoord) * texture(texture1, vs_texcoord) * vec4(vs_color, 1.0f);;
	fs_color = texture(texture1, vs_texcoord) * vec4(vs_color, 1.0f) 
		* (vec4(ambientLight, 1.0f) + vec4(diffuseLight, 1.0f) + vec4(specularLight, 1.0f));
}