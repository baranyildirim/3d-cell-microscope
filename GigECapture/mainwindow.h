#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "controlpanel.h"
#include "videostreamwindow.h"
#include <FlyCapture2.h>


class MainWindow : public QMainWindow
{
    Q_OBJECT
    public:
        explicit MainWindow(QWidget *parent = nullptr);
	public slots:
		void handleStartCaptureMain(FlyCapture2::GigECamera* cam);
    private:
        ControlPanel m_controlPanel;
        VideoStreamWindow* m_videoWindow;

    public slots:
};

#endif // MAINWINDOW_H
