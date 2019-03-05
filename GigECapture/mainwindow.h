#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "controlpanel.h"
#include "videostreamwindow.h"
#include <FlyCapture2.h>
#include <QHBoxLayout>


class MainWindow : public QObject
{
    Q_OBJECT
    public:
        explicit MainWindow();
	public slots:
		void handleStartCaptureMain(FlyCapture2::GigECamera* cam);
    private:
		QHBoxLayout m_layout;
        ControlPanel* m_controlPanel;
        VideoStreamWindow* m_videoWindow;

    public slots:
};

#endif // MAINWINDOW_H
