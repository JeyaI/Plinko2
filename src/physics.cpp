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

double distanceSquared(const Vec2D pointA, const Vec2D pointB){
    return (pointA.x - pointB.x) * (pointA.x - pointB.x) + (pointA.y - pointB.y) * (pointA.y - pointB.y);
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

Vec2D closestPointOnLine(const LineSegment lineSegment, const Vec2D point){ //todo should really be line parameter
    if(lineSegment.pointA.y - lineSegment.pointB.y == 0){ //horizontal
        return {point.x, lineSegment.pointA.y};
    }
    
    if(lineSegment.pointA.x - lineSegment.pointB.x == 0){ //vertical
        return {lineSegment.pointA.x, point.y};
    }
    
    double m = (lineSegment.pointA.y - lineSegment.pointB.y)/(lineSegment.pointA.x - lineSegment.pointB.x);

    double x = (point.y - lineSegment.pointA.y + m * lineSegment.pointA.x + (1.0/m) * point.x)/(m + 1.0/m);

    double y = lineSegment.pointA.y + m * (x - lineSegment.pointA.x);

    Vec2D closestPoint{x, y};

    return closestPoint;
}

void collisionStaticLineSegmentDynamicDisk(const LineSegment& lineSegment, Disk& disk, double dampening = 1.0){

    Vec2D closestPoint = closestPointOnLine(lineSegment, disk.origin);

    if( distance(closestPoint, disk.origin) <= disk.radius ){
        if( (lineSegment.pointA.x <= closestPoint.x && closestPoint.x <= lineSegment.pointB.x) || (lineSegment.pointA.x >= closestPoint.x && closestPoint.x >= lineSegment.pointB.x) ){
            //This implies that the disk has collided in the middle of the line segment
            Vec2D normal{closestPoint.x - disk.origin.x, closestPoint.y - disk.origin.y};
            double magnitude = sqrt(normal.x * normal.x + normal.y * normal.y);
            normal.x /= magnitude;
            normal.y /= magnitude;

            double overlap = disk.radius - distance(closestPoint, disk.origin);

            disk.origin.x -= overlap * normal.x;
            disk.origin.y -= overlap * normal.y;
            
            disk.velocity = reflectionDampened(normal, disk.velocity, dampening);

            return;
        }
    }

    if( distance(disk.origin, lineSegment.pointA) <= disk.radius ){
        //Disk-disk
        collisionStaticDiskDynamicDisk(lineSegment.pointA, 0, disk, dampening);
        return;
    }

    if( distance(disk.origin, lineSegment.pointB) <= disk.radius ){
        //Disk-disk
        collisionStaticDiskDynamicDisk(lineSegment.pointB, 0, disk, dampening);
        return;
    }

}

void collisionStaticDiskDynamicDisk(const Disk& staticDisk, Disk& dynamicDisk, double dampening){
    Vec2D normal;
    normal.x = staticDisk.origin.x - dynamicDisk.origin.x;
    normal.y = staticDisk.origin.y - dynamicDisk.origin.y;

    dynamicDisk.velocity = reflectionDampened(normal, dynamicDisk.velocity, dampening);
    double overlap = staticDisk.radius + dynamicDisk.radius - distance(staticDisk.origin, dynamicDisk.origin); 

    dynamicDisk.origin.x -= overlap * (normal.x)/(distance(staticDisk.origin, dynamicDisk.origin));
    dynamicDisk.origin.y -= overlap * (normal.y)/(distance(staticDisk.origin, dynamicDisk.origin));
}

void collisionStaticDiskDynamicDisk(const Vec2D staticDiskOrigin, const double staticDiskRadius, Disk& dynamicDisk, double dampening){
    Vec2D normal;
    normal.x = staticDiskOrigin.x - dynamicDisk.origin.x;
    normal.y = staticDiskOrigin.y - dynamicDisk.origin.y;

    dynamicDisk.velocity = reflectionDampened(normal, dynamicDisk.velocity, dampening);
    double overlap = staticDiskRadius + dynamicDisk.radius - distance(staticDiskOrigin, dynamicDisk.origin); 

    dynamicDisk.origin.x -= overlap * (normal.x)/(distance(staticDiskOrigin, dynamicDisk.origin));
    dynamicDisk.origin.y -= overlap * (normal.y)/(distance(staticDiskOrigin, dynamicDisk.origin));
}

void collisionDynamicDiskDynamicDisk(Disk& disk1, Disk& disk2){
    double distanceSquared = (disk1.origin.x - disk2.origin.x) * (disk1.origin.x - disk2.origin.x) + (disk1.origin.y - disk2.origin.y) * (disk1.origin.y - disk2.origin.y); //TODO:turn into function

    Vec2D velocity1prime;
    Vec2D velocity2prime;
    //TODO make this more realistic
    velocity1prime.x = disk1.velocity.x - /*7.0/8.0 **/ ((disk1.velocity.x - disk2.velocity.x) * (disk1.origin.x - disk2.origin.x) + (disk1.velocity.y - disk2.velocity.y) * (disk1.origin.y - disk2.origin.y)) * (disk1.origin.x - disk2.origin.x) / distanceSquared;
    velocity1prime.y = disk1.velocity.y - /*7.0/8.0 **/ ((disk1.velocity.x - disk2.velocity.x) * (disk1.origin.x - disk2.origin.x) + (disk1.velocity.y - disk2.velocity.y) * (disk1.origin.y - disk2.origin.y)) * (disk1.origin.y - disk2.origin.y) / distanceSquared;

    velocity2prime.x = disk2.velocity.x - /*7.0/8.0 **/ ((disk2.velocity.x - disk1.velocity.x) * (disk2.origin.x - disk1.origin.x) + (disk2.velocity.y - disk1.velocity.y) * (disk2.origin.y - disk1.origin.y)) * (disk2.origin.x - disk1.origin.x) / distanceSquared;
    velocity2prime.y = disk2.velocity.y - /*7.0/8.0 **/ ((disk2.velocity.x - disk1.velocity.x) * (disk2.origin.x - disk1.origin.x) + (disk2.velocity.y - disk1.velocity.y) * (disk2.origin.y - disk1.origin.y)) * (disk2.origin.y - disk1.origin.y) / distanceSquared;
    //(disk2.velocity.x - disk1.velocity.x) * (disk2.origin.x - disk1.origin.x) + (disk2.velocity.y - disk1.velocity.y) * (disk2.origin.y - disk1.origin.y)
    disk1.velocity = velocity1prime;
    disk2.velocity = velocity2prime;
}

bool isCollidingDiskDisk(const Disk& disk1, const Disk& disk2){
    double dd = distanceSquared(disk1.origin, disk2.origin);
    return (dd <= (disk1.radius + disk2.radius) * (disk1.radius + disk2.radius) );
}