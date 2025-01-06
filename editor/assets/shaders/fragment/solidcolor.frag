#version 410 core

layout(location = 0) out vec4 o_FragColor;

void main() {
    float u_FadeEnd = 50.0f;
    float u_FadeStart = 10.0f;

    float depth = gl_FragCoord.z / gl_FragCoord.w; // linearized depth value

    // fade factor
    float fade = clamp((u_FadeEnd - depth) / (u_FadeEnd - u_FadeStart), 0.0, 1.0);

    o_FragColor = vec4(0.4, 0.4, 0.4, fade); // dark gray color
}