#ifndef HELPER_H
#define HELPER_H

#include "FlyCapture2.h"
#include <iostream>
#include <sstream>
#include <iomanip>

void PrintBuildInfo();
void PrintCameraInfo( FlyCapture2::CameraInfo* pCamInfo );
void PrintStreamChannelInfo( FlyCapture2::GigEStreamChannel* pStreamChannel );
void PrintError( FlyCapture2::Error error );

#endif // HELPER_H
