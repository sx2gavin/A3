#include <QtWidgets>
#include <QtOpenGL>
#include "Viewer.hpp"
#include <iostream>
#include <math.h>


#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE 0x809D
#endif

Viewer::Viewer(const QGLFormat& format, QWidget *parent) 
    : QGLWidget(format, parent) 
#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
    , mCircleBufferObject(QOpenGLBuffer::VertexBuffer)
    , mVertexArrayObject(this)
#else 
    , mCircleBufferObject(QGLBuffer::VertexBuffer)
#endif
{
	sphereQuality = 2; // recursionlevel of refining the sphere.
	b_trackball_circle = false;
	b_z_buffer = false;
	b_back_face_cull = false;
	b_front_face_cull = false;
	mode = POSITION_ORIENTATION;
}

Viewer::~Viewer() {
    // Nothing to do here right now.
}

QSize Viewer::minimumSizeHint() const {
    return QSize(50, 50);
}

QSize Viewer::sizeHint() const {
    return QSize(300, 300);
}

void Viewer::setSceneNode(SceneNode* node) {
	std::cerr << "setSceneNode in Viewer called." << std::endl;
	root = node;
	if (root == NULL) {
		std::cerr << "ERROR: root is NULL" << std::endl;
	}
}

void Viewer::togglePickedName(std::string name)
{
	for (int i = 0; i < pickedNames.size(); i++) {
		if (pickedNames[i] == name) {
			pickedNames.remove(i);
			update();
			return;
		}
	}
	pickedNames.push_back(name);
	update();
}	

void Viewer::initializeGL() {
    QGLFormat glFormat = QGLWidget::format();
    if (!glFormat.sampleBuffers()) {
        std::cerr << "Could not enable sample buffers." << std::endl;
        return;
    }

    glShadeModel(GL_SMOOTH);
    glClearColor( 0.4, 0.4, 0.4, 0.0 );
    
    if (!mProgram.addShaderFromSourceFile(QGLShader::Vertex, "shader.vert")) {
        std::cerr << "Cannot load vertex shader." << std::endl;
        return;
    }

    if (!mProgram.addShaderFromSourceFile(QGLShader::Fragment, "shader.frag")) {
        std::cerr << "Cannot load fragment shader." << std::endl;
        return;
    }

    if ( !mProgram.link() ) {
        std::cerr << "Cannot link shaders." << std::endl;
        return;
    }

    float circleData[120];

    double radius = width() < height() ? 
        (float)width() * 0.25 : (float)height() * 0.25;
	diameter = 2 * radius;
        
    for(size_t i=0; i<40; ++i) {
        circleData[i*3] = radius * cos(i*2*M_PI/40);
        circleData[i*3 + 1] = radius * sin(i*2*M_PI/40);
        circleData[i*3 + 2] = 0.0;
    }


#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
    mVertexArrayObject.create();
    mVertexArrayObject.bind();

    mCircleBufferObject.create();
    mCircleBufferObject.setUsagePattern(QOpenGLBuffer::StaticDraw);

	mSphereBufferObject.create();
	mSphereBufferObject.setUsagePattern(QOpenGLBuffer::StaticDraw);

	mSphereNormalBufferObject.create();
	mSphereNormalBufferObject.setUsagePattern(QOpenGLBuffer::StaticDraw);
#else 
    /*
     * if qt version is less than 5.1, use the following commented code
     * instead of QOpenGLVertexVufferObject. Also use QGLBuffer instead of 
     * QOpenGLBuffer.
     */
    uint vao;
     
    typedef void (APIENTRY *_glGenVertexArrays) (GLsizei, GLuint*);
    typedef void (APIENTRY *_glBindVertexArray) (GLuint);
     
    _glGenVertexArrays glGenVertexArrays;
    _glBindVertexArray glBindVertexArray;
     
    glGenVertexArrays = (_glGenVertexArrays) QGLWidget::context()->getProcAddress("glGenVertexArrays");
    glBindVertexArray = (_glBindVertexArray) QGLWidget::context()->getProcAddress("glBindVertexArray");
     
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);    

    mCircleBufferObject.create();
    mCircleBufferObject.setUsagePattern(QGLBuffer::StaticDraw);

	mSphereBufferObject.create();
	mSphereBufferObject.setUsagePattern(QGLBuffer::StaticDraw);

	mSphereNormalBufferObject.create();
	mSphereNormalBufferObject.setUsagePattern(QGLBuffer::StaticDraw);
