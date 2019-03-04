#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QWidget>
#include <QFormLayout>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include "data.h"
#include <QLabel>
#include <QThreadPool>
#include "videostreamwindow.h"
#include "helper.h"
#include <FlyCapture2.h>

class ControlPanel: public QWidget
{
    Q_OBJECT
    public:
        explicit ControlPanel(QWidget *parent = nullptr);
	signals:
		void startCaptureMain(FlyCapture2::GigECamera* cam);

    public slots:
        void onSettingsChanged();
        void onCaptureClicked();
        void onApplySettingsClicked();
    private:
        QVBoxLayout m_mainLayout;
        QLabel m_cameraSelectorLabel;
        QComboBox m_cameraSelector;
        QHBoxLayout m_cameraSelectorLayout;
        QHBoxLayout m_buttonLayout;
        QHBoxLayout m_panelLayout;
        QFormLayout m_leftFormLayout;
        QFormLayout m_rightFormLayout;
        QLineEdit m_exposureEdit;
        QLineEdit m_gainEdit;
        QLineEdit m_framerateEdit;
        QLineEdit m_shutterTimeEdit;
        QLineEdit m_imageXOffsetEdit;
        QLineEdit m_imageYOffsetEdit;
        QLineEdit m_packetSizeEdit;
        QLineEdit m_packetDelayEdit;
        QLineEdit m_numberOfFramesEdit;
        QLineEdit m_imageWidthEdit;
        QLineEdit m_imageHeightEdit;
        QPushButton m_applySettingsButton;
        QPushButton m_captureButton;
        QVector<int> m_serials;
		QMap<int, Settings> m_settings;
		QMap<int, FlyCapture2::PGRGuid*> m_guids;
		QMap<int, FlyCapture2::GigECamera*> m_cameras;

        void startCapture();
        void applySettings();
        void getAllCameras();
};

#endif // CONTROLPANEL_H
