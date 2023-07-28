#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

// MODEL VIEW PROJECTION
uniform mat4 u_M;
uniform mat4 u_V;
uniform mat4 u_P;

void main(){
    gl_Position =  u_P * u_V * u_M * vec4(position, 1.0);
    v_TexCoord = texCoord;
}


//-----------------------------------------------------------------------------
#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

struct Light {
    vec3 ambient;
    vec3 diffuse;
};

uniform sampler2D u_Texture;
uniform Light u_Light;

void main(){
    vec4 texColor = texture(u_Texture, v_TexCoord);
    color = texColor * vec4(u_Light.diffuse + u_Light.ambient, 1.0);
}
