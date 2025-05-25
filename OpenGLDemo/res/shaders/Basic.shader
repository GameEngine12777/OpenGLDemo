#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

out vec4 v_Color;

uniform float u_Time;

void main()
{
    // 心跳因子：随时间脉动
    float scale = 1.0 + 0.05 * sin(u_Time * 6.0); // 跳动频率和幅度

    vec2 scaledPos = vec2(position.r, position.g) * scale;

    gl_Position = vec4(scaledPos, 0.0, 1.0);
    v_Color = color;
};


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform float u_Offset;

in vec4 v_Color;

void main()
{
    color = v_Color;
};