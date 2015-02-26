#include "primitive.hpp"
#include <cmath>
#include <QtOpenGL>
#include <iostream>

Primitive::~Primitive()
{
}

Sphere::~Sphere()
{
}

void Primitive::set_shader_program(QGLShaderProgram *program) 
{
	mProgram = program;
}

void Primitive::set_transformation(QMatrix4x4 matrix)
{
	mTransformMatrix = matrix;
}

void Primitive::set_scale(QMatrix4x4 scale) 
{
	mScaleMatrix = scale;
}

void Sphere::walk_gl(bool picking) const
{
	int mModelMLocation = mProgram->uniformLocation("mMatrix"); 
	int mScaleLocation = mProgram->uniformLocation("scaleMatrix");
	mProgram->setUniformValue(mModelMLocation, mTransformMatrix); 
	mProgram->setUniformValue(mScaleLocation, mScaleMatrix);
	glDrawArrays(GL_TRIANGLES, 0, 20 * 4 * 4 * 3);
}
