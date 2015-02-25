#ifndef CS488_VIEWER_HPP
#define CS488_VIEWER_HPP

#include <QGLWidget>
#include <QGLShaderProgram>
#include <QMatrix4x4>
#include <QVector4D>
#include <QtGlobal>
#include "scene.hpp"

#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#else 
#include <QGLBuffer>
#endif

class Viewer : public QGLWidget {
    
    Q_OBJECT

public:
	enum Mode {POSITION_ORIENTATION, JOINTS};
    Viewer(const QGLFormat& format, QWidget *parent = 0);
    virtual ~Viewer();
    
    QSize minimumSizeHint() const;
    QSize sizeHint() const;

	void setSceneNode(SceneNode* node);

    // If you want to render a new frame, call do not call paintGL(),
    // instead, call update() to ensure that the view gets a paint 
    // event.
  
protected:

    // Events we implement

    // Called when GL is first initialized
    virtual void initializeGL();
    // Called when our window needs to be redrawn
    virtual void paintGL();
    // Called when the window is resized (formerly on_configure_event)
    virtual void resizeGL(int width, int height);
    // Called when a mouse button is pressed
    virtual void mousePressEvent ( QMouseEvent * event );
    // Called when a mouse button is released
    virtual void mouseReleaseEvent ( QMouseEvent * event );
    // Called when the mouse moves
    virtual void mouseMoveEvent ( QMouseEvent * event );
    
    // Draw a circle for the trackball, with OpenGL commands.
    // Assumes the context for the viewer is active.
    void draw_trackball_circle();

private:
	void addVertax(QVector<float> *sphereVertices, QVector3D point);
	void addTriangle(QVector<float> *sphereVertices, QVector<QVector3D> vertices, int index_1, int index_2, int index_3);
	void addTriangle(QVector<float> *sphereVertices, QVector3D point_1, QVector3D point_2, QVector3D point_3);
	QVector3D getMiddlePoint(QVector3D point_1, QVector3D point_2);
	void createSphereGeometry();
	void draw_scene();

    QMatrix4x4 getCameraMatrix();
    void translateWorld(float x, float y, float z);
    void rotateWorld(float x, float y, float z);
    void scaleWorld(float x, float y, float z);
    void set_colour(const QColor& col);
	void vCalcRotVec(float fNewX, float fNewY,
                 float fOldX, float fOldY,
                 float fDiameter,
                 float *fVecX, float *fVecY, float *fVecZ);
	void vAxisRotMatrix(float fVecX, float fVecY, float fVecZ, QMatrix4x4 &mNewMat); 

#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
    QOpenGLBuffer mCircleBufferObject;
	QOpenGLBuffer mSphereBufferObject;
	QOpenGLBuffer mSphereNormalBufferObject;
    QOpenGLVertexArrayObject mVertexArrayObject;
#else 
    QGLBuffer mCircleBufferObject;
	QGLBuffer mSphereBufferObject;
	QGLBuffer mSphereNormalBufferObject;
#endif
    
    int mMvpMatrixLocation;
    int mColorLocation;
	int mLightLocation;

    QMatrix4x4 mPerspMatrix;
    QMatrix4x4 mTransformMatrix;
    QGLShaderProgram mProgram;
	
	// sphere
	int sphereQuality;

	// mouse
	Qt::MouseButton pressedMouseButton;
	QVector2D prePos;
	double diameter;

	// Scene
	SceneNode* root;	
};

#endif
