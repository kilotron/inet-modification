#include "inet/mobility/static/StaticTriMobility.h"

namespace inet {

Define_Module(StaticTriMobility);


void StaticTriMobility::setInitialPosition()
{

    double distance = par("distance");
    double skewX=par("skewX");
    double skewY=par("skewY");
    unsigned int index = subjectModule->getIndex();

    unsigned int  layer=0;
    unsigned int reside;
    int i=1;
    int j=index+1;
    while(j>0)
    {
        layer+=1;
        j-=i;
        i++;
    }
    reside = j+i-1;


    if(constraintAreaMin.x > -INFINITY) {
        lastPosition.x += constraintAreaMin.x;
    }

    if(constraintAreaMin.y > -INFINITY) {
        lastPosition.y += constraintAreaMin.y;
    }
    
    if(index > 0) {
        
        double angle = 1.0/6.0 * M_PI;
        lastPosition.x = (layer-1)*distance*cos(angle)+skewX;
        lastPosition.y = (reside-1)*distance-(layer-1)*distance*sin(angle)+skewY;
    }
    else
    {
        lastPosition.x=0+skewX;
        lastPosition.y=0+skewY;
    }   

    lastPosition.z = par("initialZ");
    recordScalar("x", lastPosition.x);
    recordScalar("y", lastPosition.y);
    recordScalar("z", lastPosition.z);
}

} // namespace inet
