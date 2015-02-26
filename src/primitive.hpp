#ifndef CS488_PRIMITIVE_HPP
#define CS488_PRIMITIVE_HPP

#include "algebra.hpp"
#include <QGLShaderProgram>
#include <QMatrix4x4>

class Primitive {
public:
  virtual ~Primitive();
  virtual void walk_gl(bool picking) const = 0;
  void set_shader_program(QGLShaderProgram *program);
  void set_transformation(QMatrix4x4 matrix);
  void set_scale(QMatrix4x4 scale);
protected:
  QGLShaderProgram *mProgram;
  QMatrix4x4 mTransformMatrix;
  QMatrix4x4 mScaleMatrix;
};

class Sphere : public Primitive {
public:
  virtual ~Sphere();
  virtual void walk_gl(bool picking) const;
};

#endif
