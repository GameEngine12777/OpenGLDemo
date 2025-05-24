#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

out vec4 v_Color;

void main()
{
   gl_Position = position;
   v_Color = color;
};


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform float u_Offset;

in vec4 v_Color;

void main()
{
	// vec3 offsetColor = clamp(vec3(v_Color) + u_Offset, 0.0, 1.0);
	vec3 offsetColor = mod(vec3(v_Color) + u_Offset, 1.0);
	color = vec4(offsetColor, v_Color.w);
};