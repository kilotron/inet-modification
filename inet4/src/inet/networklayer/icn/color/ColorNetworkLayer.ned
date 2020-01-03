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

import inet.common.MessageDispatcher;
import inet.networklayer.contract.INetworkLayer;
//import inet.networklayer.configurator.color.ColorConfigurator;
import inet.networklayer.icn.base.icn;
import inet.networklayer.icn.cluster.ICluster;
import inet.networklayer.icn.cluster.SimpleCluster;
import inet.networklayer.icn.cacheTable.ColorCacheTable;
import inet.networklayer.icn.pendingTable.colorPendingGetTable;
import inet.networklayer.icn.routingTable.ColorRoutingTable;


module ColorNetworklayer like INetworkLayer
{
    parameters:
        string nid = default("xxx");
    gates:
        input ifIn @labels(INetworkHeader);
        output ifOut @labels(INetworkHeader);
        input transportIn @labels(ColorControlInfo/down);
        output transportOut @labels(ColorControlInfo/up);
    submodules:
        //        configurator: ColorConfigurator {
        //            parameters:
        //                @display("p=100,100;is=s");
        //        }
        routingTable: ColorRoutingTable {
            parameters:
                @display("p=204,114;is=s");
        }
        PendingTable: colorPendingGetTable {
            parameters:
                @display("p=204,257;is=s");
        }
        CacheTable: ColorCacheTable {
            parameters:
                @display("p=204,192;is=s");
        }
        up: MessageDispatcher {
            parameters:
                @display("p=550,100;b=600,5");
        }
        lp: MessageDispatcher {
            parameters:
                @display("p=550,300;b=600,5");
        }
        colorNet: <default("colorCluster")> like icn {
            parameters:
                @display("p=400,200;q=queue");
        }
        cluster: SimpleCluster {
            parameters:
                @display("p=676,200;q=pendingQueue");
        }

    connections allowunconnected:
        transportIn --> { @display("m=n"); } --> up.in++;
        transportOut <-- { @display("m=n"); } <-- up.out++;


        up.out++ --> colorNet.transportIn;
        up.in++ <-- colorNet.transportOut;

        cluster.ifOut --> lp.in++;
        cluster.ifIn <-- lp.out++;

        colorNet.queueOut --> lp.in++;
        colorNet.queueIn <-- lp.out++;

        lp.out++ --> { @display("m=s"); } --> ifOut;
        lp.in++ <-- { @display("m=s"); } <-- ifIn;
}
