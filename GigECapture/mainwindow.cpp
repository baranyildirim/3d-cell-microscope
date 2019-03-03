#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	m_videoWindow.setParent(this);
    m_controlPanel.setParent(this);
	
    this->setCentralWidget(&m_controlPanel);
	connect(&m_controlPanel, &ControlPanel::startCaptureMain, this, &MainWindow::handleStartCaptureMain);
}

void MainWindow::handleStartCaptureMain(int serial) {
	using namespace std;
	using namespace FlyCapture2;
	BusManager busMgr;
	PGRGuid guid;
	busMgr.GetCameraFromSerialNumber(serial, &guid);

	Error error;
	GigECamera* cam = new GigECamera();
	cout << "Connecting to camera..." << endl;
	cout << "Connecting to: " << guid.value << endl;
	// Connect to a camera
	error = cam->Connect(NULL);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return;
	}
	
	cout << "Connected to camera..." << endl;
	m_videoWindow.setCamera(cam);
}
