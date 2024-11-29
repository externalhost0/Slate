#version 410
in vec3 Normal;
in vec2 TexCoord;
in vec3 FragPos;

uniform int v_EntityID;

layout(location = 0) out vec4 o_FragColor;
layout(location = 1) out int o_EntityID;

void main() {
    vec3 normalizedNormal = normalize(Normal);

    // Define a basis for the checkerboard projection
    // Choose two perpendicular vectors in the plane of the normal
    vec3 tangent = normalize(abs(normalizedNormal.x) > 0.9
                             ? vec3(0.0, 1.0, 0.0)
                             : vec3(1.0, 0.0, 0.0));
    vec3 bitangent = cross(normalizedNormal, tangent);

    // Compute planar coordinates based on the position and the basis
    vec2 planarCoords = vec2(dot(FragPos, tangent), dot(FragPos, bitangent));

    // Scale the checkerboard pattern
    float scale = 3.0; // Number of checks per unit
    vec2 scaledCoords = planarCoords * scale;

    // Determine whether the current fragment is in a "light" or "dark" square
    float check = mod(floor(scaledCoords.x) + floor(scaledCoords.y), 2.0);

    // Define the checkerboard colors
    vec3 color1 = vec3(1, 0.467, 0.937);
    vec3 color2 = vec3(0.157, 0.165, 0.239);

    // Select the color based on the checkerboard pattern
    vec3 color = mix(color1, color2, check);

    // Output the color
    o_FragColor = vec4(color, 1.0);

    o_EntityID = v_EntityID;
}