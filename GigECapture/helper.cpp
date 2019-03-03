#include "helper.h"

void PrintBuildInfo()
{
    using namespace std;
    using namespace FlyCapture2;

    FC2Version fc2Version;
    Utilities::GetLibraryVersion( &fc2Version );

    ostringstream version;
    version << "FlyCapture2 library version: " << fc2Version.major << "." << fc2Version.minor << "." << fc2Version.type << "." << fc2Version.build;
    cout << version.str() << endl;

    ostringstream timeStamp;
    timeStamp << "Application build date: " << __DATE__ << " " << __TIME__;
    cout << timeStamp.str() << endl << endl;
}

void PrintCameraInfo( FlyCapture2::CameraInfo* pCamInfo )
{
    using namespace std;
    using namespace FlyCapture2;
    ostringstream macAddress;
    macAddress << hex << setw(2) << setfill('0') << (unsigned int)pCamInfo->macAddress.octets[0] << ":" <<
        hex << setw(2) << setfill('0') << (unsigned int)pCamInfo->macAddress.octets[1] << ":" <<
        hex << setw(2) << setfill('0') << (unsigned int)pCamInfo->macAddress.octets[2] << ":" <<
        hex << setw(2) << setfill('0') << (unsigned int)pCamInfo->macAddress.octets[3] << ":" <<
        hex << setw(2) << setfill('0') << (unsigned int)pCamInfo->macAddress.octets[4] << ":" <<
        hex << setw(2) << setfill('0') << (unsigned int)pCamInfo->macAddress.octets[5];


    ostringstream ipAddress;
    ipAddress << (unsigned int)pCamInfo->ipAddress.octets[0] << "." <<
        (unsigned int)pCamInfo->ipAddress.octets[1] << "." <<
        (unsigned int)pCamInfo->ipAddress.octets[2] << "." <<
        (unsigned int)pCamInfo->ipAddress.octets[3];

    ostringstream subnetMask;
    subnetMask << (unsigned int)pCamInfo->subnetMask.octets[0] << "." <<
        (unsigned int)pCamInfo->subnetMask.octets[1] << "." <<
        (unsigned int)pCamInfo->subnetMask.octets[2] << "." <<
        (unsigned int)pCamInfo->subnetMask.octets[3];

    ostringstream defaultGateway;
    defaultGateway << (unsigned int)pCamInfo->defaultGateway.octets[0] << "." <<
        (unsigned int)pCamInfo->defaultGateway.octets[1] << "." <<
        (unsigned int)pCamInfo->defaultGateway.octets[2] << "." <<
        (unsigned int)pCamInfo->defaultGateway.octets[3];

    cout << endl;
    cout << "*** CAMERA INFORMATION ***" << endl;
    cout << "Serial number - " << pCamInfo->serialNumber << endl;
    cout << "Camera model - " << pCamInfo->modelName << endl;
    cout << "Camera vendor - " << pCamInfo->vendorName << endl;
    cout << "Sensor - " << pCamInfo->sensorInfo << endl;
    cout << "Resolution - " << pCamInfo->sensorResolution << endl;
    cout << "Firmware version - " << pCamInfo->firmwareVersion << endl;
    cout << "Firmware build time - " << pCamInfo->firmwareBuildTime << endl;
    cout << "GigE version - " << pCamInfo->gigEMajorVersion << "." << pCamInfo->gigEMinorVersion << endl;
    cout << "User defined name - " << pCamInfo->userDefinedName << endl;
    cout << "XML URL 1 - " << pCamInfo->xmlURL1 << endl;
    cout << "XML URL 2 - " << pCamInfo->xmlURL2 << endl;
    cout << "MAC address - " << macAddress.str() << endl;
    cout << "IP address - " << ipAddress.str() << endl;
    cout << "Subnet mask - " << subnetMask.str() << endl;
    cout << "Default gateway - " << defaultGateway.str() << endl << endl;

}

void PrintStreamChannelInfo( FlyCapture2::GigEStreamChannel* pStreamChannel )
{
    using namespace std;
    using namespace FlyCapture2;
    //char ipAddress[32];
    ostringstream ipAddress;
    ipAddress << (unsigned int)pStreamChannel->destinationIpAddress.octets[0] << "." <<
        (unsigned int)pStreamChannel->destinationIpAddress.octets[1] << "." <<
        (unsigned int)pStreamChannel->destinationIpAddress.octets[2] << "." <<
        (unsigned int)pStreamChannel->destinationIpAddress.octets[3];

    cout << "Network interface: " << pStreamChannel->networkInterfaceIndex << endl;
    cout << "Host Port: " << pStreamChannel->hostPort << endl;
    cout << "Do not fragment bit: " << ( pStreamChannel->doNotFragment ? "Enabled" : "Disabled") << endl;
    cout << "Packet size: " << pStreamChannel->packetSize << endl;
    cout << "Inter packet delay: " << pStreamChannel->interPacketDelay << endl;
    cout << "Destination IP address: " << ipAddress.str() << endl;
    cout << "Source port (on camera): " << pStreamChannel->sourcePort << endl << endl;

}

void PrintError( FlyCapture2::Error error )
{
    error.PrintErrorTrace();
	std::cout << "Error while connecting to camera: " << error.GetDescription() << std::endl;
}
