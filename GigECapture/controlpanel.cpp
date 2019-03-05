#include "controlpanel.h"

ControlPanel::ControlPanel(QWidget *parent): QDialog(parent)
{
    this->setLayout(&m_mainLayout);
	setWindowModality(Qt::NonModal);
    m_exposureEdit.setMaximumWidth(60);
    m_gainEdit.setMaximumWidth(60);
    m_framerateEdit.setMaximumWidth(60);
    m_shutterTimeEdit.setMaximumWidth(60);
    m_imageXOffsetEdit.setMaximumWidth(60);
    m_imageYOffsetEdit.setMaximumWidth(60);
    m_packetSizeEdit.setMaximumWidth(60);
    m_packetDelayEdit.setMaximumWidth(60);
    m_numberOfFramesEdit.setMaximumWidth(60);
    m_imageWidthEdit.setMaximumWidth(60);
    m_imageHeightEdit.setMaximumWidth(60);

    m_leftFormLayout.addRow(tr("&Exposure (EV):"), &m_exposureEdit);
    m_leftFormLayout.addRow(tr("&Gain (dB):"), &m_gainEdit);
    m_leftFormLayout.addRow(tr("&Framerate:"), &m_framerateEdit);
    m_leftFormLayout.addRow(tr("&Shutter Time:"), &m_shutterTimeEdit);
    m_leftFormLayout.addRow(tr("&Image-X Offset:"), &m_imageXOffsetEdit);
    m_leftFormLayout.addRow(tr("&Image-Y Offset:"), &m_imageYOffsetEdit);


    m_rightFormLayout.addRow(tr("&Packet Size:"),&m_packetSizeEdit);
    m_rightFormLayout.addRow(tr("&Packet Delay:"),&m_packetDelayEdit);
    m_rightFormLayout.addRow(tr("&Number of Frames:"),&m_numberOfFramesEdit);
    m_rightFormLayout.addRow(tr("&Image Width:"),&m_imageWidthEdit);
    m_rightFormLayout.addRow(tr("&Image Height:"),&m_imageHeightEdit);


    m_panelLayout.addLayout(&m_leftFormLayout);
    m_panelLayout.addLayout(&m_rightFormLayout);

    m_applySettingsButton.setText(tr("&Apply Settings"));
    m_captureButton.setText(tr("&Start Capture"));

    m_buttonLayout.addWidget(&m_applySettingsButton);
    m_buttonLayout.addWidget(&m_captureButton);


    m_cameraSelectorLabel.setText(tr("&Camera ID:"));
    m_cameraSelectorLabel.setBuddy(&m_cameraSelector);
    m_cameraSelector.setMaximumWidth(100);
    m_cameraSelectorLayout.addWidget(&m_cameraSelectorLabel);
    m_cameraSelectorLayout.addWidget(&m_cameraSelector);
    m_cameraSelectorLayout.setAlignment(Qt::AlignCenter);


    m_mainLayout.addLayout(&m_cameraSelectorLayout);
    m_mainLayout.addLayout(&m_panelLayout);
    m_mainLayout.addLayout(&m_buttonLayout);

	connect(&m_captureButton, &QPushButton::released, this, &ControlPanel::onCaptureClicked);
    connect(&m_exposureEdit, &QLineEdit::textEdited, this, &ControlPanel::onSettingsChanged);
    connect(&m_gainEdit, &QLineEdit::textEdited, this, &ControlPanel::onSettingsChanged);
    connect(&m_framerateEdit, &QLineEdit::textEdited, this, &ControlPanel::onSettingsChanged);
    connect(&m_shutterTimeEdit, &QLineEdit::textEdited, this, &ControlPanel::onSettingsChanged);
    connect(&m_imageXOffsetEdit, &QLineEdit::textEdited, this, &ControlPanel::onSettingsChanged);
    connect(&m_imageYOffsetEdit, &QLineEdit::textEdited, this, &ControlPanel::onSettingsChanged);
    connect(&m_packetSizeEdit, &QLineEdit::textEdited, this, &ControlPanel::onSettingsChanged);
    connect(&m_packetDelayEdit, &QLineEdit::textEdited, this, &ControlPanel::onSettingsChanged);
    connect(&m_numberOfFramesEdit, &QLineEdit::textEdited, this, &ControlPanel::onSettingsChanged);
    connect(&m_imageWidthEdit, &QLineEdit::textEdited, this, &ControlPanel::onSettingsChanged);
    connect(&m_imageHeightEdit, &QLineEdit::textEdited, this, &ControlPanel::onSettingsChanged);

    getAllCameras();
    for(int i = 0; i < m_serials.size(); i++){
        m_cameraSelector.addItem(QString::number(m_serials[i]));
    }

	if (m_serials.size() < 1)
		return;

	Settings initialSettings = m_settings[m_cameraSelector.currentText().toInt()];
	m_exposureEdit.setText(QString::number(initialSettings.exposure));
	m_gainEdit.setText(QString::number(initialSettings.gain));
	m_framerateEdit.setText(QString::number(initialSettings.framerate));
	m_shutterTimeEdit.setText(QString::number(initialSettings.shutterTime));
	m_imageXOffsetEdit.setText(QString::number(initialSettings.imageXOffset));
	m_imageYOffsetEdit.setText(QString::number(initialSettings.imageYOffset));
	m_packetSizeEdit.setText(QString::number(initialSettings.packetSize));
	m_packetDelayEdit.setText(QString::number(initialSettings.packetDelay));
	m_numberOfFramesEdit.setText(QString::number(initialSettings.numberOfFrames));
	m_imageWidthEdit.setText(QString::number(initialSettings.imageWidth));
	m_imageHeightEdit.setText(QString::number(initialSettings.imageHeight));
}

