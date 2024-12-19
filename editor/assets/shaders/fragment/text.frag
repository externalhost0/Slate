#version 410 core
in vec2 TexCoord;

uniform int v_EntityID;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

layout(location = 0) out vec4 o_FragColor;
layout(location = 1) out int o_EntityID;

void main() {
    vec4 sampled = vec4(1.0f, 1.0f, 1.0f, texture(u_Texture, TexCoord).r);
    o_FragColor = u_Color * sampled;
    o_EntityID = v_EntityID;
}