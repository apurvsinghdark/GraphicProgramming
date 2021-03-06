#version 440

layout (location = 0) in vec3 vecter_position;
layout (location = 1) in vec3 vecter_color;
layout (location = 2) in vec2 vecter_texcoord;
layout (location = 3) in vec3 vecter_normal;

out vec3 vs_position;
out vec3 vs_color;
out vec2 vs_texcoord;
out vec3 vs_normal;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main()
{
	vs_position = vec4(ModelMatrix* vec4(vecter_position, 1.0f)).xyz;
	vs_color = vecter_color;
	vs_texcoord = vec2(vecter_texcoord.x, vecter_texcoord.y * -1.0f);
	vs_normal = mat3(transpose(inverse(ModelMatrix))) * vecter_normal;

	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(vecter_position, 1.0f);
}