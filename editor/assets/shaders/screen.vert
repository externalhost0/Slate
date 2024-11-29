#version 410 core
layout (location = 0) in vec2 a_Pos;
layout (location = 1) in vec2 a_TexCoord;

out vec2 TexCords;
void main() {
    gl_Position = vec4(a_Pos.x, a_Pos.y, 0.0, 1.0);
    TexCords = a_TexCoord;
}