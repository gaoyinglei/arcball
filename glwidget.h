#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include "qopengl.h"
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QMatrix4x4>

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
Q_OBJECT

public:
	GLWidget(QWidget *parent = 0);
	~GLWidget();

	void initializeGL() override;
	void paintGL() override;
	void resizeGL(int width, int height) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void wheelEvent(QWheelEvent *event) override;
	QSize minimumSizeHint() const override;
	void arcBall(QPoint last_pnt, QPoint curr_pnt);
	void process_line(QByteArray line);

	QOpenGLVertexArrayObject m_vao;
	QOpenGLShaderProgram *m_program;
	QOpenGLShaderProgram *m_program2;
	QOpenGLShaderProgram *m_program3;
	QMatrix4x4 m_proj;
	QMatrix4x4 m_camera;
	QMatrix4x4 m_world;
	QMatrix4x4 m_world_tmp;
	int m_mvMatrixLoc;
	int m_projMatrixLoc;
	int m_mvMatrixLoc2;
	int m_projMatrixLoc2;
	int m_mvMatrixLoc3;
	int m_projMatrixLoc3;
	QPoint m_lastPos;
	QPoint m_currPos;
	QVector3D rotateAxis;
	float rotateAngle;
	struct VertexData
	{
		QVector3D position;
		QVector3D normal;
	};

	VertexData vertices[3000];
	GLushort indices[10000];
	int verticesIndex;
	int indicesIndex;
	QOpenGLBuffer arrayBuf;
	QOpenGLBuffer indexBuf;
	bool vertexFlag;
	bool faceFlag;

private slots:
	void load_file();
	void showOrHideVertexNormal();
	void showOrHideFaceNormal();

signals:
	void reset();
};

#endif
