#version 330

in vec3 vert;
in vec3 normal;
uniform mat4 pMatrix;
uniform mat4 mMatrix;
uniform mat4 vMatrix;
uniform mat4 scaleMatrix;
uniform vec3 light_source;
uniform bool front_face_cull;
uniform bool back_face_cull;
out vec3 frag_normal;
out vec3 light;

void main()
{	
	vec4 n;
	if (front_face_cull) {
		n = vec4(-normal.x, -normal.y, -normal.z, 0.0);
	} else {
		n = vec4(normal, 0.0);
	}
	frag_normal = vec3(mMatrix * n);
	light = vec3(vMatrix * vec4(light_source, 0.0)); 
    gl_Position = pMatrix * vMatrix * mMatrix * scaleMatrix * vec4(vert, 1.0);
    // gl_Position = ftransform();
}
