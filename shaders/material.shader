#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 v_FragPos;

// MODEL VIEW PROJECTION
uniform mat4 u_ModelViewMatrix;
uniform mat4 u_MVP;

void main(){
    v_FragPos = vec3(u_ModelViewMatrix * vec4(position, 1.0));
    v_TexCoord = texCoord;
    v_Normal = mat3(transpose(inverse(u_ModelViewMatrix))) * normal;

    gl_Position =  u_MVP * vec4(position, 1.0);
}

//-----------------------------------------------------------------------------

#shader fragment
#version 330 core

layout(location = 0) out vec4 FragColor;

in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_FragPos;

#define SHININESS_SCALE 128.0f
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;

    float shininess;

    float emission_level;
    bool emission_mask;
};

struct DirLight { // Directional light "LIKE SUN"
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NR_POINT_LIGHTS 1
struct PointLight { // Point light
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform vec3 u_ViewPos;
uniform Material u_Material;
uniform PointLight u_PointLight;
uniform DirLight u_DirLight;
uniform bool u_DepthTest;

// zdeth test
float near = 0.1;
float far  = 100.0;
float LinearizeDepth(float depth);

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main(){
    //properties
    vec3 norm= normalize(v_Normal);
    vec3 viewDir = normalize(u_ViewPos - v_FragPos);

    //step 1: Directional light
    vec3 result = calcDirLight(u_DirLight, norm, viewDir);
    //step 2: Point Lights
    for (int i = 0; i < NR_POINT_LIGHTS; i++)
    result += calcPointLight(u_PointLight, norm, v_FragPos, viewDir);
    //TODO step 3: Spot Light
    // result += calcSpotLight();

    if (u_DepthTest){
        float depth = LinearizeDepth(gl_FragCoord.z) / far;
        FragColor =vec4(vec3(depth), 1.0);
        return;
    }
    FragColor = vec4(result, 1.0);
}

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir){
    vec3 textureDiffuse = texture(u_Material.diffuse, v_TexCoord).rgb;
    vec3 textureSpecular = texture(u_Material.specular, v_TexCoord).rgb;

    //diffuse
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    //specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, -reflectDir), 0.0), (u_Material.shininess * SHININESS_SCALE));

    //combine
    vec3 ambient = light.ambient * textureDiffuse;
    vec3 diffuse = light.diffuse * diff * textureDiffuse;
    vec3 specular = light.specular * spec * textureSpecular;

    return (ambient + diffuse + specular);
}


vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
    vec3 textureDiffuse = texture(u_Material.diffuse, v_TexCoord).rgb;
    vec3 textureSpecular = texture(u_Material.specular, v_TexCoord).rgb;

    //dyiffuse
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    //specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, -reflectDir), 0.0), (u_Material.shininess * SHININESS_SCALE));
    // emission
    vec3 emission = texture(u_Material.emission, v_TexCoord).rgb * u_Material.emission_level;
    if (u_Material.emission_mask){ // flag enable draw specular OVER emission.
        vec3 emission_mask_rgb = step(vec3(1.0f), vec3(1.0f) - textureSpecular.rgb);
        emission *= emission_mask_rgb;
    }
    //attenuation
    float dist = length(light.position - fragPos);
    float attenuation = 1.0/(light.constant + light.linear * dist + light.quadratic * (dist * dist));

    //combine
    vec3 ambient = light.ambient * textureDiffuse;
    vec3 diffuse = light.diffuse * diff * textureDiffuse;
    vec3 specular = light.specular * spec * textureSpecular;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular + emission);
}

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // back to NDC
    return (2.0 * near * far) / (far + near - z * (far - near));
}
