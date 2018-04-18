#version 300 es
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

precision mediump float;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform mat3 normalMatrix;

uniform vec3 ambientLightColor, directionalLight, materialSpecular;

uniform float materialAmbient, materialDiffuse, shininess;

out vec3 col;

/* A function to determine the colour of a vertex, accounting
   for ambient and directional light */
vec3 ads( vec4 position, vec3 norm )
{
	vec3 s = normalize(vec3(vec4(directionalLight,1.0) - position));
	vec3 v = normalize(vec3(-position));
	vec3 r = reflect(-s, norm);
	return ambientLightColor +
	materialDiffuse * max(dot(s,norm), 0.0) +
	materialSpecular * pow(max(dot(r,v), 0.0), shininess);
}

void main() {
	vec3 eyeNormal = normalize(normalMatrix * normal);
	vec4 eyePosition =  viewMatrix * modelMatrix * vec4(position, 1.0);
	col = min(vec3(0.0) + ads(eyePosition, eyeNormal), 1.0);
	gl_Position = projectionMatrix * viewMatrix * modelMatrix *
	vec4(position, 1.0); 
}
