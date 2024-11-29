#version 410 core
in vec3 Normal;
in vec2 TexCoord;
in vec3 FragPos;

// editor manipulated uniform (v_)
uniform int v_EntityID;
uniform vec3 v_ViewPos;

// user manipulated uniforms (u_)
uniform vec3 u_ObjectColor = vec3(1.f, 0.286f, 0.f);
uniform vec3 u_LightColor = vec3(0.0f, 1.0f, 0.0f);


layout(location = 0) out vec4 o_FragColor;
layout(location = 1) out int o_EntityID;

void main() {
    vec3 ambientColor = vec3(0.3f, 0.3f, 0.3f);
    float ambientStrength = 0.5f;
    vec3 ambientLight = ambientStrength * ambientColor;

    vec3 lightPos = vec3(3.0f, 3.0f, 3.0f);

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = diff * u_LightColor;

    float specularStrength = 0.5;

    vec3 viewDir = normalize(v_ViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * u_LightColor;

    vec3 objectResult = (ambientLight + diffuse + specular) * u_ObjectColor;
    o_FragColor = vec4(objectResult, 1.0f);
    o_EntityID = v_EntityID;
}