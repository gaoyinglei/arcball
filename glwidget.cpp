#include "glwidget.h"
#include <QMouseEvent>
#include <QFileDialog>
#include <iostream>

static const char *myVertexShaderSource =
"in vec4 vPosition;\n"
"in vec3 normal;\n"
"out vec4 veryingColor;\n"
"uniform mat4 projMatrix;\n"
"uniform mat4 mvMatrix;\n"
"uniform mat3 normalMatrix;\n"
"uniform vec3 lightPos;\n"
"void main(){\n"
"    gl_Position = projMatrix * mvMatrix * vPosition;\n"
"	 vec3 N = normalize(normalMatrix * normal);\n"
"	 vec3 L = normalize(lightPos - vPosition.xyz);\n"
"	 vec4 La = vec4(0.1, 0.1, 0.1, 0.0);\n"
"	 vec4 Ld = vec4(0.5, 0.4, 0.8, 1.0);\n"
"	 vec4 Ls = vec4(1.0, 1.0, 1.0, 0.0);\n"
"	 vec3 r = normalize(reflect(-L, N));\n"
"	 veryingColor = La + Ld * dot(N, L)  + Ls * pow(dot(r, N), 128.0);\n"
"}\n";
static const char *myFragmentShaderSource =
"in vec4 veryingColor;\n"
"out vec4 fColor;\n"
"void main(){\n"
"    fColor = veryingColor;\n"
"}\n";

static const char *myVertexShaderSource2 =
"in vec4 vPosition;\n"
"uniform mat4 projMatrix;\n"
"uniform mat4 mvMatrix;\n"
"void main(){\n"
"	gl_Position = projMatrix * mvMatrix * vPosition;\n"
"}\n";

static const char *myFragmentShaderSource2 =
"out vec3 fColor;\n"
"void main(){\n"
"	fColor = vec3(255.0, 0.0, 0.0);\n"
"}\n";

static const char *myVertexShaderSource3 =
"in vec4 vPosition;\n"
"uniform mat4 projMatrix;\n"
"uniform mat4 mvMatrix;\n"
"void main(){\n"
"	gl_Position = projMatrix * mvMatrix * vPosition;\n"
"}\n";

static const char *myFragmentShaderSource3 =
"out vec3 fColor;\n"
"void main(){\n"
"	fColor = vec3(0.0, 255.0, 0.0);\n"
"}\n";

GLWidget::GLWidget(QWidget *parent)
	: QOpenGLWidget(parent), indexBuf(QOpenGLBuffer::IndexBuffer)
{
}
GLWidget::~GLWidget()
{
	arrayBuf.destroy();
	indexBuf.destroy();
}

QSize GLWidget::minimumSizeHint() const
{
	return QSize(500, 500);
}


void GLWidget::initializeGL()
{
	initializeOpenGLFunctions();
	int m_transparent = 0;
	glClearColor(0.8, 0.8, 0.8, 1);

	m_program = new QOpenGLShaderProgram;

	m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, myVertexShaderSource);
	m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, myFragmentShaderSource);
	m_program->bindAttributeLocation("vertex", 0);
	m_program->bindAttributeLocation("normal", 1);

	m_program->link();
	m_program->bind();

	m_projMatrixLoc = m_program->uniformLocation("projMatrix");
	m_mvMatrixLoc = m_program->uniformLocation("mvMatrix");

	m_program->setUniformValue(m_program->uniformLocation("lightPos"), QVector3D(30, 30, 70));

	m_vao.create();
	QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

	arrayBuf.create();
	indexBuf.create();

	// Transfer vertex data to VBO 0
	arrayBuf.bind();
	arrayBuf.allocate(vertices, 10000 * sizeof(VertexData));

	// Transfer index data to VBO 1
	indexBuf.bind();
	indexBuf.allocate(indices, 30000 * sizeof(GLushort));

	m_program->enableAttributeArray(0);
	m_program->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(VertexData));

	m_program->enableAttributeArray(1);
	m_program->setAttributeBuffer(1, GL_FLOAT, sizeof(QVector3D), 3, sizeof(VertexData));

	// Our camera never changes in this example.
	m_camera.setToIdentity();
	m_camera.translate(0, 0, -5);

	m_program->release();

	m_program2 = new QOpenGLShaderProgram;

	m_program2->addShaderFromSourceCode(QOpenGLShader::Vertex, myVertexShaderSource2);
	m_program2->addShaderFromSourceCode(QOpenGLShader::Fragment, myFragmentShaderSource2);

	m_program2->link();
	m_program2->bind();

	m_projMatrixLoc2 = m_program2->uniformLocation("projMatrix");
	m_mvMatrixLoc2 = m_program2->uniformLocation("mvMatrix");

	m_program2->release();

	m_program3 = new QOpenGLShaderProgram;

	m_program3->addShaderFromSourceCode(QOpenGLShader::Vertex, myVertexShaderSource3);
	m_program3->addShaderFromSourceCode(QOpenGLShader::Fragment, myFragmentShaderSource3);

	m_program3->link();
	m_program3->bind();

	m_projMatrixLoc3 = m_program3->uniformLocation("projMatrix");
	m_mvMatrixLoc3 = m_program3->uniformLocation("mvMatrix");

	m_program3->release();

	m_world.setToIdentity();
	rotateAngle = 0;
	rotateAxis = QVector3D(1, 0, 0);
}

void GLWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	QMatrix4x4 tmp;
	tmp.rotate(rotateAngle*180.0 / 3.14, rotateAxis);
	m_world = tmp * m_world;
	
	QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);
	m_program->bind();
	
	m_program->setUniformValue(m_projMatrixLoc, m_proj);
	m_program->setUniformValue(m_mvMatrixLoc, m_camera * m_world);

	QMatrix3x3 normalMatrix = m_world.normalMatrix();
	m_program->setUniformValue(m_program->uniformLocation("normalMatrix"), normalMatrix);

	glDrawElements(GL_TRIANGLES, indicesIndex, GL_UNSIGNED_SHORT, 0);

	m_program->release();

	m_program2->bind();

	m_program2->setUniformValue(m_projMatrixLoc2, m_proj);
	m_program2->setUniformValue(m_mvMatrixLoc2, m_camera * m_world);
	

	if (vertexFlag) {
		// 画点的法线
		glBegin(GL_LINES);
		for (int i = 0; i < verticesIndex; i++) {
			glVertex3f(vertices[i].position.x(), vertices[i].position.y(), vertices[i].position.z());
			QVector3D vertex = vertices[i].position + vertices[i].normal / 4;
			glVertex3f(vertex.x(), vertex.y(), vertex.z());
		}
		glEnd();
	}

	m_program2->release();

	m_program3->bind();

	m_program3->setUniformValue(m_projMatrixLoc3, m_proj);
	m_program3->setUniformValue(m_mvMatrixLoc3, m_camera * m_world);
	
	if (faceFlag) {
		// 画面的法线
		glBegin(GL_LINES);
		for (int i = 0; i < indicesIndex; i += 3) {
			QVector3D vertex1 = (vertices[indices[i]].position + vertices[indices[i + 1]].position + vertices[indices[i + 2]].position) / 3;
			glVertex3f(vertex1.x(), vertex1.y(), vertex1.z());

			QVector3D vertex2 = QVector3D::normal(vertices[indices[i + 1]].position - vertices[indices[i]].position,
				vertices[indices[i + 2]].position - vertices[indices[i]].position) / 4 + vertex1;
			glVertex3f(vertex2.x(), vertex2.y(), vertex2.z());
		}
		glEnd();
	}

	m_program3->release();
}

void GLWidget::resizeGL(int w, int h)
{
	m_proj.setToIdentity();
	m_proj.perspective(45.0f, GLfloat(w) / h, 0.01f, 100.0f);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
	m_lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
	m_currPos = event->pos();
	if (event->buttons() & Qt::LeftButton) {
		arcBall(m_lastPos, m_currPos);
		update();
	}
	m_lastPos = m_currPos;
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
	m_camera.translate(0, 0, event->delta() / 120);
	update();
}

void GLWidget::arcBall(QPoint last_pnt, QPoint curr_pnt)
{
	QVector3D last_p(2.0 * last_pnt.x() / this->width() - 1, -(2.0 * last_pnt.y() / this->height() - 1), 0.0),
		curr_p(2.0 * curr_pnt.x() / this->width() - 1, -(2.0 * curr_pnt.y() / this->height() - 1), 0.0);

	if (last_p.x() * last_p.x() + last_p.y() * last_p.y() <= 1) {
		last_p.setZ(sqrt(1 - last_p.x() * last_p.x() - last_p.y() * last_p.y()));
	}

	if (curr_p.x() * curr_p.x() + curr_p.y() * curr_p.y() <= 1) {
		curr_p.setZ(sqrt(1 - curr_p.x() * curr_p.x() - curr_p.y() * curr_p.y()));
	}

	rotateAngle = acos(QVector3D::dotProduct(last_p, curr_p));
	rotateAxis = QVector3D::normal(last_p, curr_p);
}

void GLWidget::process_line(QByteArray line)
{
	QString str = QString(line);
	QStringList list = str.split(' ');
	if (list.at(0) == "v") {
		vertices[verticesIndex++].position = QVector3D(list.at(1).toFloat(), list.at(2).toFloat(), list.at(3).toFloat());
	}
	else if (list.at(0) == "vn") {
		vertices[verticesIndex].normal = QVector3D(list.at(1).toFloat(), list.at(2).toFloat(), list.at(3).toFloat());
	}
	else if (list.at(0) == "f") {
		for (int i = 1; i < list.size(); ++i) {
			QStringList tmpList = list.at(i).split("//");
			indices[indicesIndex++] = tmpList.at(0).toInt() - 1;
		}
	}
}

void GLWidget::load_file()
{
	QString filePath = QFileDialog::getOpenFileName(this, tr("open file"), " ", tr("obj file(*.obj)"));

	if (filePath != NULL) {
		verticesIndex = 0;
		indicesIndex = 0;
		vertexFlag = false;
		faceFlag = false;

		QFile file(filePath);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			return;
		}

		while (!file.atEnd()) {
			QByteArray line = file.readLine();
			process_line(line);
		}
		//update();
		initializeGL();

		emit reset();
	}
}

void GLWidget::showOrHideVertexNormal()
{
	vertexFlag = !vertexFlag;
	update();
}

void GLWidget::showOrHideFaceNormal()
{
	faceFlag = !faceFlag;
	update();
}