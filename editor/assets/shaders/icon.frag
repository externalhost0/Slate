#version 410 core
in vec2 TexCoord;

uniform int v_EntityID;
uniform sampler2D u_Texture;

layout(location = 0) out vec4 o_FragColor;
layout(location = 1) out int o_EntityID;

void main() {
    o_FragColor = texture(u_Texture, TexCoord);
    o_FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    o_EntityID = v_EntityID;
}