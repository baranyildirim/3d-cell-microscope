#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H


#include "data.h"
#include "videostreamwindow.h"
#include "helper.h"

#include <FlyCapture2.h>

#include <QDialog>
#include <QtGlobal>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QThreadPool>
#include <QWidget>
#include <QFormLayout>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QCheckBox>

class ControlPanel: public QDialog
{
    Q_OBJECT
    public:
        explicit ControlPanel(QWidget *parent = nullptr);
	signals:
		void startStreamMain(FlyCapture2::GigECamera* cam);

    public slots:
        void onSettingsChanged();
		void onCaptureFramesClicked();
        void onStartStreamClicked();
		void onApplySettingsOneClicked();
        void onApplySettingsAllClicked();
		void onRefreshCamerasClicked();
		void onSelectedCameraChanged();
    private:
        QVBoxLayout m_mainLayout;
        QLabel m_cameraSelectorLabel;
        QComboBox m_cameraSelector;
		QPushButton m_refreshCamerasButton;
        QHBoxLayout m_cameraSelectorLayout;
        QGridLayout m_buttonLayout;
        QHBoxLayout m_panelLayout;
        QFormLayout m_leftFormLayout;
        QFormLayout m_rightFormLayout;
        QDoubleSpinBox m_exposureEdit;
		QDoubleSpinBox m_gainEdit;
		QDoubleSpinBox m_framerateEdit;
		QDoubleSpinBox m_shutterTimeEdit;
		QSpinBox m_imageXOffsetEdit;
		QSpinBox m_imageYOffsetEdit;
		QSpinBox m_packetSizeEdit;
		QSpinBox m_packetDelayEdit;
		QSpinBox m_numberOfFramesEdit;
		QSpinBox m_imageWidthEdit;
        QSpinBox m_imageHeightEdit;
		QCheckBox m_autoApplySettings;
		QPushButton m_applySettingsOneButton;
        QPushButton m_applySettingsAllButton;
        QPushButton m_startStreamButton;
		QPushButton m_captureFrameButton;
        QVector<int> m_serials;
		QMap<int, Settings> m_settings;
		QMap<int, FlyCapture2::PGRGuid*> m_guids;
		QMap<int, FlyCapture2::GigECamera*> m_cameras;

		void refreshSettings();
        void startStream();
        void applySettings(int serial);
        void getAllCameras();
		void updateDisplayedSettings();
		void captureFrame(int serial);
};

#endif // CONTROLPANEL_H
