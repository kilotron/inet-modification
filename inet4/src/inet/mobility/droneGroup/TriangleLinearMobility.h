/*
 * TriangleLinearMobility.h
 *
 *  Created on: 2019年4月11日
 *      Author: hiro
 */

#ifndef INET_MOBILITY_DRONEGROUP_TRIANGLELINEARMOBILITY_H_
#define INET_MOBILITY_DRONEGROUP_TRIANGLELINEARMOBILITY_H_

#include "inet/common/INETDefs.h"
#include "inet/mobility/base/MovingMobilityBase.h"

namespace inet {

class INET_API TriangleLinearMobility: public MovingMobilityBase {

protected:
    double speed;

protected:
    virtual int numInitStages() const override { return NUM_INIT_STAGES; }

    /** @brief Initializes mobility model parameters.*/
    virtual void initialize(int stage) override;

    virtual void setInitialPosition() override;

    /** @brief Move the host*/
    virtual void move() override;

public:
    virtual double getMaxSpeed() const override { return speed; }
    TriangleLinearMobility();
};

} /* namespace inet */
#endif /* INET_MOBILITY_DRONEGROUP_TRIANGLELINEARMOBILITY_H_ */

