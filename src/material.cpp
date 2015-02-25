#include "material.hpp"


Material::~Material()
{
}

void Material::set_shader_program(QGLShaderProgram *program) {
	mProgram = program;
}

PhongMaterial::PhongMaterial(const Colour& kd, const Colour& ks, double shininess)
  :m_shininess(shininess)
{
	m_kd = QColor(kd.R(), kd.G(), kd.B());
	m_ks = QColor(ks.R(), ks.G(), ks.B());	
}

PhongMaterial::~PhongMaterial()
{
}

void PhongMaterial::apply_gl() const
{
  // Perform OpenGL calls necessary to set up this material.
  int mColorLocation = mProgram->uniformLocation("frag_color");
  mProgram->setUniformValue(mColorLocation, m_kd.red(), m_kd.green(), m_kd.blue());

}
