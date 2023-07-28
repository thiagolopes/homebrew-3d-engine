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


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_FragPos;

struct Material {
    sampler2D diffuse_texture;
    sampler2D specular_texture;
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 u_CameraPos;
uniform Material u_material;
uniform Light u_light;

void main(){
    vec4 texColor = texture(u_material.diffuse_texture, v_TexCoord);
    vec4 textColorSpecular = texture(u_material.specular_texture, v_TexCoord);

    // ambient lighting
    vec3 ambient = u_light.ambient * vec3(texColor);

    // diffuse lighting (light source)
    vec3 norm = normalize(v_Normal);
    vec3 light_dir = normalize(u_light.position - v_FragPos);
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = u_light.diffuse * diff * texColor.rgb;

    // specular lighting (brightness)
    vec3 camera_dir = normalize(u_CameraPos - v_FragPos);
    vec3 reflect_dir = reflect(-light_dir, v_Normal);
    float spec = pow(max(dot(camera_dir, reflect_dir), 0.0), u_material.shininess);
    vec3 specular = u_light.specular * spec * textColorSpecular.rgb;

    color = vec4((ambient + diffuse + specular), 1.0) * texColor;
}
