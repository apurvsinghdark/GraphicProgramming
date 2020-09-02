#version 440

layout (location = 0) in vec3 vecter_position;
layout (location = 1) in vec3 vecter_color;
layout (location = 2) in vec2 vecter_texcoord;

out vec3 vs_position;
out vec3 vs_color;
out vec2 vs_texcoord;

uniform mat4 ModelMatrix;

void main()
{
	vs_position = vec4(ModelMatrix* vec4(vecter_position, 1.0f)).xyz;
	vs_color = vecter_color;
	vs_texcoord = vec2(vecter_texcoord.x, vecter_texcoord.y * -1.0f);

	gl_Position = ModelMatrix * vec4(vecter_position, 1.0f);
}