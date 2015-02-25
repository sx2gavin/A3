#version 130

in vec3 vert;
in vec3 normal;
uniform mat4 mvpMatrix;
varying vec3 frag_normal;
void main()
{	
	frag_normal = normal;
    gl_Position = mvpMatrix * vec4(vert, 1.0);
    // gl_Position = ftransform();
}
