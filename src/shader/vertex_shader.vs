#version 440 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texture;

out vec3 vertex_color;
out vec2 texture_coord;

uniform mat4 transform;

void main()
{
	gl_Position = transform * vec4(position, 1.0f);
	vertex_color = color;
	texture_coord = texture;
}