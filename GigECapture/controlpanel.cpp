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
    m_leftFormLayout.addRow(tr("&Framerate (fps):"), &m_framerateEdit);
    m_leftFormLayout.addRow(tr("&Shutter Time (ms):"), &m_shutterTimeEdit);
    m_leftFormLayout.addRow(tr("&Image-X Offset:"), &m_imageXOffsetEdit);
    m_leftFormLayout.addRow(tr("&Image-Y Offset:"), &m_imageYOffsetEdit);


    m_rightFormLayout.addRow(tr("&Packet Size:"),&m_packetSizeEdit);
    m_rightFormLayout.addRow(tr("&Packet Delay:"),&m_packetDelayEdit);
    m_rightFormLayout.addRow(tr("&Number of Frames:"),&m_numberOfFramesEdit);
    m_rightFormLayout.addRow(tr("&Image Width:"),&m_imageWidthEdit);
    m_rightFormLayout.addRow(tr("&Image Height:"),&m_imageHeightEdit);


    m_panelLayout.addLayout(&m_leftFormLayout);
    m_panelLayout.addLayout(&m_rightFormLayout);

	m_applySettingsOneButton.setText(tr("&Apply Settings for Current Camera"));
    m_applySettingsAllButton.setText(tr("&Apply Settings for All Cameras"));
	m_captureFrameButton.setText(tr("&Capture Frames"));
    m_startStreamButton.setText(tr("&Start Stream"));

    m_buttonLayout.addWidget(&m_applySettingsOneButton, 0, 0);
	m_buttonLayout.addWidget(&m_captureFrameButton, 0, 1);
	m_buttonLayout.addWidget(&m_applySettingsAllButton, 1, 0);
	m_buttonLayout.addWidget(&m_startStreamButton, 1, 1);

    m_cameraSelectorLabel.setText(tr("&Camera ID:"));
    m_cameraSelectorLabel.setBuddy(&m_cameraSelector);
    m_cameraSelector.setMaximumWidth(100);
    m_cameraSelectorLayout.addWidget(&m_cameraSelectorLabel);
    m_cameraSelectorLayout.addWidget(&m_cameraSelector);
	m_refreshCamerasButton.setText(tr("&Refresh Cameras"));
	m_cameraSelectorLayout.addWidget(&m_refreshCamerasButton);
    m_cameraSelectorLayout.setAlignment(Qt::AlignCenter);


    m_mainLayout.addLayout(&m_cameraSelectorLayout);
    m_mainLayout.addLayout(&m_panelLayout);
    m_mainLayout.addLayout(&m_buttonLayout);

	m_exposureEdit.setDecimals(3);
	m_exposureEdit.setRange(-7.5, 2.4);

	m_gainEdit.setDecimals(3);
	m_gainEdit.setRange(-5.75, 13.0);

	m_framerateEdit.setRange(0, 52.00);
	m_framerateEdit.setDecimals(3);

	m_shutterTimeEdit.setDecimals(3);
	m_shutterTimeEdit.setRange(0.020, 19.20);

	m_packetSizeEdit.setRange(0, 10000);
	m_packetDelayEdit.setRange(0, 10000);
	m_imageWidthEdit.setRange(0, 10000);
	m_imageHeightEdit.setRange(0, 10000);

	connect(&m_cameraSelector, &QComboBox::currentTextChanged, this, &ControlPanel::onSelectedCameraChanged);
	connect(&m_applySettingsOneButton, &QPushButton::released, this, &ControlPanel::onApplySettingsOneClicked);
	connect(&m_applySettingsAllButton, &QPushButton::released, this, &ControlPanel::onApplySettingsAllClicked);
	connect(&m_refreshCamerasButton, &QPushButton::released, this, &ControlPanel::onRefreshCamerasClicked);
	connect(&m_startStreamButton, &QPushButton::released, this, &ControlPanel::onStartStreamClicked);
    connect(&m_captureFrameButton, &QPushButton::released, this, &ControlPanel::onCaptureFramesClicked);
	connect(&m_exposureEdit, QOverload<const QString&>::of(&QDoubleSpinBox::valueChanged), this, &ControlPanel::onSettingsChanged);
    connect(&m_gainEdit, QOverload<const QString&>::of(&QDoubleSpinBox::valueChanged), this, &ControlPanel::onSettingsChanged);
    connect(&m_framerateEdit, QOverload<const QString&>::of(&QDoubleSpinBox::valueChanged), this, &ControlPanel::onSettingsChanged);
    connect(&m_shutterTimeEdit, QOverload<const QString&>::of(&QDoubleSpinBox::valueChanged), this, &ControlPanel::onSettingsChanged);
    connect(&m_imageXOffsetEdit, QOverload<const QString&>::of(&QSpinBox::valueChanged), this, &ControlPanel::onSettingsChanged);
    connect(&m_imageYOffsetEdit, QOverload<const QString&>::of(&QSpinBox::valueChanged), this, &ControlPanel::onSettingsChanged);
    connect(&m_packetSizeEdit, QOverload<const QString&>::of(&QSpinBox::valueChanged), this, &ControlPanel::onSettingsChanged);
    connect(&m_packetDelayEdit, QOverload<const QString&>::of(&QSpinBox::valueChanged), this, &ControlPanel::onSettingsChanged);
    connect(&m_numberOfFramesEdit, QOverload<const QString&>::of(&QSpinBox::valueChanged), this, &ControlPanel::onSettingsChanged);
    connect(&m_imageWidthEdit, QOverload<const QString&>::of(&QSpinBox::valueChanged), this, &ControlPanel::onSettingsChanged);
    connect(&m_imageHeightEdit, QOverload<const QString&>::of(&QSpinBox::valueChanged), this, &ControlPanel::onSettingsChanged);

    getAllCameras();
    for(int i = 0; i < m_serials.size(); i++){
        m_cameraSelector.addItem(QString::number(m_serials[i]));
    }

	if (m_serials.size() < 1) {
		m_startStreamButton.setEnabled(false);
		m_applySettingsAllButton.setEnabled(false);
		return;
	}

	Settings initialSettings = m_settings[m_cameraSelector.currentText().toInt()];
	m_exposureEdit.setValue(initialSettings.exposure);
	m_gainEdit.setValue(initialSettings.gain);
	m_framerateEdit.setValue(initialSettings.framerate);
	m_shutterTimeEdit.setValue(initialSettings.shutterTime);
	m_imageXOffsetEdit.setValue(initialSettings.imageXOffset);
	m_imageYOffsetEdit.setValue(initialSettings.imageYOffset);
	m_packetSizeEdit.setValue(initialSettings.packetSize);
	m_packetDelayEdit.setValue(initialSettings.packetDelay);
	m_numberOfFramesEdit.setValue(initialSettings.numberOfFrames);
	m_imageWidthEdit.setValue(initialSettings.imageWidth);
	m_imageHeightEdit.setValue(initialSettings.imageHeight);
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

