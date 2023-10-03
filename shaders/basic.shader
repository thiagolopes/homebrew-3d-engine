#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;


out vec2 v_TexCoord;

// MODEL VIEW PROJECTION
uniform mat4 u_M;
uniform mat4 u_V;
uniform mat4 u_P;

void main(){
    gl_Position =  u_P * u_V * u_M * vec4(position, 1.0);
}

//-----------------------------------------------------------------------------
#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;


void main() {
    color = vec4(1.0);
}
