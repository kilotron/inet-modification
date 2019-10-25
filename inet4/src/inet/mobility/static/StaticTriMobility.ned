package inet.mobility.static;

import inet.mobility.base.MobilityBase;

//
// Places all hosts on concentric circles
//
// TODO: rename to StaticTriMobility?
simple StaticTriMobility extends MobilityBase
{
    parameters:
        int numHosts;
        double initialZ @unit(m) = default(0m);
        double distance @unit(m) = default(100m);
        double skewX @unit(m)=default(50m);
		double skewY @unit(m)=default(50m);
        @class(StaticTriMobility);
}