#endif

    if (!mCircleBufferObject.bind()) {
        std::cerr << "could not bind vertex buffer to the context." << std::endl;
        return;
    }

    mCircleBufferObject.allocate(circleData, 40 * 3 * sizeof(float));

	createSphereGeometry();

    // mProgram.bind();
    // mProgram.enableAttributeArray("vert");
    // mProgram.setAttributeBuffer("vert", GL_FLOAT, 0, 3);

    mPerspMLocation = mProgram.uniformLocation("pMatrix");
    mColorLocation = mProgram.uniformLocation("frag_color");
	mLightLocation = mProgram.uniformLocation("light_source");
	mModelMLocation = mProgram.uniformLocation("mMatrix");
	mViewMLocation = mProgram.uniformLocation("vMatrix");


}

void Viewer::paintGL() {

	if (b_z_buffer) {
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
	} else {
		glDisable(GL_DEPTH_TEST);
	}

	int backFaceCullLocation = mProgram.uniformLocation("back_face_cull");
	mProgram.setUniformValue(backFaceCullLocation, b_back_face_cull);

	if (b_back_face_cull) {
		glFrontFace(GL_CCW);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	} else {
		glDisable(GL_CULL_FACE);
	}

	int frontFaceCullLocation = mProgram.uniformLocation("front_face_cull");
	mProgram.setUniformValue(frontFaceCullLocation, b_front_face_cull);
	if (b_front_face_cull) {
		glFrontFace(GL_CCW);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
	} else {
		glDisable(GL_CULL_FACE);
	}

    // Clear framebuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set up lighting
	
	QVector3D light(0.0, 0.0, 20.0); 

	mProgram.setUniformValue(mLightLocation, light);
	mProgram.setUniformValue(mViewMLocation, mViewMatrix);
	mProgram.setUniformValue(mPerspMLocation, mPerspMatrix);

    // Draw stuff
	
	draw_scene();
	if (b_trackball_circle) {
		draw_trackball_circle();
	}

}

void Viewer::resizeGL(int width, int height) {
    if (height == 0) {
        height = 1;
    }

    mPerspMatrix.setToIdentity();
    mPerspMatrix.perspective(30.0, (float) width / (float) height, 0.001, 1000);

	mViewMatrix.setToIdentity();
    QVector3D cameraPosition = QVector3D(0, 0, 20.0);
    QVector3D cameraUpDirection = QVector3D(0, 1, 0);
    mViewMatrix.lookAt(cameraPosition, QVector3D(0, 0, 0), cameraUpDirection);

    glViewport(0, 0, width, height);
}

void Viewer::mousePressEvent ( QMouseEvent * event ) {
    std::cerr << "Stub: button " << event->button() << " pressed\n";
	pressedMouseButton = event->button();
	prePos = QVector2D(event->x(), event->y());

}

void Viewer::mouseReleaseEvent ( QMouseEvent * event ) {
    std::cerr << "Stub: button " << event->button() << " released\n";	
}

void Viewer::mouseMoveEvent ( QMouseEvent * event ) {
    std::cerr << "Stub: Motion at " << event->x() << ", " << event->y() << std::endl;
	QMatrix4x4 transformMat;
	if (mode == POSITION_ORIENTATION) {
		if (pressedMouseButton == Qt::LeftButton) {
			transformMat.translate((event->x()-prePos.x()) / 100.0, (prePos.y()-event->y()) / 100.0, 0.0);
			mTransformMatrix = transformMat * mTransformMatrix;
		} else if (pressedMouseButton == Qt::MidButton) {
			transformMat.translate(0.0, 0.0, (event->y() - prePos.y())/100.0);
			mTransformMatrix = transformMat * mTransformMatrix;
		} else if (pressedMouseButton == Qt::RightButton) {
			float fVecX;
			float fVecY;
			float fVecZ;
			QMatrix4x4 rotationMat;
			vCalcRotVec((float)event->x() - width() / 2,  
						height() / 2 - (float)event->y(), 
						(float)prePos.x() - width() / 2,  
						height() / 2 - (float)prePos.y(), 
						(float)diameter, 
						&fVecX, &fVecY, &fVecZ); 

			vAxisRotMatrix(fVecX, fVecY, fVecZ, transformMat); 
			mTransformMatrix = transformMat * mTransformMatrix;
		}
	} else if (mode == JOINTS) {
		if (pressedMouseButton == Qt::MidButton) {
			jointAxis =  QVector3D(0.0, 1.0, 0.0);
			jointAngle = (event->y() - prePos.y())/10.0;
			// mJointTransformation.rotate((event->y() - prePos.y())/10.0, QVector3D(0.0, 1.0, 0.0));
		} else if (pressedMouseButton == Qt::RightButton) {
			jointAxis = QVector3D(-1.0, 0.0, 0.0);
			jointAngle = (event->x() - prePos.x())/10.0;
			// mJointTransformation.rotate((event->x() - prePos.x())/10.0, QVector3D(1.0, 0.0, 0.0));
		}
	}
	prePos.setX(event->x());
	prePos.setY(event->y());

	update();

}