void ControlPanel::onSettingsChanged()
{
	int serial = m_cameraSelector.currentText().toInt();
	Settings currSettings = m_settings[serial];
	currSettings.exposure = m_exposureEdit.text().toDouble();
	currSettings.gain = m_gainEdit.text().toDouble();
	currSettings.framerate = m_framerateEdit.text().toDouble();
	currSettings.shutterTime = m_shutterTimeEdit.text().toDouble();
	currSettings.imageXOffset = m_imageXOffsetEdit.text().toDouble();
	currSettings.imageYOffset = m_imageYOffsetEdit.text().toDouble();
	currSettings.packetSize = m_packetSizeEdit.text().toDouble();
	currSettings.packetDelay = m_packetDelayEdit.text().toDouble();
	currSettings.numberOfFrames = m_numberOfFramesEdit.text().toInt();
	currSettings.imageWidth = m_imageWidthEdit.text().toInt();
	currSettings.imageHeight = m_imageHeightEdit.text().toInt();
	m_settings[serial] = currSettings;
}

void ControlPanel::onCaptureClicked()
{
	startCapture();
}

void ControlPanel::onApplySettingsClicked()
{
	applySettings();
}

void ControlPanel::startCapture()
{
    using namespace std;
    using namespace FlyCapture2;
	FlyCapture2::GigECamera* currentCamera = m_cameras[m_cameraSelector.currentText().toInt()];
	cout << "Starting capture for: " << m_cameraSelector.currentText().toInt();
	emit(startCaptureMain(currentCamera));
}

void ControlPanel::applySettings()
{

}

void ControlPanel::getAllCameras()
{    using namespace std;
     using namespace FlyCapture2;
     PrintBuildInfo();

     Error error;

     // Since this application saves images in the current folder
     // we must ensure that we have permission to write to this folder.
     // If we do not have permission, fail right away.
     FILE* tempFile = fopen("tmp", "w+");
     if (tempFile == NULL)
     {
         cout << "Failed to create file in current folder.  Please check permissions." << endl;
         return;
     }
     fclose(tempFile);
     remove("tmp");

     BusManager busMgr;

     CameraInfo camInfo[20];
     unsigned int numCamInfo = 20;
     error = BusManager::DiscoverGigECameras( camInfo, &numCamInfo );
     if (error != PGRERROR_OK)
     {
         PrintError( error );
         return;
     }


     cout << "Number of cameras discovered: " << numCamInfo << endl;
	

     unsigned int numCameras;
     error = busMgr.GetNumOfCameras(&numCameras);
     if (error != PGRERROR_OK)
     {
         PrintError( error );
         return;
     }

     cout << "Number of cameras enumerated: " << numCameras << endl;
	
      for(int i = 0; i < numCamInfo; i++){
          m_serials.push_back(camInfo[i].serialNumber);
      }

	  for (int i = 0; i < numCamInfo; i++) {
		  Error error;
		  GigECamera* cam = new GigECamera();
		  PGRGuid* guid = new PGRGuid();
		  
		  busMgr.GetCameraFromSerialNumber(m_serials[i], guid);
		  error = cam->Connect(guid);
		  cout << "Connecting to: " << m_serials[i] << " || " << guid->value << endl;
		  if (error != PGRERROR_OK)
		  {
			  PrintError(error);
			  return;
		  }
		  cout << "Connected to:" << guid->value << endl;
		  m_guids[m_serials[i]] = guid;
		  m_cameras[m_serials[i]] = cam;
	}

	 for (int i = 0; i < numCamInfo; i++) {
		GigECamera* cam = m_cameras[m_serials[i]];
		GigEConfig info;
		GigEImageSettings imageSettings;
		FC2Config config;
		EmbeddedImageInfo embededInfo;
		GigEStreamChannel channel;

		cam->GetGigEConfig(&info);
		cam->GetGigEImageSettings(&imageSettings);
		cam->GetConfiguration(&config);
		cam->GetEmbeddedImageInfo(&embededInfo);
		cam->GetGigEStreamChannelInfo(0, &channel);

		Property exposure;
		exposure.type = PropertyType::AUTO_EXPOSURE;
		cam->GetProperty(&exposure);

		Property shutter;
		shutter.type = PropertyType::SHUTTER;
		cam->GetProperty(&shutter);

		Property framerate;
		framerate.type = PropertyType::FRAME_RATE;
		cam->GetProperty(&framerate);

		Property gain;
		gain.type = PropertyType::GAIN;
		cam->GetProperty(&gain);

		Settings settings;
		settings.cameraID = m_serials[i];
		settings.exposure = exposure.absValue;
		settings.framerate = framerate.absValue;
		settings.gain = gain.absValue;
		settings.imageHeight = imageSettings.height;
		settings.imageWidth = imageSettings.width;
		settings.imageXOffset = imageSettings.offsetX;
		settings.imageYOffset = imageSettings.offsetY;
		settings.packetDelay = channel.interPacketDelay;
		settings.packetSize = channel.packetSize;
		settings.shutterTime = shutter.absValue;

		m_settings[m_serials[i]] = settings;
	 }

}
