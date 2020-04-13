
#ifndef __INET_STATICTRIMOBILITY_H
#define __INET_STATICTRIMOBILITY_H


#include "inet/mobility/base/StationaryMobilityBase.h"


namespace inet {

/**
 * @brief Mobility model which places all hosts on concenctric circles
 *
 * @ingroup mobility
 * @author li
 */
class INET_API StaticTriMobility : public inet::StationaryMobilityBase
{
  protected:
    /** @brief Initializes the position according to the mobility model. */
    virtual void setInitialPosition() override;

    
  public:
    StaticTriMobility() {};
};

}

#endif
