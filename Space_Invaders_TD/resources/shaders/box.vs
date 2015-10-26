#version 430

layout(location = 0) in vec2 vs_vert;

uniform mat4 model;
uniform mat4 projection;

void main(){
	gl_Position = projection * model * vec4(vs_vert, 0.0, 1.0);
}