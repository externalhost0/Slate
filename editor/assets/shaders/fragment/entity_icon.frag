#version 410 core
in vec2 TexCoord;

uniform int v_EntityID;
uniform sampler2D u_Texture;
uniform vec3 u_Color;

layout(location = 0) out vec4 o_FragColor;
layout(location = 1) out int o_EntityID;

void main() {
    float borderThickness = 1.0f;
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(u_Texture, TexCoord).r);
    float cutout = 1.0 - sampled.a;

    o_FragColor = vec4(u_Color, cutout);
    o_EntityID = v_EntityID;
}