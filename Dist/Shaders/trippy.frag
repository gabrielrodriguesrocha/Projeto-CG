#version 300 es

precision highp float;

in vec2 UV;
in vec3 eyeNormal;
in vec4 eyePosition;

uniform mat3 normalMatrix;

uniform vec3 ambientLightColor, directionalLight, materialSpecular;

uniform float materialAmbient, materialDiffuse, shininess;

uniform sampler2D myTextureSampler;

out vec4 outputColor;
vec2 iResolution = vec2(1280.0, 800.0);

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

void main(void)
{ 
    vec2 uv = gl_FragCoord.xy / iResolution.xy;
    uint x = uint(uv.x * 1280.0);
    uint y = uint(uv.y * 800.0);
    
    uint z = 0u;
    uint xdist, ydist;
    for(int i = 0; i < 256; ++i)
    {
        xdist = ((x - 160u) * z + 4096u) >> 8;
        ydist = ((y - 100u) * z + 4096u) >> 8;
        switch(z / 64u)
        {
        case 0u: xdist -= 10u; break;
        case 1u: xdist -= 40u; break;
        case 2u: xdist -= 20u; break;
        case 3u: xdist -= 30u; break;
        }
        if ((xdist >=32u && ((z & 32u) != 0u)) || (ydist >= 32u))
            break;
        ++z;
    }

    uint texel = (xdist & 255u) ^ (ydist & 255u) ^ z;
    texel %= 16u;
    float c = float(texel) / 16.0;
    outputColor = vec4(texture( myTextureSampler, UV ).rgb, 1.0f);
}