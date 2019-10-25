/*
 * Croute.cc
 *
 *  Created on: 2019年7月5日
 *      Author: hiro
 */

#include "Croute.h"

namespace inet{
    std::string Croute::str()
    {
        return "SID is " + sid.str() + "nexthop is: " + nextHop.str() + "metric is: "+ std::to_string(metric);
    }
}



