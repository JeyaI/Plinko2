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

double distance(Vec2D point, LineSegment lineSegment);