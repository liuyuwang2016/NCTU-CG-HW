#version 430

layout(binding = 1) uniform sampler2D Tex0;

uniform vec4 Specular;
uniform float Shiness;
uniform vec4 Ambient;
uniform int Mode;
uniform vec4 Ambient_Light;
uniform vec4 Specular_Light;

in vec3 normal;
in vec3 light_Dir;
in vec3 eye_Dir;
in vec2 tex_Coord;
flat in vec3 fNormal;


out vec4 outColor;

void main() 
{
	vec4 ambientLight = Ambient_Light;
	vec4 specularLight = Specular_Light;
	vec4 diffuseLight = Specular_Light;
	vec4 Kd = texture2D(Tex0, tex_Coord).rgba;
	vec4 Ks = Specular;

	vec4 color = Ambient * ambientLight;
	vec3 N = normalize(normal);
	if (Mode == 1) N = normalize(fNormal);
	vec3 L = normalize(light_Dir);
	float cosTheta = dot(N, L);
	if (cosTheta > 0.0) {
		color += Kd * diffuseLight * cosTheta;
		vec3 E = normalize(eye_Dir);
		vec3 R = reflect(-L, N);
		float specular = pow( max(dot(R, E), 0.0), Shiness);
		color += Ks * specularLight * specular;
	}
	outColor = color;
}