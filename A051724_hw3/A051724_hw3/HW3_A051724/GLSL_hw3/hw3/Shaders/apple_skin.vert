#version 430

uniform vec3 Eye;
uniform vec3 Light;
uniform mat4 MV;
uniform mat4 P;
uniform mat4 Rotate;

layout(location = 4) in vec3 Position;
layout(location = 5) in vec3 Normal;
layout(location = 6) in vec2 TexCoord;
layout(location = 7) in vec3 flatNormal;

out vec3 normal;
out vec3 light_Dir;
out vec3 eye_Dir;
out vec2 tex_Coord;
flat out vec3 fNormal;

void main() {
	normal = (transpose(inverse(Rotate)) * vec4(Normal, 1.0)).xyz;
	vec3 vVertex = vec3(MV * vec4(Position, 1.0));
	light_Dir = vec3(MV * vec4(Light, 1.0) - vVertex);
	eye_Dir = -vVertex;
	tex_Coord = TexCoord;
	fNormal = (transpose(inverse(Rotate)) * vec4(flatNormal, 1.0)).xyz;

	gl_Position = P * Rotate * vec4(Position, 1.0);
}