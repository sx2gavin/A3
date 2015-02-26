#version 330 

uniform vec3 frag_color;
uniform bool front_face_cull;
uniform bool back_face_cull;
out vec4 finalColor;
in vec3 frag_normal;
in vec3 light;

void main()
{
	vec3 n = normalize(frag_normal);
	vec3 l = normalize(light);
	  if (front_face_cull || back_face_cull) {
	  	float cosTheta = clamp( dot(n, l), -5, 1);
	  	if (cosTheta > 0) {
	  		finalColor = vec4(frag_color * cosTheta,  1.0);
	  	} else {
	  		// finalColor = vec4(0.4, 0.4, 0.4, 0.0);
	  	}
	  } else {
		float cosTheta = clamp( dot(n, l), 0, 1);	
		finalColor = vec4(frag_color * cosTheta,  1.0);
	}
}
