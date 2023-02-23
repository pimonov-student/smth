#version 440 core

in vec3 vertex_color;
in vec2 texture_coord;

out vec4 color;

uniform sampler2D texture;

void main()
{
	color = texture(texture, texture_coord) * vec4(vertex_color, 1.0f);
}