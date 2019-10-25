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

simple color like icn
{
    parameters:
        string interfaceTableModule = "";   // The path to the InterfaceTable module
        string routingTableModule="";
        string clusterModule="";

        int hopLimit = default(5);
        // double interval;
        double procDelay @unit(s) = default(0s);
        int mtu=2304;
        
        
    gates:
        input transportIn ;
        output transportOut;
        input queueIn;
        output queueOut;
}