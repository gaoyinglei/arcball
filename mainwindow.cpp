#include "mainwindow.h"

MainWindow::MainWindow()
{
	menuBar = new QMenuBar();
	menuFile = menuBar->addMenu(tr("&File"));
	menuNormal = menuBar->addMenu(tr("&Normal"));

	loadFile = new QAction(tr("Load File"));
	loadFile->setShortcut(QKeySequence(tr("Ctrl+O")));
	menuFile->addAction(loadFile);

	vertexNormal = new QAction(tr("Show Vertex Normal"));
	vertexNormal->setShortcut(QKeySequence(tr("Ctrl+V")));
	menuNormal->addAction(vertexNormal);

	faceNormal = new QAction(tr("Show Face Normal"));
	faceNormal->setShortcut(QKeySequence(tr("Ctrl+F")));
	menuNormal->addAction(faceNormal);

	menuNormal->setEnabled(false);

	connect(vertexNormal, SIGNAL(triggered(bool)), this, SLOT(changeVertexNormalText()));
	connect(faceNormal, SIGNAL(triggered(bool)), this, SLOT(changeFaceNormalText()));

	setMenuBar(menuBar);
	glWidget = new GLWidget(this);

	connect(loadFile, SIGNAL(triggered(bool)), glWidget, SLOT(load_file()));
	connect(vertexNormal, SIGNAL(triggered(bool)), glWidget, SLOT(showOrHideVertexNormal()));
	connect(faceNormal, SIGNAL(triggered(bool)), glWidget, SLOT(showOrHideFaceNormal()));
	connect(glWidget, SIGNAL(reset()), this, SLOT(reset()));

	setCentralWidget(glWidget);

	vertexFlag = false;
	faceFlag = false;
}

void MainWindow::changeVertexNormalText()
{
	if (vertexFlag) {
		vertexNormal->setText(tr("Show Vertex Normal"));
	}
	else {
		vertexNormal->setText(tr("Hide Vertex Normal"));
	}
	vertexFlag = !vertexFlag;
}

void MainWindow::changeFaceNormalText()
{
	if (faceFlag) {
		faceNormal->setText(tr("Show Face Normal"));
	}
	else {
		faceNormal->setText(tr("Hide Face Normal"));
	}

	faceFlag = !faceFlag;
}

void MainWindow::reset()
{
	vertexFlag = false;
	faceFlag = false;
	vertexNormal->setText(tr("Show Vertex Normal"));
	faceNormal->setText(tr("Show Face Normal"));

	menuNormal->setEnabled(true);
}
