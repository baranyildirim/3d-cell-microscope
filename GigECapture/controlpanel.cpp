#include "controlpanel.h"

ControlPanel::ControlPanel(QWidget *parent)
{
    this->setLayout(&m_mainLayout);

    m_exposureEdit.setMaximumWidth(50);
    m_gainEdit.setMaximumWidth(50);
    m_framerateEdit.setMaximumWidth(50);
    m_shutterTimeEdit.setMaximumWidth(50);
    m_imageXOffsetEdit.setMaximumWidth(50);
    m_imageYOffsetEdit.setMaximumWidth(50);
    m_packetSizeEdit.setMaximumWidth(50);
    m_packetDelayEdit.setMaximumWidth(50);
    m_numberOfFramesEdit.setMaximumWidth(50);
    m_imageWidthEdit.setMaximumWidth(50);
    m_imageHeightEdit.setMaximumWidth(50);

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

	connect(&m_captureButton, &QPushButton::released, this, &ControlPanel::startCapture);
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
}

void ControlPanel::onSettingsChanged()
{
    m_settings.exposure = m_exposureEdit.text().toDouble();
    m_settings.gain = m_gainEdit.text().toDouble();
    m_settings.framerate = m_framerateEdit.text().toDouble();
    m_settings.shutterTime = m_shutterTimeEdit.text().toDouble();
    m_settings.imageXOffset = m_imageXOffsetEdit.text().toDouble();
    m_settings.imageYOffset = m_imageYOffsetEdit.text().toDouble();
    m_settings.packetSize = m_packetSizeEdit.text().toDouble();
    m_settings.packetDelay = m_packetDelayEdit.text().toDouble();
    m_settings.numberOfFrames = m_numberOfFramesEdit.text().toInt();
    m_settings.imageWidth = m_imageWidthEdit.text().toInt();
    m_settings.imageHeight = m_imageHeightEdit.text().toInt();
    applySettings();
}

void ControlPanel::onCaptureClicked()
{

}

void ControlPanel::onApplySettingsClicked()
{

}

void ControlPanel::startCapture()
{
    using namespace std;
    using namespace FlyCapture2;
    int currentCamera = m_cameraSelector.currentData().toInt();
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

}
