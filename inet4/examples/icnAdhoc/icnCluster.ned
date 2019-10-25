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

package inet.examples.icnAdhoc;

import inet.common.scenario.ScenarioManager;
//import inet.node.inet.AdhocHost;
import inet.physicallayer.ieee80211.packetlevel.Ieee80211ScalarRadioMedium;
import inet.visualizer.contract.IIntegratedVisualizer;
import inet.visualizer.integrated.IntegratedVisualizer;
import inet.networklayer.configurator.color.ColorConfigurator;
import inet.node.icnAdhoc.testHost;


network cluster
{
    int numHosts = default(4);
    int move= default(0);
    int group1Num= default(0);
    int group2Num = default(0);
	int Cindex = default(0);
    int Pindex = default(8);
    double sentInterval = default(0.001) ;
    submodules:
        visualizer: <default("IntegratedCanvasVisualizer")> like IIntegratedVisualizer if hasVisualizer() {
            @display("p=100,300;is=s");
        }
//        configurator: ColorConfigurator {
//            @display("p=580,200");
//        }
        scenarioManager: ScenarioManager {
            parameters:
                script = default(xml("<scenario/>"));
                @display("p=100,400;is=s");
        }
        radioMedium: Ieee80211ScalarRadioMedium {
            @display("p=580,275");
        }
        host[move]: testHost {
            parameters:
                @display("r=,,#707070;p=300,200");

        }
        staticHost[numHosts-move]:testHost
        {
            
        }
        group1[group1Num] :testHost
        {
            
        }
        group2[group2Num]:testHost
        {
            
        }
        
}