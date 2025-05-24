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
	// 把颜色旋转（绕 RGB 通道循环）
    float r = v_Color.r;
    float g = v_Color.g;
    float b = v_Color.b;

    // 循环偏移颜色
    float offset = mod(u_Offset, 3.0);
    
    vec3 rotatedColor;

    if (offset < 1.0) {
        float t = offset;
        rotatedColor = vec3(mix(r, g, t), mix(g, b, t), mix(b, r, t)); // 做线性插值
    } else if (offset < 2.0) {
        float t = offset - 1.0;
        rotatedColor = vec3(mix(g, b, t), mix(b, r, t), mix(r, g, t));
    } else {
        float t = offset - 2.0;
        rotatedColor = vec3(mix(b, r, t), mix(r, g, t), mix(g, b, t));
    }

    color = vec4(rotatedColor, v_Color.a);
};