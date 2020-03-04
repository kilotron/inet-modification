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

package inet.networklayer.icn.color;
import inet.networklayer.icn.base.icn;

simple colorTwoInterface like icn
{
    parameters:
        @class("colorTwoInterface");
        string interfaceTableModule = "";   // The path to the InterfaceTable module
        string routingTableModule="";
        string clusterModule="";

        int hopLimit = default(5);
        // double interval;
        double GetProcDelay @unit(s) = default(0s);
        double DataProcDelay @unit(s) = default(0s);
        int mtu=2304;
  
        int multi;

        double getDelayTime = default(0.001);
        double dataDelayTime  = default(0.001);
        int TC = default(2);

        bool flood = default(true);
        bool unicast = default(false);
        double routeLifeTime @unit(s) = default(1.0s);
    gates:
        input transportIn ;
        output transportOut;
        input queueIn;
        output queueOut;
}