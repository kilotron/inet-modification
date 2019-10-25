/*
 * AttachedTriangle.cc
 *
 *  Created on: 2019年10月25日
 *      Author: hiro
 */

#include "inet/common/geometry/common/Quaternion.h"
#include "inet/mobility/droneGroup/AttachedTriangle.h"

namespace inet
{

Define_Module(AttachedTriangle);

void AttachedTriangle::initialize(int stage)
{
    MobilityBase::initialize(stage);
    EV_TRACE << "initializing AttachedMobility stage " << stage << endl;

    if (stage == INITSTAGE_LOCAL)
    {
        distance = par("distance").doubleValue();
        int SelfIndex = subjectModule->getIndex();
        mobility = getModuleFromPar<IMobility>(par("mobilityModule"), this, true);
        int TargetIndex = getModuleFromPar<cModule>(par("target"), this, true)->getIndex();
        
        if ((SelfIndex-TargetIndex)%2)
        {
            positionOffset.x = ((SelfIndex - TargetIndex) / 2 + 1) * distance / sqrt(2.0);
            positionOffset.y = positionOffset.x;
        }
        else
        {
            positionOffset.x = ((TargetIndex - SelfIndex) / 2) * distance / sqrt(2.0);
            positionOffset.y = - positionOffset.x;
        }

        positionOffset.z = par("offsetZ");
        auto alpha = deg(par("offsetHeading"));
        auto offsetElevation = deg(par("offsetElevation"));
        // NOTE: negation is needed, see IMobility comments on orientation
        auto beta = -offsetElevation;
        auto gamma = deg(par("offsetBank"));
        orientationOffset = Quaternion(EulerAngles(alpha, beta, gamma));
        isZeroOffset = positionOffset == Coord::ZERO;
        check_and_cast<cModule *>(mobility)->subscribe(IMobility::mobilityStateChangedSignal, this);
    }
}
} // namespace inet
