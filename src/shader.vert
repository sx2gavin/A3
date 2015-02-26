#version 330

in vec3 vert;
in vec3 normal;
uniform mat4 pMatrix;
uniform mat4 mMatrix;
uniform mat4 vMatrix;
uniform mat4 scaleMatrix;
uniform vec3 light_source;
out vec3 frag_normal;
out vec3 light;
void main()
{	
	frag_normal = vec3(mMatrix * vec4(normal, 0.0));
	light = vec3(vMatrix * vec4(light_source, 0.0)); 
    gl_Position = pMatrix * vMatrix * mMatrix * scaleMatrix * vec4(vert, 1.0);
    // gl_Position = ftransform();
}
