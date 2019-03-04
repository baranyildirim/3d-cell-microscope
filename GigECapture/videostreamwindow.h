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
#include <QMediaPlayer>
#include <QMediaService>
#include <QVideoWidget>
#include <QMediaPlaylist>
#include <QDialog>
#define MAX_PATH 260



class VideoStreamWindow : public QDialog
{
	Q_OBJECT
public:
    explicit VideoStreamWindow(QWidget *parent = nullptr);
	void setCamera(FlyCapture2::GigECamera* cam);
public slots:
	void handleRecordingStarted(QString dir);
private:
    void createStreamWindow();
    void initCam();
    void startStream();
    void stopStream();
    FlyCapture2::GigECamera* m_cam;

    QMediaPlayer m_player;
    QVideoWidget m_videoWidget;
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
    StreamWorker(FlyCapture2::GigECamera* cam);
    void run() override;

signals:
    void error(FlyCapture2::Error e);
	void recordingStarted(QString dir);

private:

    enum RecorderState
    {
        STOPPED,
        STARTED,
        SAVING,
        STOPPING,
        ABORT_SAVING
    };

    struct VideoSettings
    {
        char filename[MAX_PATH];
        char fileExtension[MAX_PATH];
    };

    void RetrieveNextImageFromBuffer(FlyCapture2::Image* image);
    void SaveImageToVideo(FlyCapture2::AVIRecorder* aviRecorder, FlyCapture2::Image* image);
    RecorderState GetRecorderState();

    QMutex m_recorderBuffer;
    RecorderState m_currRecordingState;
    VideoSettings m_currVideoSettings;
    FlyCapture2::GigECamera* m_cam;
    std::vector<FlyCapture2::Image> m_imageBuffer;
};

class GrabWorker : public QObject, public QRunnable {
	Q_OBJECT
	public:
		GrabWorker(FlyCapture2::GigECamera* cam, std::vector<FlyCapture2::Image>& imageBuffer, QMutex& recorderBuffer);
		void run() override;
	signals:
		void imageGrabbed(FlyCapture2::Image im);
	private:
		FlyCapture2::GigECamera* m_cam;
		std::vector<FlyCapture2::Image>& m_imageBuffer;
		FlyCapture2::Image m_rawImage;
		FlyCapture2::Image m_processedImage;
		QMutex& m_recorderBuffer;
};
#endif // VIDEOSTREAMWINDOW_H
