#version 410 core
in vec3 Normal;
in vec2 TexCoord;
in vec3 FragPos;

// editor manipulated uniform (v_)
uniform int v_EntityID;

layout(location = 0) out vec4 o_FragColor;
layout(location = 1) out int o_EntityID;

void main() {
    o_FragColor = vec4(0.5f, 0.5f, 0.6f, 0.5f);
    o_EntityID = v_EntityID;
}