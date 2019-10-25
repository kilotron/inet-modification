/*
 * TriangleLinearMobility.cpp
 *
 *  Created on: 2019年4月11日
 *      Author: hiro
 */

#include "inet/common/INETMath.h"
#include "TriangleLinearMobility.h"

namespace inet {

Define_Module(TriangleLinearMobility);

void TriangleLinearMobility::setInitialPosition()
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

TriangleLinearMobility::TriangleLinearMobility()
{
    speed = 0;
}

void TriangleLinearMobility::initialize(int stage)
{
    MovingMobilityBase::initialize(stage);

    EV_TRACE << "initializing TriangleLinearMobility stage " << stage << endl;
    if (stage == INITSTAGE_LOCAL) {
        speed = par("speed");
        stationary = (speed == 0);
        rad heading = deg(fmod(par("initialMovementHeading").doubleValue(), 360));
        rad elevation = deg(fmod(par("initialMovementElevation").doubleValue(), 360));
        Coord direction = Quaternion(EulerAngles(heading, -elevation, rad(0))).rotate(Coord::X_AXIS);

        lastVelocity = direction * speed;
    }
}

void TriangleLinearMobility::move()
{
    double elapsedTime = (simTime() - lastUpdate).dbl();
    lastPosition += lastVelocity * elapsedTime;

    // do something if we reach the wall
    Coord dummyCoord;
    handleIfOutside(REFLECT, dummyCoord, lastVelocity);
}

} /* namespace inet */
