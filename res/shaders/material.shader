#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 v_FragPos;

// MODEL VIEW PROJECTION
uniform mat4 u_M;
uniform mat4 u_V;
uniform mat4 u_P;

void main(){
    v_FragPos = vec3(u_M * vec4(position, 1.0));
    v_TexCoord = texCoord;
    v_Normal = mat3(transpose(inverse(u_M))) * normal;

    gl_Position =  u_P * u_V * u_M * vec4(position, 1.0);
}

//-----------------------------------------------------------------------------
#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_FragPos;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
    float emission_level;
    bool emission_mask;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 u_CameraPos;
uniform Material u_Material;
uniform Light u_Light;

void main(){
    // ambient lighting
    vec4 material_texture = texture(u_Material.diffuse, v_TexCoord);
    vec3 ambient = u_Light.ambient * material_texture.rgb;

    // diffuse lighting (light source)
    vec3 norm = normalize(v_Normal);
    vec3 light_dir = normalize(u_Light.position - v_FragPos);
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = u_Light.diffuse * diff * material_texture.rgb;

    // specular lighting (brightness)
    vec4 specular_texture = texture(u_Material.specular, v_TexCoord);
    vec3 camera_dir = normalize(u_CameraPos - v_FragPos);
    vec3 reflect_dir = reflect(-light_dir, v_Normal);
    float spec = pow(max(dot(camera_dir, reflect_dir), 0.0), (u_Material.shininess * 128.0));
    vec3 specular = u_Light.specular * spec * specular_texture.rgb;

    // emission
    vec3 emission = texture(u_Material.emission, v_TexCoord).rgb * u_Material.emission_level;
    if (u_Material.emission_mask){ // flag enable draw specular OVER emission.
        vec3 emission_mask_rgb = step(vec3(1.0f), vec3(1.0f) - specular_texture.rgb);
        emission *= emission_mask_rgb;
    }

    // output
    color = vec4((ambient + diffuse + specular + emission), 1.0);
}
