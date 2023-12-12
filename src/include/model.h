#pragma once

#include "physics.h"

class Model{
private:
    std::vector<Disk> disks;
    std::vector<LineSegment> lineSegments;
    double deltaTime;
    Disk diskPreview;
    int score;
    int disksToPlace;
    
public:
    Model();
    void step();
    std::vector<Disk>& viewDisks();
    std::vector<LineSegment>& viewLineSegments();
    bool go;
    void setDeltaTime(double deltaTime);
    Disk& viewDiskPreview();
    const int viewScore();
    void reset();
    int viewDisksToPlace();
    void dropDisk(Disk d);
};