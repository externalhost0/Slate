#version 410 core
// DEFINES
struct AmbientLight {
    vec3 Color;
    float Intensity;
};

struct DirectionalLight {
    vec3 Color;
    float Intensity;
    vec3 Direction;
};

struct PointLight {
    vec3 Position;
    vec3 Color;
    float Intensity;
    float Range;
};
struct SpotLight {
    vec3 Position;
    vec3 Color;
    float Intensity;
    float Cutoff;
};

#define NR_POINT_LIGHTS 4
#define NR_SPOT_LIGHTS 2

// INPUT
in vec3 Normal;
in vec2 TexCoord;
in vec3 FragPos;

// OUTPUT
layout(location = 0) out vec4 o_FragColor;
layout(location = 1) out int o_EntityID;

// UNIFORMS
uniform int v_EntityID;
uniform vec3 u_ObjectColor = vec3(0.8f, 0.8f, 0.8f);
uniform vec3 v_ViewPos;
// lights
uniform AmbientLight u_AmbientLight = AmbientLight(vec3(0.1f, 0.1, 0.1f), 1.0f);
uniform DirectionalLight u_DirectionalLight;
uniform PointLight u_PointLights[NR_POINT_LIGHTS];
uniform SpotLight u_SpotLightsp[NR_SPOT_LIGHTS];

// FUNCTIONS
vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
    vec3 lightResult = u_AmbientLight.Color * u_AmbientLight.Intensity;
    vec3 normaly = normalize(Normal);
    vec3 viewDir = normalize(v_ViewPos - FragPos);


    lightResult += CalcDirLight(u_DirectionalLight, normaly, viewDir);
    // Iterate over point lights
    for (int i = 0; i < NR_POINT_LIGHTS; i++) {
        lightResult += CalcPointLight(u_PointLights[i], normaly, FragPos, viewDir);
    }

    vec3 totalcol = lightResult * u_ObjectColor;
    o_FragColor = vec4(totalcol, 1.0f);
    o_EntityID = v_EntityID;
}

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir)  {
    vec3 lightDir = normalize(-light.Direction);
    // diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16.0); // 16 is arbitrary for its "specularness"

    // combine
    vec3 diffuse = light.Intensity * light.Color * diff;
    vec3 specular = light.Intensity * light.Color * spec;
    return (diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    // find direction and distance
    vec3 lightDir = normalize(light.Position - fragPos);
    float distance = length(light.Position - fragPos);

    // calc attenuation (falloff)
    float attenuation = clamp((light.Intensity / light.Range) * (1.0 - distance / light.Range), 0.0, 1.0);

    // diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * light.Color;

    // specular
    float specularStrength = 0.5;
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 16.0);
    vec3 specular = specularStrength * spec * light.Color;

    return attenuation * (diffuse + specular);
}
