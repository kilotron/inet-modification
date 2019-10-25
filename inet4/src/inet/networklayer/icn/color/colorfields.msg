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
import inet.common.INETDefs;


namespace inet;

cplusplus {{
    #include <omnetpp.h>
    #include <stdint.h>
    #include <vector>
    #include <list>
    #include "../field/dataType.h"
    
    typedef std::vector<PID_t> PIDS;
    typedef std::list<NID_t> NIDS;
    typedef std::vector<int> path;
}}

enum TYPE
{
    intrest=1;
    data=2;
};

class path
{
    @existingClass;
}

class NIDS
{
    @existingClass;
}

class PID_t
{
    @existingClass;
    @opaque;
}

class PIDS
{
    @existingClass;
}

class SID_t
{
    @existingClass;
    @opaque;
}

class NID_t
{
    @existingClass;
    @opaque;
}

class Port_t
{
    @existingClass;
    @opaque;
}