#version 410 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;


uniform mat4 v_ModelMatrix;
uniform mat4 v_ViewMatrix;
uniform mat4 v_ProjectionMatrix;

out vec2 TexCoord;

void main() {
    // extract camera values
    vec3 cameraRight = vec3(v_ViewMatrix[0][0], v_ViewMatrix[1][0], v_ViewMatrix[2][0]);
    vec3 cameraUp = vec3(v_ViewMatrix[0][1], v_ViewMatrix[1][1], v_ViewMatrix[2][1]);

    // extract scale
    vec3 scale = vec3(v_ModelMatrix[0][0], v_ModelMatrix[1][1], v_ModelMatrix[2][2]);

    // compute corrected billboard locationm
    vec3 billboardCenter = vec3(v_ModelMatrix[3]); // Extract world position of the billboard
    vec3 offset = (a_Position.x * cameraRight * scale.x) + (a_Position.y * cameraUp * scale.y);
    // calculate adjusted pos
    vec3 finalPosition = billboardCenter + offset;

    gl_Position = v_ProjectionMatrix * v_ViewMatrix * vec4(finalPosition, 1.0);
    TexCoord = a_TexCoord;
}