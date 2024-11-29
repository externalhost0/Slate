#version 410 core

uniform int v_EntityID;

layout(location = 0) out vec4 o_FragColor;
layout(location = 1) out int o_EntityID;

void main() {
    o_FragColor = vec4(0.7, 0.7, 0.7, 1.0); // Light gray color
    o_EntityID = v_EntityID;
}