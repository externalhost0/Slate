#version 410
in vec3 Normal;
in vec2 TexCoord;
in vec3 FragPos;

uniform int v_EntityID;

layout(location = 0) out vec4 o_FragColor;
layout(location = 1) out int o_EntityID;

void main() {
    vec3 normalizedNormal = normalize(Normal);

    vec3 tangent = normalize(abs(normalizedNormal.x) > 0.9
                             ? vec3(0.0, 1.0, 0.0)
                             : vec3(1.0, 0.0, 0.0));
    vec3 bitangent = cross(normalizedNormal, tangent);

    vec2 planarCoords = vec2(dot(FragPos, tangent), dot(FragPos, bitangent));

    // scale of pattern
    float scale = 3.0;
    vec2 scaledCoords = planarCoords * scale;

    float checkerboardmask = mod(floor(scaledCoords.x) + floor(scaledCoords.y), 2.0);

    vec3 color1 = vec3(1, 0.467, 0.937);
    vec3 color2 = vec3(0.157, 0.165, 0.239);
    vec3 finalcolor = mix(color1, color2, checkerboardmask);

    o_FragColor = vec4(finalcolor, 1.0);
    o_EntityID = v_EntityID;
}