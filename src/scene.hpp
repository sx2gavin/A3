#ifndef SCENE_HPP
#define SCENE_HPP

#include <list>
#include <QGLShaderProgram>
#include "algebra.hpp"
#include "primitive.hpp"
#include "material.hpp"
#include <QMatrix4x4>
#include <QVector3D>

class SceneNode {
public:
  SceneNode(const std::string& name);
  virtual ~SceneNode();

  virtual void walk_gl(bool picking = false) const;

  const QMatrix4x4& get_transform() const { return m_trans; }
  const QMatrix4x4& get_inverse() const { return m_invtrans; }
  const QMatrix4x4& get_scale() const { return m_scale; }
  
  void set_transform(const QMatrix4x4& m)
  {
    m_trans = m;
    m_invtrans = m.inverted();
  }

  void set_transform(const QMatrix4x4& m, const QMatrix4x4& i)
  {
    m_trans = m;
    m_invtrans = i;
  }

  void set_parent_transform(const QMatrix4x4& m)
  {
    m_parent_trans = m;
	m_parent_invtrans = m.inverted();
  }

  void set_picked_names(const QVector<std::string> picked)
  {
	pickedNames = picked;
  }

  void set_picked(bool picked)
  {
	  this->picked = picked;
  }

  virtual void set_joint(const float angle, const QVector3D axis);

  void set_shader_program(QGLShaderProgram *program);

  void add_child(SceneNode* child)
  {
    m_children.push_back(child);
  }

  void remove_child(SceneNode* child)
  {
    m_children.remove(child);
  }

  // Callbacks to be implemented.
  // These will be called from Lua.
  void rotate(char axis, double angle);
  void scale(const Vector3D& amount);
  void translate(const Vector3D& amount);

  // Returns true if and only if this node is a JointNode
  virtual bool is_joint() const;
  
protected:
  
  // Useful for picking
  int m_id;
  std::string m_name;
  QVector<std::string> pickedNames; 
  bool picked;

  // Transformations
  QMatrix4x4 m_trans;
  QMatrix4x4 m_invtrans;
  QMatrix4x4 m_scale;
  QMatrix4x4 m_joint;
  float jointAngle;
  QVector3D jointAxis;

  // Whole Transformation 
  QMatrix4x4 m_parent_trans;
  QMatrix4x4 m_parent_invtrans;

  // Hierarchy
  typedef std::list<SceneNode*> ChildList;
  ChildList m_children;

  // shader program
  QGLShaderProgram *mProgram;

};

class JointNode : public SceneNode {
public:
  JointNode(const std::string& name);
  virtual ~JointNode();

  virtual void walk_gl(bool bicking = false) const;

  virtual bool is_joint() const;
  virtual void set_joint(const float jointAngle, const QVector3D jointAxis);

  void set_joint_x(double min, double init, double max);
  void set_joint_y(double min, double init, double max);

  struct JointRange {
    double min, init, max;
  };

  
protected:
  JointRange m_joint_x, m_joint_y;
};

class GeometryNode : public SceneNode {
public:
  GeometryNode(const std::string& name,
               Primitive* primitive);
  virtual ~GeometryNode();

  virtual void walk_gl(bool picking = false) const;

  const Material* get_material() const;
  Material* get_material();

  void set_material(Material* material)
  {
    m_material = material;
  }

protected:
  Material* m_material;
  Primitive* m_primitive;
};

#endif
