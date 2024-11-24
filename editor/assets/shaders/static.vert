#version 410 core
layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

//layout (std140) uniform Matrices
//{
//    mat4 v_ProjectionMatrix;
//    mat4 v_ViewMatrixiew;
//};
uniform mat4 v_ModelMatrix;
uniform mat4 v_ViewMatrix;
uniform mat4 v_ProjectionMatrix;

out vec3 o_Normal;
out vec2 o_TexCoord;

void main() {
    o_Normal = a_Normal;
    o_TexCoord = a_TexCoord;
    gl_Position = (v_ProjectionMatrix * v_ViewMatrix * v_ModelMatrix) * vec4(a_Pos, 1.0);
}