void ControlPanel::onCaptureFramesClicked()
{
	using namespace std;
	using namespace FlyCapture2;

	for(int i = 0; i < m_serials.size(); i++)
		captureFrame(m_serials[i]);
}

void ControlPanel::onStartStreamClicked()
{
	startStream();
}

void ControlPanel::onApplySettingsOneClicked()
{
	applySettings(m_cameraSelector.currentText().toInt());
}

void ControlPanel::onApplySettingsAllClicked()
{
	Settings toBeApplied = m_settings[m_cameraSelector.currentText().toInt()];
	for (int i = 0; i < m_serials.size(); i++)
		m_settings[m_serials[i]] = toBeApplied;

	for(int i = 0; i < m_serials.size(); i++)
		applySettings(m_serials[i]);
	refreshSettings();
}

void ControlPanel::onRefreshCamerasClicked()
{
	getAllCameras();

	if (m_serials.size() < 1) {
		m_startStreamButton.setEnabled(false);
		m_applySettingsAllButton.setEnabled(false);
	}
	else {
		m_startStreamButton.setEnabled(true);
		m_applySettingsAllButton.setEnabled(true);
	}
}

void ControlPanel::onSelectedCameraChanged()
{
	refreshSettings();
	updateDisplayedSettings();
}

void ControlPanel::refreshSettings()
{
	using namespace std;
	using namespace FlyCapture2;
	for (int i = 0; i < m_serials.size(); i++) {
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

void ControlPanel::startStream()
{
    using namespace std;
    using namespace FlyCapture2;
	FlyCapture2::GigECamera* currentCamera = m_cameras[m_cameraSelector.currentText().toInt()];
	cout << "Starting capture for: " << m_cameraSelector.currentText().toInt();
	emit(startStreamMain(currentCamera));
}

void ControlPanel::applySettings(int serial)
{
	using namespace std;
	using namespace FlyCapture2;

	GigECamera* cam = m_cameras[serial];
	Settings s = m_settings[serial];
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
	exposure.absControl = true;
	exposure.autoManualMode = false;
	exposure.absValue = s.exposure;

	Property shutter;
	shutter.type = PropertyType::SHUTTER;
	cam->GetProperty(&shutter);
	shutter.absControl = true;
	shutter.autoManualMode = false;
	shutter.absValue = s.shutterTime;

	Property framerate;
	framerate.type = PropertyType::FRAME_RATE;
	cam->GetProperty(&framerate);

	framerate.absControl = true;
	framerate.autoManualMode = false;
	framerate.absValue = s.framerate;

	Property gain;
	gain.type = PropertyType::GAIN;
	cam->GetProperty(&gain);
	gain.absControl = true;
	gain.autoManualMode = false;
	gain.absValue = s.framerate;

	cam->SetProperty(&exposure);
	cam->SetProperty(&shutter);
	cam->SetProperty(&framerate);
	cam->SetProperty(&gain);
	cout << "Settings applied to camera: " << serial << endl;
}

void ControlPanel::getAllCameras()
{   
	 using namespace std;
     using namespace FlyCapture2;
     PrintBuildInfo();

     Error error;

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

void ControlPanel::updateDisplayedSettings()
{
	Settings settings = m_settings[m_cameraSelector.currentText().toInt()];
	m_exposureEdit.setValue(settings.exposure);
	m_gainEdit.setValue(settings.gain);
	m_framerateEdit.setValue(settings.framerate);
	m_shutterTimeEdit.setValue(settings.shutterTime);
	m_imageXOffsetEdit.setValue(settings.imageXOffset);
	m_imageYOffsetEdit.setValue(settings.imageYOffset);
	m_packetSizeEdit.setValue(settings.packetSize);
	m_packetDelayEdit.setValue(settings.packetDelay);
	m_numberOfFramesEdit.setValue(settings.numberOfFrames);
	m_imageWidthEdit.setValue(settings.imageWidth);
	m_imageHeightEdit.setValue(settings.imageHeight);
}

void ControlPanel::captureFrame(int serial)
{
	using namespace std;
	using namespace FlyCapture2;
	Image buffImage;
	Error error;
	char fileName[MAX_PATH];
	char fileExtension[5] = ".PNG";
	error = m_cameras[serial]->StartCapture();
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return;
	}

	error = m_cameras[serial]->RetrieveBuffer(&buffImage);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return;
	}
	itoa(serial, fileName, 10);
	strcat(fileName, fileExtension);
	buffImage.Save(fileName, FlyCapture2::ImageFileFormat::PNG);

	cout << "Capture frame for: " << serial << endl;

	error = m_cameras[serial]->StopCapture();
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return;
	}
}
