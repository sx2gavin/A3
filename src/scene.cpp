#include "scene.hpp"
#include <iostream>

SceneNode::SceneNode(const std::string& name)
  : m_name(name)
{
}

SceneNode::~SceneNode()
{
}

void SceneNode::walk_gl(bool picking) const
{
  for (std::list<SceneNode*>::const_iterator it = m_children.begin(); it != m_children.end(); ++it) {
	  (*it)->set_shader_program(mProgram);
	  (*it)->set_parent_transform(m_parent_trans * m_trans);
      (*it)->walk_gl(picking);
  }	  
}

void SceneNode::set_shader_program(QGLShaderProgram *program) {
	mProgram = program;
}

void SceneNode::rotate(char axis, double angle)
{
	std::cerr << "Stub: Rotate " << m_name << " around " << axis << " by " << angle << std::endl;
	QVector3D _axis;
	if (axis == 'x') {
		_axis = QVector3D( 1.0, 0.0, 0.0);
	} else if (axis == 'y') {
		_axis = QVector3D( 0.0, 1.0, 0.0);
	} else if (axis == 'z') {
		_axis = QVector3D( 0.0, 0.0, 1.0);
	}
	m_trans.rotate(angle, _axis); 
}

void SceneNode::scale(const Vector3D& amount)
{
  std::cerr << "Stub: Scale " << m_name << " by " << amount << std::endl;
  m_trans.scale(amount[0], amount[1], amount[2]);
}

void SceneNode::translate(const Vector3D& amount)
{
  std::cerr << "Stub: Translate " << m_name << " by " << amount << std::endl;
  m_trans.translate(amount[0], amount[1], amount[2]);
}

bool SceneNode::is_joint() const
{
  return false;
}

JointNode::JointNode(const std::string& name)
  : SceneNode(name)
{
}

JointNode::~JointNode()
{
}

void JointNode::walk_gl(bool picking) const
{
	SceneNode::walk_gl(picking);
}

bool JointNode::is_joint() const
{
  return true;
}

void JointNode::set_joint_x(double min, double init, double max)
{
  m_joint_x.min = min;
  m_joint_x.init = init;
  m_joint_x.max = max;
}

void JointNode::set_joint_y(double min, double init, double max)
{
  m_joint_y.min = min;
  m_joint_y.init = init;
  m_joint_y.max = max;
}

GeometryNode::GeometryNode(const std::string& name, Primitive* primitive)
  : SceneNode(name),
    m_primitive(primitive)
{
}

GeometryNode::~GeometryNode()
{
}

void GeometryNode::walk_gl(bool picking) const
{
	m_primitive->set_shader_program(mProgram);
	m_primitive->set_transformation(m_parent_trans * m_trans);
	m_primitive->walk_gl(picking);
	SceneNode::walk_gl(picking);
}
 
