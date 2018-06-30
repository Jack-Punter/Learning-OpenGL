#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

uniform mat4 uMVP;

void main()
{
	gl_Position = uMVP * position;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 colour;

void main()
{
	colour = vec4(0.2, 0.3, 0.8, 1.0);
};
