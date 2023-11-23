#include "physics.h"
#include <algorithm>

Vec2D reflection(const Vec2D axis, const Vec2D incident){ //axis is really the normal
    Vec2D reflected;

    //Transform incident to axis basis
    reflected.x =        axis.x * incident.x + axis.y * incident.y;
    reflected.y = -1.0 * axis.y * incident.x + axis.x * incident.y;

    //Flip parallel to axis, keep perpindicular to axis
    reflected.x *= -1.0;

    //Transform back from axis basis
    Vec2D temp = reflected;

    reflected.x = axis.x * temp.x + -1.0 * axis.y * temp.y; 
    reflected.y = axis.y * temp.x +        axis.x * temp.y; 

    //Scale
    reflected.x *= 1.0/(axis.x * axis.x + axis.y * axis.y);
    reflected.y *= 1.0/(axis.x * axis.x + axis.y * axis.y);

    return reflected;
}

Vec2D reflectionDampened(const Vec2D axis, const Vec2D incident, const double dampening){
    Vec2D reflected;

    //Transform incident to axis basis
    reflected.x =        axis.x * incident.x + axis.y * incident.y;
    reflected.y = -1.0 * axis.y * incident.x + axis.x * incident.y;

    //Flip parallel to axis and dampen, keep perpindicular to axis
    reflected.x *= -1.0 * dampening;

    //Transform back from axis basis
    Vec2D temp = reflected;

    reflected.x = axis.x * temp.x + -1.0 * axis.y * temp.y; 
    reflected.y = axis.y * temp.x +        axis.x * temp.y; 

    //Scale
    reflected.x *= 1.0/(axis.x * axis.x + axis.y * axis.y);
    reflected.y *= 1.0/(axis.x * axis.x + axis.y * axis.y);

    return reflected;
}

double distance(const Vec2D pointA, const Vec2D pointB){
    return sqrt((pointA.x - pointB.x) * (pointA.x - pointB.x) + (pointA.y - pointB.y) * (pointA.y - pointB.y));
}

double distance(const Vec2D point, const LineSegment lineSegment){
    /*Return the smallest between 
    1: the smallest distance between the point and the two lineSegment endpoints 
    2: and the distance calculated along the normal to the line from the point*/

    double pointToLSA = distance(point, lineSegment.pointA);
    double pointToLSB = distance(point, lineSegment.pointB);

    double pointToLine;
    /*Using Point-Point form of a line: y - y1 = (y2 - y1)/(x2 - x1) * (x - x1)*/
    //TODO: put in line segment, optimize
    double m = (lineSegment.pointA.y - lineSegment.pointB.y)/(lineSegment.pointA.x - lineSegment.pointB.x);

    double x = (point.y - lineSegment.pointA.y + m * lineSegment.pointA.x + (1.0/m) * point.x)/(m + 1.0/m);

    double y = lineSegment.pointA.y + m * (x - lineSegment.pointA.x);

    //Distance between this and point
    pointToLine = sqrt((point.x - x) * (point.x - x) + (point.y - y) * (point.y - y));

    if( (lineSegment.pointA.x <= x && x <= lineSegment.pointB.x) || (lineSegment.pointA.x >= x && x >= lineSegment.pointB.x) ){
        return pointToLine;
    }else{
        return 9999;//std::min(pointToLSA, pointToLSB); TODO PATCH
    }
}