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




cplusplus {{

    #include <vector>
    #include <list>

    #include "../field/NID.h"
    #include "../field/SID.h"
    using PID_t = int;
    using Port_t = unsigned;
    
    typedef std::vector<PID_t> PIDS;
    typedef std::list<NID> NIDS;
    typedef std::vector<int> path;
}}


enum TYPE
{
    GET=1;
    DATA=2;
};

class path
{
    @existingClass;
}

class NIDS
{
    @existingClass;
}

class NID
{
    @existingClass;
}

class SID
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



class Port_t
{
    @existingClass;
    @opaque;
}