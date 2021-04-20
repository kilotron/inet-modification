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

package inet.applications.pccpapp;

import inet.applications.contract.IApp;
import inet.applications.pccpapp.PccpApp;

simple PccpBurstApp extends PccpApp
{
    parameters:
        @class(PccpBurstApp);
//        int transNum = default(1000);		// 一次传输数据包的个数，默认1000个（1MB）
//        double sleepTime @unit(s) = default(5s); // 传输一次数据后，等待sleepTime后再次开始传输数据
}