#ifndef DATA_H
#define DATA_H

struct Settings{
    int cameraID;
    double exposure;
    double gain;
    double framerate;
    double shutterTime;
    double imageXOffset;
    double imageYOffset;
    double packetSize;
    double packetDelay;
    int numberOfFrames;
    int imageWidth;
    int imageHeight;
};

#endif // DATA_H
