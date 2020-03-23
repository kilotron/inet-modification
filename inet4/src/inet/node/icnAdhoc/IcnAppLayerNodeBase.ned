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

package inet.node.icnAdhoc;

import inet.applications.contract.IApp;
import inet.common.MessageDispatcher;

module IcnAppLayerNodeBase extends IcnNetworkLayerNodeBase
{
    
    parameters:       
        numWlanInterfaces = default(3);
        int numApps = default(1);
        @display("i=device/cellphone");
        wlan[*].mgmt.typename = default("Ieee80211MgmtAdhoc");
        wlan[*].agent.typename = default("");
    
     submodules:
        app[numApps]: <> like IApp {
            parameters:
                @display("p=375,76,row,150");
        }
        at: MessageDispatcher {
            parameters:
                @display("p=750,146;b=1000,5,,,,1");
        }
        
    connections allowunconnected:
        for i=0..numApps-1 {
            app[i].socketOut --> at.in++;
            app[i].socketIn <-- at.out++;
        }
        
        at.out++ --> color.transportIn ;
        at.in++ <-- color.transportOut ;
    
}