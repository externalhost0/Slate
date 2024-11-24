#version 410 core
in vec3 a_Normal;
in vec2 a_TexCoord;
// editor manipulated uniform
uniform int v_EntityID;

// user manipulated uniforms
uniform vec3 objectColor;
uniform vec4 sunColor;
uniform float opacity;

layout(location = 0) out vec4 o_FragColor;
layout(location = 1) out int o_EntityID;

void main() {
//    vec4 res = sunColor * vec4(objectColor, 1.0f);
    o_FragColor = vec4(objectColor, opacity);
    o_EntityID = v_EntityID;
}