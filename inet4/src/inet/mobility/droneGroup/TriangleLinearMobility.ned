//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package inet.mobility.droneGroup;

import inet.mobility.base.MovingMobilityBase;

//
// This is a linear mobility model with speed, angle and acceleration parameters.
// Angle only changes when the mobile node hits a wall: then it reflects
// off the wall at the same angle.
//
// @author Li
//
simple TriangleLinearMobility extends MovingMobilityBase
{
    parameters:
//        double initialX @unit(m) = default(uniform(this.constraintAreaMinX, this.constraintAreaMaxX));
//        double initialY @unit(m) = default(uniform(this.constraintAreaMinY, this.constraintAreaMaxY));
//        double initialZ @unit(m) = default(nanToZero(uniform(this.constraintAreaMinZ, this.constraintAreaMaxZ)));
		double initialZ @unit(m) = default(0m);
        double distance @unit(m) = default(15m);
        double skewX @unit(m)=default(50m);
		double skewY @unit(m)=default(50m);
        bool initFromDisplayString = default(true); // enables one time initialization from the subject module's display string
        double speed @unit(mps) = default(0mps); // speed of the host
        double initialMovementHeading @unit(deg) = default(uniform(0deg, 360deg));
        double initialMovementElevation @unit(deg) = default(0deg);
        @class(TriangleLinearMobility);
}