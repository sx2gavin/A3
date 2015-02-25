#version 130 

uniform vec3 frag_color;
uniform vec3 light_source;
out vec4 finalColor;
varying vec3 frag_normal;

void main()
{
	vec3 n = normalize(frag_normal);
	vec3 l = normalize(light_source);
	vec3 e = vec3(0.0, 0.0, 1.0);
	vec3 r = reflect(-l, n);
 
	float cosTheta = clamp( dot(n, l), 0, 1);	
	float cosAlpha = clamp( dot(e, r), 0, 1);
    finalColor = vec4(frag_color * cosTheta + frag_color * pow(cosAlpha, 10),  1.0);
}
