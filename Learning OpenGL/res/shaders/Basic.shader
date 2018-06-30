#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 vTexCoord;

uniform mat4 uMVP;

void main()
{
	gl_Position = uMVP * position;
	vTexCoord = texCoord;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 colour;

in vec2 vTexCoord;

uniform sampler2D uTexture;

void main()
{
	vec4 texColour = texture(uTexture, vTexCoord);
	colour = texColour; 
};
