#version 440

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord;
in vec3 vs_normal;

out vec4 fs_color;

uniform sampler2D texture0;
uniform sampler2D texture1;

uniform vec3 lightPos0;

void main()
{ 
	//Ambient Light
	vec3 ambientLight = vec3(0.3f, 0.3f, 0.3f);

	//Diffuse light
	vec3 normalDir = normalize(vs_normal);
	vec3 lightDir = normalize(vs_position - lightPos0);
	vec3 diffColor = vec3(1.0f, 1.0f, 1.0f);
	float diffDir = clamp(dot(normalDir, lightDir), 0, 1);
	vec3 diffuseLight = diffColor * diffDir;

	//fs_color = texture(texture0, vs_texcoord) * texture(texture1, vs_texcoord) * vec4(vs_color, 1.0f);;
	fs_color = texture(texture1, vs_texcoord) * vec4(vs_color, 1.0f) * (vec4(ambientLight, 1.0f) + vec4(diffuseLight, 1.0f));
}