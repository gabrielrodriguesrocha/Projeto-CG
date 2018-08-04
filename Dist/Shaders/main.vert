#version 300 es

precision highp float;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform mat3 normalMatrix;

out vec2 UV;
out vec3 eyeNormal;
out vec4 eyePosition;

void main() {
	eyeNormal = normalize(normalMatrix * normal);
	eyePosition =  viewMatrix * modelMatrix * vec4(position, 1.0);
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
	UV = uv;
}
