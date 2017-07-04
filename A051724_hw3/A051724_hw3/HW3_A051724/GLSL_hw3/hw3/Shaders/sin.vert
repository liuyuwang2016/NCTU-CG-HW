#version 450

layout(location = 0) in vec3 Position;

out vec3 position_v;

void main() {
	
	gl_Position = vec4(Position, 1.0);
	position_v = Position;
}