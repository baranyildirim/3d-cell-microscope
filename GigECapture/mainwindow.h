#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "controlpanel.h"
#include "videostreamwindow.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT
    public:
        explicit MainWindow(QWidget *parent = nullptr);
	public slots:
		void handleStartCaptureMain(int serial);
    private:
        ControlPanel m_controlPanel;
        VideoStreamWindow m_videoWindow;

    public slots:
};

#endif // MAINWINDOW_H
