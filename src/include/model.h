#pragma once

#include "physics.h"

class Model{
private:
    std::vector<Disk> disks;
    std::vector<LineSegment> lineSegments;
    double deltaTime;
    Disk diskPreview;
    
public:
    Model();
    void step();
    std::vector<Disk>& viewDisks();
    std::vector<LineSegment>& viewLineSegments();
    bool go;
    void setDeltaTime(double deltaTime);
    Disk& viewDiskPreview();
};