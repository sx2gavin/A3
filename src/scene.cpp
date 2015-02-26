#include "scene.hpp"
#include <iostream>

SceneNode::SceneNode(const std::string& name)
  : m_name(name)
{
	picked = false;
}

SceneNode::~SceneNode()
{
}

void SceneNode::walk_gl(bool picking) const
{
  for (std::list<SceneNode*>::const_iterator it = m_children.begin(); it != m_children.end(); ++it) {
	  (*it)->set_shader_program(mProgram);
	  (*it)->set_parent_transform(m_parent_trans * m_trans);
	  (*it)->set_picked_names(pickedNames);
	  (*it)->set_joint(jointAngle, jointAxis);
      (*it)->walk_gl(picking);
  }	  
}

void SceneNode::set_shader_program(QGLShaderProgram *program) {
	mProgram = program;
}

void SceneNode::set_joint(const float jointAngle, const QVector3D jointAxis)
{
	this->jointAngle = jointAngle;
	this->jointAxis = jointAxis;
}

void SceneNode::reset_joint() {

  for (std::list<SceneNode*>::const_iterator it = m_children.begin(); it != m_children.end(); ++it) {
	  (*it)->reset_joint();
  }	  
}


void SceneNode::rotate(char axis, double angle)
{
	std::cerr << "Stub: Rotate " << m_name << " around " << axis << " by " << angle << std::endl;
	QVector3D _axis;
	QMatrix4x4 mat;
	if (axis == 'x') {
		_axis = QVector3D( 1.0, 0.0, 0.0);
	} else if (axis == 'y') {
		_axis = QVector3D( 0.0, 1.0, 0.0);
	} else if (axis == 'z') {
		_axis = QVector3D( 0.0, 0.0, 1.0);
	}
	mat.rotate(angle, _axis); 
	m_trans = m_trans * mat;
}

void SceneNode::scale(const Vector3D& amount)
{
  std::cerr << "Stub: Scale " << m_name << " by " << amount << std::endl;
  m_scale.scale(amount[0], amount[1], amount[2]);
}

void SceneNode::translate(const Vector3D& amount)
{
  std::cerr << "Stub: Translate " << m_name << " by " << amount << std::endl;
  QMatrix4x4 mat;
  mat.translate(amount[0], amount[1], amount[2]);
  m_trans = m_trans * mat;
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
  for (std::list<SceneNode*>::const_iterator it = m_children.begin(); it != m_children.end(); ++it) {
	  (*it)->set_picked(picked);
	  (*it)->set_shader_program(mProgram);
	  (*it)->set_parent_transform(m_parent_trans * m_trans * m_joint);
	  (*it)->set_picked_names(pickedNames);
	  (*it)->set_joint(jointAngle, jointAxis);
      (*it)->walk_gl(picking);
  }	  
}

bool JointNode::is_joint() const
{
  return true;
}

void JointNode::set_joint(const float angle, const QVector3D axis)
{
	jointAngle = angle;
	jointAxis = axis;

	for (int i = 0; i < pickedNames.size(); i++) {
		if (m_name == pickedNames[i]) {
			if (jointAxis.x() == -1.0) { // X axis	
				if (m_joint_x.init + jointAngle > m_joint_x.max) {
					jointAngle = m_joint_x.max - m_joint_x.init;
				} else if (m_joint_x.init + jointAngle < m_joint_x.min) {
					jointAngle = m_joint_x.init - m_joint_x.init;
				}	
				m_joint_x.init+=jointAngle;
				std::cerr << "x min= " << m_joint_x.min << ", x init= " << m_joint_x.init << ", x max= " << m_joint_x.max << std::endl;
			} else if (jointAxis.y() == 1.0) {
				if (m_joint_y.init + jointAngle > m_joint_y.max) {
					jointAngle = m_joint_y.max - m_joint_y.init;
				} else if (m_joint_y.init + jointAngle < m_joint_y.min) {
					jointAngle = m_joint_y.init - m_joint_y.init;
				}	
				m_joint_y.init+=jointAngle;
				std::cerr << "y min= " << m_joint_y.min << ", y init= " << m_joint_y.init << ", y max= " << m_joint_y.max << std::endl;
			}	
			m_joint.rotate(jointAngle, jointAxis);
			picked = true;
			return;
		}
	}
	picked = false;
}

void JointNode::reset_joint() {
	m_joint.setToIdentity();
	SceneNode::reset_joint();
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
	m_material->set_shader_program(mProgram);
	m_material->apply_gl();	
	m_primitive->set_shader_program(mProgram);
	m_primitive->set_transformation(m_parent_trans * m_trans);
	m_primitive->set_scale(m_scale);
	m_primitive->walk_gl(picked);
	
	SceneNode::walk_gl();
}
 
