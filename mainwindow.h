#ifndef MYWINDOW_H
#define MYWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QPushButton>
#include "glwidget.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();

	GLWidget *glWidget;
	QMenuBar *menuBar;
	QMenu *menuFile;
	QMenu *menuNormal;
	QAction *loadFile;
	QAction *vertexNormal;
	QAction *faceNormal;
	bool vertexFlag;
	bool faceFlag;

private slots:
	void changeVertexNormalText();
	void changeFaceNormalText();
	void reset();
};

#endif