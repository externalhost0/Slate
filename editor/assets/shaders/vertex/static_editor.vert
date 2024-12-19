#version 410 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

//layout (std140) uniform CameraMatrices
//{
//    mat4 v_ProjectionMatrix;
//    mat4 v_ViewMatrix;
//};
uniform mat4 v_ModelMatrix;
uniform mat4 v_ViewMatrix;
uniform mat4 v_ProjectionMatrix;

out vec2 TexCoord;

void main() {
    gl_Position = (v_ProjectionMatrix * v_ViewMatrix * v_ModelMatrix) * vec4(a_Position, 1.0);
    TexCoord = a_TexCoord;
}
