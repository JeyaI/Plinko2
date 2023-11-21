#pragma once

#include "physics.h"

class Model{
private:
    std::vector<Disk> disks;
    std::vector<LineSegment> lineSegments;
public:
    Model();
    void step();
    std::vector<Disk>& viewDisks();
    std::vector<LineSegment>& viewLineSegments();
    bool go;
};