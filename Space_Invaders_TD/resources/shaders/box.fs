#version 430

in vec2 fs_tex;
out vec4 color;

uniform vec3 boxColor;

void main() {
	color = vec4(boxColor, 1.0);
}