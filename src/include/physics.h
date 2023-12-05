#pragma once

#include <vector>
#include <iostream>
#include <math.h>

struct Vec2D{
    double x;
    double y;
};

struct Point2D{
    Vec2D location;
};

class Disk { //2D disk
public:
    Vec2D origin;
    double radius;
    Vec2D velocity;
    bool isStatic;
    bool remove;
};

class LineSegment {
public:
    Vec2D pointA;
    Vec2D pointB;
    bool isStatic;
};

class PhysicsContainer{

};

Vec2D reflection(const Vec2D axis, const Vec2D incident);

Vec2D reflectionDampened(const Vec2D axis, const Vec2D incident, const double percentDampening);

double distance(const Vec2D pointA, const Vec2D pointB);

double distance(const Vec2D point, const LineSegment lineSegment);

double distanceSquared(const Vec2D pointA, const Vec2D pointB);

Vec2D closestPointToLine(const LineSegment lineSegment, const Vec2D point);

void collisionStaticLineSegmentDynamicDisk(const LineSegment& lineSegment, Disk& disk, double dampening);

void collisionStaticDiskDynamicDisk(const Disk& staticDisk, Disk& dynamicDisk, double dampening);

void collisionStaticDiskDynamicDisk(const Vec2D staticDiskOrigin, const double staticDiskRadius, Disk& dynamicDisk, double dampening);

void collisionDynamicDiskDynamicDisk(Disk& disk1, Disk& disk2);

bool isCollidingDiskDisk(const Disk& disk1, const Disk& disk2);