void Viewer::vCalcRotVec(float fNewX, float fNewY,
                 float fOldX, float fOldY,
                 float fDiameter,
                 float *fVecX, float *fVecY, float *fVecZ) {
   long  nXOrigin, nYOrigin;
   float fNewVecX, fNewVecY, fNewVecZ,        /* Vector corresponding to new mouse location */
         fOldVecX, fOldVecY, fOldVecZ,        /* Vector corresponding to old mouse location */
         fLength;

   /* Vector pointing from center of virtual trackball to
    * new mouse position
    */
   fNewVecX    = fNewX * 2.0 / fDiameter;
   fNewVecY    = fNewY * 2.0 / fDiameter;
   fNewVecZ    = (1.0 - fNewVecX * fNewVecX - fNewVecY * fNewVecY);
   
   /* If the Z component is less than 0, the mouse point
    * falls outside of the trackball which is interpreted
    * as rotation about the Z axis.
    */
   if (fNewVecZ < 0.0) {
      fLength = sqrt(1.0 - fNewVecZ);
      fNewVecZ  = 0.0;
      fNewVecX /= fLength;
      fNewVecY /= fLength;
   } else {
      fNewVecZ = sqrt(fNewVecZ);
   }

   /* Vector pointing from center of virtual trackball to
    * old mouse position
    */
   fOldVecX    = fOldX * 2.0 / fDiameter;
   fOldVecY    = fOldY * 2.0 / fDiameter;
   fOldVecZ    = (1.0 - fOldVecX * fOldVecX - fOldVecY * fOldVecY);
 
   /* If the Z component is less than 0, the mouse point
    * falls outside of the trackball which is interpreted
    * as rotation about the Z axis.
    */
   if (fOldVecZ < 0.0) {
      fLength = sqrt(1.0 - fOldVecZ);
      fOldVecZ  = 0.0;
      fOldVecX /= fLength;
      fOldVecY /= fLength;
   } else {
      fOldVecZ = sqrt(fOldVecZ);
   }

   /* Generate rotation vector by calculating cross product:
    * 
    * fOldVec x fNewVec.
    * 
    * The rotation vector is the axis of rotation
    * and is non-unit length since the length of a crossproduct
    * is related to the angle between fOldVec and fNewVec which we need
    * in order to perform the rotation.
    */
   *fVecX = fOldVecY * fNewVecZ - fNewVecY * fOldVecZ;
   *fVecY = fOldVecZ * fNewVecX - fNewVecZ * fOldVecX;
   *fVecZ = fOldVecX * fNewVecY - fNewVecX * fOldVecY;
}

void Viewer::vAxisRotMatrix(float fVecX, float fVecY, float fVecZ, QMatrix4x4 &mNewMat) 
{

    float fRadians, fInvLength, fNewVecX, fNewVecY, fNewVecZ;

    /* Find the length of the vector which is the angle of rotation
     * (in radians)
     */
    fRadians = sqrt(fVecX * fVecX + fVecY * fVecY + fVecZ * fVecZ);

    /* If the vector has zero length - return the identity matrix */
    if (fRadians > -0.000001 && fRadians < 0.000001) {
		mNewMat.setToIdentity();
        return;
    }

    /* Normalize the rotation vector now in preparation for making
     * rotation matrix. 
     */
    fInvLength = 1 / fRadians;
    fNewVecX   = fVecX * fInvLength;
    fNewVecY   = fVecY * fInvLength;
    fNewVecZ   = fVecZ * fInvLength;

    /* Create the arbitrary axis rotation matrix */
    double dSinAlpha = sin(fRadians);
    double dCosAlpha = cos(fRadians);
    double dT = 1 - dCosAlpha;

	mNewMat.setColumn(0, QVector4D( 
		dCosAlpha + fNewVecX*fNewVecX*dT,
		fNewVecX*fNewVecY*dT + fNewVecZ*dSinAlpha,
		fNewVecX*fNewVecZ*dT - fNewVecY*dSinAlpha,
		0));

	mNewMat.setColumn(1, QVector4D(
		fNewVecX*fNewVecY*dT - dSinAlpha*fNewVecZ,
		dCosAlpha + fNewVecY*fNewVecY*dT,
		fNewVecY*fNewVecZ*dT + dSinAlpha*fNewVecX,
		0));

	mNewMat.setColumn(2, QVector4D(
		fNewVecZ*fNewVecX*dT + dSinAlpha*fNewVecY,
		fNewVecZ*fNewVecY*dT - dSinAlpha*fNewVecX,
		dCosAlpha + fNewVecZ*fNewVecZ*dT,
		0));

	mNewMat.setColumn(3, QVector4D( 0, 0, 0, 1));
}	

