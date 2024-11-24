#version 410
in vec2 TexCords;
out vec4 FragColor;

uniform sampler2D u_DepthTexture;
uniform sampler2D u_ColorTexture;
uniform sampler2D u_StencilTexture;

uniform vec2 u_ScreenSize;
uniform int u_EntityID;

// lower yeilds better results
const float u_EdgeThreshold = 0.001;
void main() {
    float stencilValue = texture(u_StencilTexture, TexCords).r; // Use the red channel for stencil buffer
    if (stencilValue == u_EntityID) {
        FragColor = vec4(vec3(1.0), 1.0);
    } else {
        FragColor = vec4(vec3(0.0), 1.0);
    }
    // Visualize it as grayscale


/**
    // pixel size
    vec2 texelSize = 1.0 / u_ScreenSize;

    // individual samples
    float depthCenter = texture(u_DepthTexture, TexCords).r;
    float depthLeft   = texture(u_DepthTexture, TexCords + vec2(-texelSize.x, 0.0)).r;
    float depthRight  = texture(u_DepthTexture, TexCords + vec2(texelSize.x, 0.0)).r;
    float depthUp     = texture(u_DepthTexture, TexCords + vec2(0.0, texelSize.y)).r;
    float depthDown   = texture(u_DepthTexture, TexCords + vec2(0.0, -texelSize.y)).r;

    // now clamp the sampled values
    depthCenter = clamp(depthCenter, 0.0, 1.0);
    depthLeft = clamp(depthLeft, 0.0, 1.0);
    depthRight = clamp(depthRight, 0.0, 1.0);
    depthUp = clamp(depthUp, 0.0, 1.0);
    depthDown = clamp(depthDown, 0.0, 1.0);

    // scale gradients by depth factor
    float depthFactor = depthCenter; // reduce sensitivity based on depth
    float dX = (depthRight - depthLeft) * depthFactor;
    float dY = (depthUp - depthDown) * depthFactor;

    // ignore small measurements of threshold
    float gradientThreshold = 0.005;
    dX = abs(dX) > gradientThreshold ? dX : 0.0;
    dY = abs(dY) > gradientThreshold ? dY : 0.0;

    float edgeIntensity = sqrt(dX * dX + dY * dY);
    edgeIntensity = clamp(edgeIntensity, 0.0, 1.0);

    // blend between color and edge that overwrites
    if (edgeIntensity > u_EdgeThreshold) {
        FragColor = vec4(1.0, 1.0, 1.0, 1.0); // bright white for edges
    } else {
        FragColor = texture(u_ColorTexture, TexCords); // just original scene color
    }*/
}