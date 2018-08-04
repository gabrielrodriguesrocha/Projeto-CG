#version 300 es

precision highp float;

in vec2 UV;
in vec3 eyeNormal;
in vec4 eyePosition;

uniform mat3 normalMatrix;

uniform vec3 ambientLightColor, directionalLight, materialSpecular;

uniform float materialAmbient, materialDiffuse, shininess;

uniform sampler2D myTextureSampler;

out vec4 FragColor;

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

void main()
{
	FragColor = ( vec4(min(vec3(0.0) + ads(eyePosition, eyeNormal), 1.0), 1.0f) + vec4(texture( myTextureSampler, UV ).rgb, 1.0f) ) / 2.0f;
	//FragColor = texture( myTextureSampler, UV ).rgb;
} 