void Viewer::createSphereGeometry() {

	QVector<float> sphereVertices;	
	QVector<QVector3D> vertices;
	vertices.resize(12);
	float t = (1.0 + std::sqrt(5.0)) / 2.0;

	vertices[0] = QVector3D(-1.0, t, 0.0);
	vertices[1] = QVector3D(1.0, t, 0.0);
	vertices[2] = QVector3D(-1.0, -t, 0.0);
	vertices[3] = QVector3D(1.0, -t, 0.0);

	vertices[4] = QVector3D(0.0, -1.0, t);
	vertices[5] = QVector3D(0.0, 1.0, t);
	vertices[6] = QVector3D(0.0, -1.0, -t);
	vertices[7] = QVector3D(0.0, 1.0, -t);

	vertices[8] = QVector3D(t, 0.0, -1.0);
	vertices[9] = QVector3D(t, 0.0, 1.0);
	vertices[10] = QVector3D(-t, 0.0, -1.0);
	vertices[11] = QVector3D(-t, 0.0, 1.0);	

 	addTriangle(&sphereVertices, vertices, 0, 11, 5);
 	addTriangle(&sphereVertices, vertices, 0, 5, 1);
 	addTriangle(&sphereVertices, vertices, 0, 1, 7);
 	addTriangle(&sphereVertices, vertices, 0, 7, 10);
 	addTriangle(&sphereVertices, vertices, 0, 10, 11);

 	addTriangle(&sphereVertices, vertices, 1, 5, 9);
 	addTriangle(&sphereVertices, vertices, 5, 11, 4);
 	addTriangle(&sphereVertices, vertices, 11, 10, 2);
 	addTriangle(&sphereVertices, vertices, 10, 7, 6);
 	addTriangle(&sphereVertices, vertices, 7, 1, 8);

 	addTriangle(&sphereVertices, vertices, 3, 9, 4);
 	addTriangle(&sphereVertices, vertices, 3, 4, 2);
 	addTriangle(&sphereVertices, vertices, 3, 2, 6);
 	addTriangle(&sphereVertices, vertices, 3, 6, 8);
 	addTriangle(&sphereVertices, vertices, 3, 8, 9);

 	addTriangle(&sphereVertices, vertices, 4, 9, 5);
 	addTriangle(&sphereVertices, vertices, 2, 4, 11);
 	addTriangle(&sphereVertices, vertices, 6, 2, 10);
 	addTriangle(&sphereVertices, vertices, 8, 6, 7);
 	addTriangle(&sphereVertices, vertices, 9, 8, 1);

	// refine triangles
	for (int i = 0; i < sphereQuality; i++) {
		QVector<float> newSphereVertices;
		for (int j = 0; j < sphereVertices.size(); j+=9) {
			QVector3D a(sphereVertices[j], sphereVertices[j+1], sphereVertices[j+2]);
			QVector3D b(sphereVertices[j+3], sphereVertices[j+4], sphereVertices[j+5]);	
			QVector3D c(sphereVertices[j+6], sphereVertices[j+7], sphereVertices[j+8]);
				
			QVector3D m_ab = getMiddlePoint(a, b);
			QVector3D m_bc = getMiddlePoint(c, b);
			QVector3D m_ac = getMiddlePoint(a, c);

			addTriangle(&newSphereVertices, a, m_ab, m_ac);
			addTriangle(&newSphereVertices, b, m_bc, m_ab);
			addTriangle(&newSphereVertices, c, m_ac, m_bc);
			addTriangle(&newSphereVertices, m_bc, m_ac, m_ab);
		}
		sphereVertices.clear();
		sphereVertices = newSphereVertices;
	}

    if (!mSphereBufferObject.bind()) {
        std::cerr << "could not bind vertex buffer to the context." << std::endl;
        return;
    }

	mSphereBufferObject.allocate(sphereVertices.constData(), sphereVertices.size() * sizeof(float));


	if (!mSphereNormalBufferObject.bind()) {
        std::cerr << "could not bind sphere normal buffer object to the context." << std::endl;
        return;
	}
		
	mSphereNormalBufferObject.allocate(sphereVertices.constData(), sphereVertices.size() * sizeof(float));

}


