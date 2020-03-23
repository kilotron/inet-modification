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


import inet.common.MessageDispatcher;
import inet.node.base.NetworkLayerNodeBase;
import inet.networklayer.icn.cluster.ICluster;
import inet.networklayer.icn.cluster.SimpleCluster;
import inet.node.inet.AdhocHost;
import inet.routing.aodv_cluster.Aodv_cluster;
import inet.routing.aodv.Aodv;
import inet.networklayer.icn.color.color;
import inet.networklayer.icn.color.colorCluster;

module icnHost extends AdhocHost
{
    parameters:
        numWlanInterfaces = 1;
        numLoInterfaces = 0;
    submodules:

        color: color {
            parameters:
                @display("p=956.02496,374.84998;q=queue");
        }

        cluster: SimpleCluster {
            parameters:
                @display("p=800,376;q=pendingQueue");
        }
        aodv: Aodv_cluster {
            @display("p=825,226");
        }
    connections allowunconnected:
        cluster.ifOut --> nl.in++;
        cluster.ifIn <-- nl.out++;

        color.ifIn <-- nl.out++;
        color.ifOut --> nl.in++;

        at.out++ --> color.transportIn;
        at.in++ <-- color.transportOut;

        aodv.ipOut --> tn.in++;
        aodv.ipIn <-- tn.out++;

}
