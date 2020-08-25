#version 440

layout (location = 0) in vec3 vecter_position;
layout (location = 1) in vec3 vecter_color;
layout (location = 2) in vec2 vecter_texcoord;

out vec3 vs_position;
out vec3 vs_color;
out vec2 vs_texcoord;

void main()
{
	vs_position = vecter_position;
	vs_color = vecter_color;
	vs_texcoord = vec2(vecter_texcoord.x, vecter_texcoord.y * -1.0f);

	gl_Position = vec4(vecter_position, 1.0f);
}