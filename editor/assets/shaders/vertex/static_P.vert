#version 410 core
layout(location = 0) in vec3 a_Position;

uniform mat4 v_ModelMatrix;
uniform mat4 v_ViewMatrix;
uniform mat4 v_ProjectionMatrix;

void main() {
    gl_Position = (v_ProjectionMatrix * v_ViewMatrix * v_ModelMatrix) * vec4(a_Position, 1.0);
}