void Viewer::addVertax(QVector<float> *sphereVertices, QVector3D point) {
	// unit circle
	// force all the vertices to be on the sphere.
	double length = sqrt(point.x() * point.x() + point.y() * point.y() + point.z() * point.z());

	sphereVertices->push_back(point.x() / length);
	sphereVertices->push_back(point.y() / length);
	sphereVertices->push_back(point.z() / length);
}

void Viewer::addTriangle(QVector<float> *sphereVertices, QVector<QVector3D> vertices, int index_1, int index_2, int index_3) {
	addVertax(sphereVertices, vertices[index_1]);
	addVertax(sphereVertices, vertices[index_2]);
	addVertax(sphereVertices, vertices[index_3]);
}

void Viewer::addTriangle(QVector<float> *sphereVertices, QVector3D point_1, QVector3D point_2, QVector3D point_3) {
	addVertax(sphereVertices, point_1);
	addVertax(sphereVertices, point_2);
	addVertax(sphereVertices, point_3);
}


QVector3D Viewer::getMiddlePoint(QVector3D point_1, QVector3D point_2) {
	QVector3D middle;
	middle.setX(point_1.x() + point_2.x());
	middle.setY(point_1.y() + point_2.y());
	middle.setZ(point_1.z() + point_2.z());
	return middle;
}
QMatrix4x4 Viewer::getCameraMatrix() {
    return mPerspMatrix * mViewMatrix * mTransformMatrix;
}

void Viewer::translateWorld(float x, float y, float z) {
    // Todo: Ask if we want to keep this.
    mTransformMatrix.translate(x, y, z);
}

void Viewer::rotateWorld(float x, float y, float z) {
    // Todo: Ask if we want to keep this.
    mTransformMatrix.rotate(x, y, z);
}

void Viewer::scaleWorld(float x, float y, float z) {
    // Todo: Ask if we want to keep this.
    mTransformMatrix.scale(x, y, z);
}

void Viewer::set_colour(const QColor& col)
{
  mProgram.setUniformValue(mColorLocation, col.red(), col.green(), col.blue());
}

void Viewer::draw_trackball_circle()
{
    int current_width = width();
    int current_height = height();

    // Set up for orthogonal drawing to draw a circle on screen.
    // You'll want to make the rest of the function conditional on
    // whether or not we want to draw the circle this time around.

    set_colour(QColor(0.0, 0.0, 0.0));

    // Set orthographic Matrix
    QMatrix4x4 orthoMatrix;
	QMatrix4x4 viewMatrix;
    orthoMatrix.ortho(0.0, (float)current_width, 
           0.0, (float)current_height, -0.1, 0.1);

    // Translate the view to the middle
    QMatrix4x4 transformMatrix;
	QMatrix4x4 scale;
	int mScaleLocation = mProgram.uniformLocation("scaleMatrix");
    transformMatrix.translate(width()/2.0, height()/2.0, 0.0);

    // Bind buffer object
    mCircleBufferObject.bind();
	
    mProgram.bind();
    mProgram.enableAttributeArray("vert");
    mProgram.setAttributeBuffer("vert", GL_FLOAT, 0, 3);

    mProgram.setUniformValue(mPerspMLocation, orthoMatrix);
	mProgram.setUniformValue(mViewMLocation, viewMatrix);
	mProgram.setUniformValue(mModelMLocation, transformMatrix);
	mProgram.setUniformValue(mScaleLocation, scale);

    // Draw buffer
    glDrawArrays(GL_LINE_LOOP, 0, 40);    
}

void Viewer::draw_scene()
{
	set_colour(QColor(1.0, 0.0, 0.0));
	mSphereBufferObject.bind();
	
    mProgram.bind();
	mProgram.enableAttributeArray("vert");
	mProgram.setAttributeBuffer("vert", GL_FLOAT, 0, 3);

	mSphereNormalBufferObject.bind();

	mProgram.bind();
	mProgram.enableAttributeArray("normal");
	mProgram.setAttributeBuffer("normal", GL_FLOAT, 0, 3);

	// mProgram.setUniformValue(mMvpMatrixLocation, getCameraMatrix());
	// Draw buffer 
	// glDrawArrays(GL_TRIANGLES, 0, 20 * 4 * 4 * 3);
	root->set_shader_program(&mProgram);
	root->set_parent_transform(mTransformMatrix);
	if (mode == POSITION_ORIENTATION){
	   jointAngle = 0;
	}	   
	root->set_picked_names(pickedNames);
	root->set_joint(jointAngle, jointAxis);
	root->walk_gl(true);	
}
