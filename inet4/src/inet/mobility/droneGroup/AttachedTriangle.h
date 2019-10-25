/*
 * AttachedTriangle.h
 *
 *  Created on: 2019年10月25日
 *      Author: hiro
 */

#ifndef INET_MOBILITY_DRONEGROUP_ATTACHEDTRIANGLE_H_
#define INET_MOBILITY_DRONEGROUP_ATTACHEDTRIANGLE_H_

#include "inet/mobility/single/AttachedMobility.h"

namespace inet{

class INET_API AttachedTriangle : public AttachedMobility
{
    protected:
        double distance;

    public:
        void initialize(int stage) override;
};
}

#endif /* INET_MOBILITY_DRONEGROUP_ATTACHEDTRIANGLE_H_ */
