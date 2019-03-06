#include "mainwindow.h"

MainWindow::MainWindow()  
{
	m_controlPanel = new ControlPanel();
	m_controlPanel->setModal(false);
	m_controlPanel->show();
	connect(m_controlPanel, &ControlPanel::startStreamMain, this, &MainWindow::handleStartStreamMain);
}

void MainWindow::handleStartStreamMain(FlyCapture2::GigECamera* cam) {
	using namespace std;
	using namespace FlyCapture2;
	m_videoWindow = new VideoStreamWindow();
	m_videoWindow->setWindowModality(Qt::NonModal);
	m_videoWindow->setCamera(cam);
	m_videoWindow->setModal(false);
	m_videoWindow->show();
}
