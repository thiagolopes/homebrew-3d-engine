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

uniform sampler2D u_Texture;
uniform vec3 u_LightColor;
uniform vec3 u_LightPos;
uniform vec3 u_CameraPos;

void main(){
    // ambient lighting
    float ambient_streth = 0.4;
    vec3 ambient = ambient_streth * u_LightColor;

    // diffuse lighting (light source)
    vec3 norm = normalize(v_Normal);
    vec3 light_dir = normalize(u_LightPos - v_FragPos);
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = diff * u_LightColor;

    // specular lighting (brightness)
    float specular_strength = 0.4;
    vec3 camera_dir = normalize(u_CameraPos - v_FragPos);
    vec3 reflect_dir = reflect(-light_dir, v_Normal);
    float spec = pow(max(dot(camera_dir, reflect_dir), 0.0), 32);
    vec3 specular = specular_strength * spec * u_LightColor;

    vec4 texColor = texture(u_Texture, v_TexCoord);
    color = vec4((ambient + diffuse + specular), 1.0) * texColor;
}
