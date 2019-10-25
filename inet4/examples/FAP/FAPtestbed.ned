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

package inet.examples.FAP;

import inet.common.scenario.ScenarioManager;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
//import inet.node.inet.AdhocHost;
import inet.networklayer.ipv4.RoutingTableRecorder;
import inet.node.aodv.AodvRouter;
import inet.physicallayer.ieee80211.packetlevel.Ieee80211ScalarRadioMedium;
import inet.visualizer.contract.IIntegratedVisualizer;
import inet.visualizer.integrated.IntegratedVisualizer;

import inet.node.inet.AdhocHost;


network fapNet
{
    int numHosts;
    int move;
    submodules:
        visualizer: <default("IntegratedCanvasVisualizer")> like IIntegratedVisualizer if hasVisualizer() {
            @display("p=100,300;is=s");
        }
//        visualizer: IntegratedVisualizer {
//            @display("p=100,50");
//        }
        configurator: Ipv4NetworkConfigurator {
            @display("p=580,200");
        }
        scenarioManager: ScenarioManager {
            parameters:
                script = default(xml("<scenario/>"));
                @display("p=100,400;is=s");
        }
        radioMedium: Ieee80211ScalarRadioMedium {
            @display("p=580,275");
        }
        host[move]: AodvRouter {
            parameters:
                @display("r=,,#707070;p=300,200");
        }
        staticHost[numHosts-move]:AodvRouter{
        }
}