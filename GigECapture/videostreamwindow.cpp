#include "videostreamwindow.h"

VideoStreamWindow::VideoStreamWindow(QWidget *parent) : m_cam(nullptr), QWidget(parent)
{
    using namespace std;
    using namespace FlyCapture2;

	cout << "Constructing video stream window";
	createStreamWindow();
}

void VideoStreamWindow::handleRecordingStarted(QString dir) {
	std::cout << "Starting to display file: " << dir.toStdString() << endl;
	m_player.setMedia(QUrl::fromLocalFile(dir));
	m_player.play();
}

void VideoStreamWindow::setCamera(FlyCapture2::GigECamera * cam)
{
	m_cam = cam;
	initCam();
	startStream();
}

void VideoStreamWindow::createStreamWindow()
{
    m_player.setParent(this);
    m_videoWidget.setParent(this);
    m_player.setVideoOutput(&m_videoWidget);
}

void VideoStreamWindow::initCam()
{
    using namespace std;
    using namespace FlyCapture2;
    // Get the camera information
	cout << "Initalizing camera" << endl;
    Error error;
    CameraInfo camInfo;
    error = m_cam->GetCameraInfo(&camInfo);
    if (error != PGRERROR_OK)
    {
        PrintError( error );
        return;
    }

    PrintCameraInfo(&camInfo);

    unsigned int numStreamChannels = 0;
    error = m_cam->GetNumStreamChannels( &numStreamChannels );
    if (error != PGRERROR_OK)
    {
        PrintError( error );
        return;
    }

    for (unsigned int i=0; i < numStreamChannels; i++)
    {
        GigEStreamChannel streamChannel;
        error = m_cam->GetGigEStreamChannelInfo( i, &streamChannel );
        if (error != PGRERROR_OK)
        {
            PrintError( error );
            return;
        }

        streamChannel.destinationIpAddress.octets[0] = 224;
        streamChannel.destinationIpAddress.octets[1] = 0;
        streamChannel.destinationIpAddress.octets[2] = 0;
        streamChannel.destinationIpAddress.octets[3] = 1;

        error = m_cam->SetGigEStreamChannelInfo( i, &streamChannel );
        if (error != PGRERROR_OK)
        {
            PrintError( error );
            return;
        }

        cout << "Printing stream channel information for channel " << i << endl;
        PrintStreamChannelInfo( &streamChannel );
    }

    cout << "Querying GigE image setting information..." << endl;

    GigEImageSettingsInfo imageSettingsInfo;
    error = m_cam->GetGigEImageSettingsInfo( &imageSettingsInfo );
    if (error != PGRERROR_OK)
    {
        PrintError( error );
        return;
    }

    GigEImageSettings imageSettings;
    imageSettings.offsetX = 0;
    imageSettings.offsetY = 0;
    imageSettings.height = imageSettingsInfo.maxHeight;
    imageSettings.width = imageSettingsInfo.maxWidth;
    imageSettings.pixelFormat = PIXEL_FORMAT_MONO8;

    cout << "Setting GigE image settings..." << endl;

    error = m_cam->SetGigEImageSettings( &imageSettings );
    if (error != PGRERROR_OK)
    {
        PrintError( error );
        return;
    }
	cout << "Done initializing camera" << endl;
}


void VideoStreamWindow::startStream()
{

    using namespace std;
    using namespace FlyCapture2;
    Error error;
    cout << "Starting image capture..." << endl;

    // Start capturing images
    error = m_cam->StartCapture();
    if (error != PGRERROR_OK)
    {
        return;
    }
    StreamWorker* worker = new StreamWorker(m_cam);
	connect(worker, &StreamWorker::recordingStarted, this, &VideoStreamWindow::handleRecordingStarted);
    QThreadPool::globalInstance()->start(worker);
}

void VideoStreamWindow::stopStream()
{
    using namespace std;
    using namespace FlyCapture2;
    cout << "Stopping capture" << endl;
    Error error;
    // Stop capturing images
    error = m_cam->StopCapture();
    if (error != PGRERROR_OK)
    {
        PrintError( error );
        return;
    }

    // Disconnect the camera
    error = m_cam->Disconnect();
    if (error != PGRERROR_OK)
    {
        PrintError( error );
        return;
    }
}

