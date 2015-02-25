#ifndef CS488_MATERIAL_HPP
#define CS488_MATERIAL_HPP

#include "algebra.hpp"
#include <QColor>
#include <QGLShaderProgram>

class Material {
public:
  virtual ~Material();
  virtual void apply_gl() const = 0;
  void set_shader_program(QGLShaderProgram *program);

protected:
  Material()
  {
  }

  QGLShaderProgram *mProgram;
};

class PhongMaterial : public Material {
public:
  PhongMaterial(const Colour& kd, const Colour& ks, double shininess);
  virtual ~PhongMaterial();

  virtual void apply_gl() const;

private:
  QColor m_kd;
  QColor m_ks;

  double m_shininess;
};


#endif
