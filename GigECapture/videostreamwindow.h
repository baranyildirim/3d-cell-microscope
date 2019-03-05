#ifndef VIDEOSTREAMWINDOW_H
#define VIDEOSTREAMWINDOW_H

#include <QWidget>
#include "FlyCapture2.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <QMutex>
#include "helper.h"
#include <QThread>
#include <QThreadPool>
#include <QRunnable>
#include <QDialog>
#include <QMainWindow>
#include <QtAV>
#include <QSizePolicy>
#include <QtAVWidgets>
#include <QHBoxLayout>
#define MAX_PATH 260

enum RecorderState
{
	STOPPED,
	STARTED,
	SAVING,
	STOPPING,
	ABORT_SAVING
};


class VideoStreamWindow : public QDialog
{
	Q_OBJECT
public:
    explicit VideoStreamWindow(QWidget *parent = nullptr);
	~VideoStreamWindow();
	void setCamera(FlyCapture2::GigECamera* cam);
public slots:
	void handleRecordingStarted(QString dir);
private:
    void initCam();
    void startStream();
    void stopStream();
	void createStreamWindow();
    FlyCapture2::GigECamera* m_cam;

	QHBoxLayout m_layout;
	QtAV::GLWidgetRenderer2 m_renderer;
	QtAV::AVPlayer m_player;

	RecorderState m_currRecordingState;
};


// RecordingException
class RecordingException : public std::runtime_error
{
public:
    RecordingException(const std::string& error) : std::runtime_error(error){}
};

class StreamWorker : public QObject, public QRunnable {
	Q_OBJECT
public:
    StreamWorker(FlyCapture2::GigECamera* cam, RecorderState& recorderState);
    void run() override;

signals:
    void error(FlyCapture2::Error e);
	void recordingStarted(QString dir);

private:


    struct VideoSettings
    {
        char filename[MAX_PATH];
        char fileExtension[MAX_PATH];
    };

    void RetrieveNextImageFromBuffer(FlyCapture2::Image* image);
    void SaveImageToVideo(FlyCapture2::AVIRecorder* aviRecorder, FlyCapture2::Image* image);
    RecorderState GetRecorderState();

    QMutex m_recorderBuffer;
    RecorderState& m_currRecordingState;
    VideoSettings m_currVideoSettings;
    FlyCapture2::GigECamera* m_cam;
    std::vector<FlyCapture2::Image> m_imageBuffer;
};

#endif // VIDEOSTREAMWINDOW_H
