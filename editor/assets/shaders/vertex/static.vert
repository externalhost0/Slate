#version 410 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

//layout (std140) uniform CameraMatrices
//{
//    mat4 v_ProjectionMatrix;
//    mat4 v_ViewMatrix;
//};
uniform mat4 v_ModelMatrix;
uniform mat4 v_ViewMatrix;
uniform mat4 v_ProjectionMatrix;

out vec3 Normal;
out vec2 TexCoord;
out vec3 FragPos;

mat3 computeNormalMatrix(mat4 modelMatrix) {
    return mat3(transpose(inverse(modelMatrix)));
}

void main() {
    mat3 normalMatrix = computeNormalMatrix(v_ModelMatrix);

    // OUTPUTTING
    Normal = normalize(normalMatrix * a_Normal);
    FragPos = vec3(v_ModelMatrix * vec4(a_Position, 1.0));
    TexCoord = a_TexCoord;
    gl_Position = (v_ProjectionMatrix * v_ViewMatrix * v_ModelMatrix) * vec4(a_Position, 1.0);
}
