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

    vec3 Direction;

    float Size;
    float Blend;
};

#define NR_POINT_LIGHTS 4
#define NR_SPOT_LIGHTS 6

// INPUT
in vec3 Normal;
in vec2 TexCoord;
in vec3 FragPos;

// OUTPUT
layout(location = 0) out vec4 o_FragColor;
layout(location = 1) out int o_EntityID;

// UNIFORMS
uniform int v_EntityID;
uniform vec3 u_ObjectColor = vec3(1.f, 1.f, 1.f);
uniform vec3 v_ViewPos;
// lights
uniform AmbientLight u_AmbientLight = AmbientLight(vec3(0.1f, 0.1, 0.1f), 1.0f);
uniform DirectionalLight u_DirectionalLight;
uniform PointLight u_PointLights[NR_POINT_LIGHTS];
uniform SpotLight u_SpotLights[NR_SPOT_LIGHTS];

// FUNCTION DECLARATION
vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

// MAIN SHADER
void main() {
    vec3 lightResult = u_AmbientLight.Color * u_AmbientLight.Intensity;
    vec3 normalzedNormal = normalize(Normal);
    vec3 viewDir = normalize(v_ViewPos - FragPos);


    lightResult += CalcDirLight(u_DirectionalLight, normalzedNormal, viewDir);
    // Iterate over point lights
    for (int i = 0; i < NR_POINT_LIGHTS; i++) {
        lightResult += CalcPointLight(u_PointLights[i], normalzedNormal, FragPos, viewDir);
    }
    // Iterate over spot lights
    for (int i = 0; i < NR_SPOT_LIGHTS; i++) {
        lightResult += CalcSpotLight(u_SpotLights[i], normalzedNormal, FragPos, viewDir);
    }

    // total light contribution mutlipled by object material
    vec3 totalcol = lightResult * u_ObjectColor;
    o_FragColor = vec4(totalcol, 1.0f);
    o_EntityID = v_EntityID;
}

// FUNCTION DEFINITION
vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir)  {
    vec3 lightDir = normalize(-light.Direction);
    // diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16.0); // 16 is arbitrary for its "specularness"

    // combine
    float intensity = light.Intensity / 10.0f;
    vec3 diffuse = intensity * light.Color * diff;
    vec3 specular = intensity * light.Color * spec;
    return (diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    // find direction and distance
    vec3 direction = normalize(light.Position - fragPos);
    float distancelight = length(light.Position - fragPos);

    // calc attenuation (falloff)
    // differs from spot due to having range feature
//    float attenuation = clamp((light.Intensity / light.Range) * (1.0 - distancelight / light.Range), 0.0, 1.0);
    //https://imdoingitwrong.wordpress.com/tag/lighting/

    float r = light.Range;
    float attenuation = clamp( 1.0 - (distancelight*distancelight) / (r*r), 0.0, 1.0);

    // diffuse
    float diff = max(dot(normal, direction), 0.0);
    vec3 diffuse = diff * light.Color;

    // specular
    vec3 halfwayDir = normalize(direction + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 16.0);
    vec3 specular = spec * light.Color;


    attenuation *= light.Intensity/distancelight;
    return attenuation * (diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    // vars
    vec3 direction = normalize(light.Position - fragPos);
    float distancelight = length(light.Position - fragPos);

    // diffuse
    float diff = max(dot(normal, direction), 0.0);
    vec3 diffuse = diff * light.Color;

    // spec
    vec3 halfwayDir = normalize(direction + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 16.0);
    vec3 specular = spec * light.Color;


    // attenuation calculations
    float theta = dot(direction, normalize(-light.Direction));
    float cosOuter = cos(radians(light.Size * 0.5));
    float attenuation;

    if (light.Blend == 0.0) {
        // Sharp cutoff for Blend = 0.0
        attenuation = step(cosOuter, theta); // 1.0 if theta >= cosOuter, else 0.0
    } else {
        // Smooth transition for Blend > 0.0
        float cosInner = cos(radians(light.Size * 0.5 * (1.0 - light.Blend))); // Cosine of half the inner cutoff angle
        attenuation = clamp((theta - cosOuter) / (cosInner - cosOuter), 0.0, 1.0);
    }

    attenuation *= light.Intensity/distancelight;
    // Combine diffuse and specular with attenuation and intensity
    return attenuation * (diffuse + specular);
}