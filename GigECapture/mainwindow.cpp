#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	m_videoWindow.setParent(this);
    m_controlPanel.setParent(this);
	
    this->setCentralWidget(&m_controlPanel);
	connect(&m_controlPanel, &ControlPanel::startCaptureMain, this, &MainWindow::handleStartCaptureMain);
}

void MainWindow::handleStartCaptureMain(FlyCapture2::GigECamera* cam) {
	using namespace std;
	using namespace FlyCapture2;
	m_videoWindow.setCamera(cam);
}
