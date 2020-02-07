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

package inet.node.aodv_cluster;

import inet.common.MessageDispatcher;
import inet.node.base.NetworkLayerNodeBase;
import inet.networklayer.icn.cluster.ICluster;
import inet.networklayer.icn.cluster.SimpleCluster;

module Network_clusterNodeBase extends NetworkLayerNodeBase
{
     submodules:
         cluster: SimpleCluster {
            parameters:
                @display("p=676,200;q=pendingQueue");
        }
}
