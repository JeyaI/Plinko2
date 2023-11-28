#include "model.h"

Model::Model(){
    go = false;

    lineSegments.push_back({{100.0, 590.0},{1640.0, 589.0}});

    lineSegments.push_back({{0.0, 640*2.0},{1000.0, -300.0}});

    lineSegments.push_back({{0.0, 0.0}, {640.0, 0.0}});
    lineSegments.push_back({{640.0, 0.0}, {640.0, 640.0}});
    lineSegments.push_back({{640.0, 640.0}, {0.0, 640.0}});
    lineSegments.push_back({{0.0, 640.0}, {0.0, 0.0}});

    Disk d{{321,40}, 20, {0,0}, false};

    disks.push_back(d);

    Disk d2{{160,25}, 15, {0,0}, false};

    disks.push_back(d2);

    for(int i = 0; i < 7; i++){
        bool doOffset = !(i % 2);
        
        int startX = 100;
        if(doOffset){
            startX = 135;
        }

        for(int j = 0; j < 6 || j < 7 && !doOffset; j++){
            Disk s{{startX + j * 70.0, 100 + i * 80.0}, 4, {0,0}, true};
            disks.push_back(s);
        }
        
    }

    for(int l = 40; l < 640; l += 50){
        Disk x{{(double)l + 1.0, 40 + l/50 * 2.0}, 20, {0,0}, false};
        disks.push_back(x);
    }

    /*Disk d1{{1.0, 620}, 20, {400, -100}, false};
    Disk d2{{600.0, 620}, 20, {-100, -170}, false};*/

    /*Disk d1{{30, 10}, 20, {0.0, 0.0}, false};
    Disk d2{{55, 620}, 20, {0.0, 0.0}, false};

    disks.push_back(d2);
    disks.push_back(d1);*/

    /*Disk d1{{30, 620}, 20, {100.0,0.0}, false};
    disks.push_back(d1);
    for(int i = 0; i < 10; i++){
        Disk d{{100 + i * 50.0, 620}, 20, {0.0,0.0}, false};
        disks.push_back(d);
    }*/

    /*disks.push_back({{55, 620}, 20, {400.0, 0.0}, false});
    lineSegments.push_back({{320, 640.0 - 25.0},{640, 640.0 - 24.0}});*/

}

void Model::setDeltaTime(double deltaTime){
    this->deltaTime = deltaTime;
}

void Model::step(){
    if(!go){
        return;
    }

    std::vector<std::pair<Disk*,Disk*>> dynamicCollisions;

    for(unsigned int i = 0; i < disks.size(); i++){
        Disk& iDisk = disks[i];

        if(!iDisk.isStatic){
            iDisk.velocity.y += 1000 * deltaTime; //TODO replace with deltatime
            iDisk.origin.y += iDisk.velocity.y * deltaTime;
            iDisk.origin.x += iDisk.velocity.x * deltaTime;
        }

        for(unsigned int j = 0; j < disks.size(); j++){
            if(iDisk.isStatic == true){
                break;
            }
            if(i == j){
                continue;
            }
            Disk& jDisk = disks[j];
            
            //std::cout << "Distance Squared: " << (iDisk.origin.x - jDisk.origin.x) * (iDisk.origin.x - jDisk.origin.x) + (iDisk.origin.y - jDisk.origin.y) * (iDisk.origin.y - jDisk.origin.y) << "\n";
            //std::cout << "Combined Radius Squared: " << (jDisk.radius + iDisk.radius) * (jDisk.radius + iDisk.radius) << "\n"; 
            double distanceSquared = (iDisk.origin.x - jDisk.origin.x) * (iDisk.origin.x - jDisk.origin.x) + (iDisk.origin.y - jDisk.origin.y) * (iDisk.origin.y - jDisk.origin.y);

            if(isCollidingDiskDisk(iDisk, jDisk)){
                Vec2D normal;
                normal.x = jDisk.origin.x - iDisk.origin.x;
                normal.y = jDisk.origin.y - iDisk.origin.y;
                
                if(jDisk.isStatic){
                    collisionStaticDiskDynamicDisk(jDisk, iDisk, 0.6);
                }else{
                    double overlap = 0.5 * (jDisk.radius + iDisk.radius - sqrt(distanceSquared)); 

                    iDisk.origin.x -= overlap * (normal.x)/(sqrt(distanceSquared));
                    iDisk.origin.y -= overlap * (normal.y)/(sqrt(distanceSquared));

                    jDisk.origin.x += overlap * (normal.x)/(sqrt(distanceSquared));
                    jDisk.origin.y += overlap * (normal.y)/(sqrt(distanceSquared));

                    bool alreadyAdded = false;
                    for(int k = 0; k < dynamicCollisions.size(); k++){
                        std::pair<Disk*,Disk*> p{&disks[j], &disks[i]};
                        if(dynamicCollisions[k] == p){
                            alreadyAdded = true;
                        }
                    }

                    if(!alreadyAdded){
                        dynamicCollisions.push_back({&disks[i], &disks[j]});
                    }
                }
            }

        }

        for(unsigned int l = 0; l < lineSegments.size(); l++){
            collisionStaticLineSegmentDynamicDisk(lineSegments[l], iDisk, 0.6);
        }

        /*if(iDisk.origin.y + iDisk.radius >= 640){
            iDisk.velocity = reflectionDampened({0, -1.0}, iDisk.velocity, 0.6);
            iDisk.origin.y = 640 - iDisk.radius;
        }

        if(iDisk.origin.x + iDisk.radius >= 640){
            iDisk.velocity = reflectionDampened({-1.0, 0}, iDisk.velocity, 0.6);
            iDisk.origin.x = 640 - iDisk.radius;
        }

        if(iDisk.origin.x - iDisk.radius <= 0){
            iDisk.velocity = reflectionDampened({1.0, 0}, iDisk.velocity, 0.6);
            iDisk.origin.x = iDisk.radius;
        }*/

    }

    for(auto& collision : dynamicCollisions){
        //TODO derive this and use masses
        collisionDynamicDiskDynamicDisk(*(collision.first), *(collision.second));
    }
}

std::vector<Disk>& Model::viewDisks(){
    return disks;
}

std::vector<LineSegment>& Model::viewLineSegments(){
    return lineSegments;
}