#version 410 core
in vec2 TexCords;

layout(location = 0) out vec4 FragColor;

// just manually fill in these values for now
uniform sampler2D u_DepthTexture; // Depth texture
uniform float u_Near;            // Near plane
uniform float u_Far;             // Far plane
uniform float u_Gamma;           // Gamma correction value

void main() {
    float depth = texture(u_DepthTexture, TexCords).r;
    // linearize depth
    float linearDepth = (2.0 * u_Near * u_Far) / (u_Far + u_Near - (u_Far - u_Near) * (2.0 * depth - 1.0));
    // normalize
    float normalizedDepth = (linearDepth - u_Near) / (u_Far - u_Near);
    // apply gamma correction
    float correctedDepth = pow(normalizedDepth, 1.0 / u_Gamma);
    // output as grayscale
    FragColor = vec4(correctedDepth, correctedDepth, correctedDepth, 1.0);
}