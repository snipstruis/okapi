#version 140

uniform mat4 modelview;
uniform mat4 projection;

in vec3 vertex;
out vec3 pos;

void main(void) {
	gl_Position = projection * modelview * vec4(vertex,1);
	vec4 pos4 = modelview * vec4(vertex,1);
	pos = pos4.xyz;
}

