#version 330 core

layout(location = 0) in vec2 vertex_pos;

void main()
{
	gl_Position = vec4(vertex_pos.x, vertex_pos.y, 0.f, 1.f);
}