#version 330 

uniform vec3 frag_color;
out vec4 finalColor;
in vec3 frag_normal;
in vec3 light;

void main()
{
	vec3 n = normalize(frag_normal);
	vec3 l = normalize(light);
	vec3 e = vec3(0.0, 0.0, 1.0);
	vec3 r = reflect(-l, n);
 
	float cosTheta = clamp( dot(n, l), 0, 1);	
	float cosAlpha = clamp( dot(e, r), 0, 1);
    finalColor = vec4(frag_color * cosTheta,  1.0);
}