StreamWorker::StreamWorker(FlyCapture2::GigECamera* cam): m_cam(cam)
{
    using namespace std;
    using namespace FlyCapture2;
    CameraInfo camInfo;
    Error error;
    error = m_cam->GetCameraInfo(&camInfo);
    if (error != PGRERROR_OK)
    {
        return;
    }
	cout << "Camera information given to StreamWorker" << endl;
    itoa(camInfo.serialNumber, m_currVideoSettings.filename, 10);
    strcpy(m_currVideoSettings.fileExtension, "avi");
}

void StreamWorker::run()
{
    using namespace std;
    using namespace FlyCapture2;

    Error error;
    Image image;


    AVIRecorder aviRecorder;
    AVIOption aviOption;
    error = aviRecorder.AVIOpen(m_currVideoSettings.filename, &aviOption);
	cout << "Trying to open AVI file" << endl;
    if(error != PGRERROR_OK)
    {
		PrintError(error);
        return;
    }
	cout << "Opened AVI file" << endl;
	//emit(recordingStarted(QString(m_currVideoSettings.filename) + QString(".avi")));
	m_currRecordingState = STARTED;

    while(GetRecorderState() == STARTED)
    {
		cout << "Recording loop" << endl;
		RetrieveNextImageFromBuffer(&image);

        try
        {
            SaveImageToVideo(&aviRecorder, &image);
        }
        catch (RecordingException& e)
        {
			cout << "Recording exception" << endl;
            break;
        }

    }

    // do cleanup
    error = aviRecorder.AVIClose();
    if(error != PGRERROR_OK)
    {
        return;
    }

    // We cannot restart camera in this thread so we reset the dialog
    // and put the recorder in a stopping state.
    return;
}



void StreamWorker::RetrieveNextImageFromBuffer(FlyCapture2::Image *image)
{
	using namespace std;
	using namespace FlyCapture2;
	Error error;
	QString csMessage;
	bool isCorruptFrame = false;

	PixelFormat pixFormat = UNSPECIFIED_PIXEL_FORMAT, pixFormatPrev = UNSPECIFIED_PIXEL_FORMAT;
	unsigned int cols = 0, rows = 0, colsPrev = 0, rowsPrev = 0;

	Image buffImage;
	error = m_cam->RetrieveBuffer(&buffImage);
	if (error != PGRERROR_OK)
	{
		if (error == PGRERROR_IMAGE_CONSISTENCY_ERROR)
		{
			return;
		}
		else
		{
			time_t rawtime;
			struct tm * timeinfo;
			time(&rawtime);
			timeinfo = localtime(&rawtime);

			char currTimeStr[128];
			sprintf(currTimeStr, "%s", asctime(timeinfo));
			currTimeStr[strlen(currTimeStr) - 1] = '\0';

			char errorMsg[1024];
			sprintf(
				errorMsg,
				"%s: Grab loop had an error: %s\n",
				currTimeStr,
				error.GetDescription());
		}
		return;
	}
	cout << "Grabbed frame" << endl;
	rows = buffImage.GetRows();
	cols = buffImage.GetCols();
	pixFormat = buffImage.GetPixelFormat();
	m_imageBuffer.push_back(buffImage);

    *image = m_imageBuffer.front();
    m_imageBuffer.erase(m_imageBuffer.begin());
}

void StreamWorker::SaveImageToVideo(FlyCapture2::AVIRecorder *aviRecorder, FlyCapture2::Image *image)
{
    using namespace std;
    using namespace FlyCapture2;
    Error error = aviRecorder->AVIAppend(image);
    if(error != PGRERROR_OK)
    {
        throw RecordingException(error.GetDescription());
    }

    if (GetRecorderState() == STOPPED)
    {
        return;
    }
}

StreamWorker::RecorderState StreamWorker::GetRecorderState()
{
    return m_currRecordingState;
}