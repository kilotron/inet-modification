/*
 * enumList.h
 *
 *  Created on: 2016��5��25��
 *      Author: lhyi2
 */

#ifndef ENUMLIST_H_
#define ENUMLIST_H_

enum class NodeType {
        UNDEFINED = 0,
        HOST = 1,
        ARNODE = 2,
        BRNODE = 3,
        RMNODE = 4,
        ROUTER = 5,
};

enum class PIDState {
    UNDEFINED = 0,
    PIDSTABLE = 1,
    PIDNEGO = 2,
    PIDDIST = 3,
};

enum class PIDMessage {
    UNDEFINED = 0,
    PIDNEGOTIATE = 1,
    PIDCONFIRM = 2,
};

enum class RouteType {
    DIRECT = 0,
    REMOTE = 1,
    DEFAULT = 2,
};

enum class RouteSource {
    MANUAL = 0,
    INTERFACEMASK = 1,
};

enum class ProtocolType {
    IPV4 = 0,
    IPV6 = 1,
    CoLoR = 2,
};

enum class InterfaceType {
    PPP = 0,
    ETH = 1,
};

enum class ModuleType {
    UNKNOWN = 0,
    TransportControl = 1,
    GetManipulate = 2,
    RegManipulate = 3,
    DataManipulate = 4,
    PIDUpdateManipulate = 5,
};

enum class ModuleState {
    UNKNOWN = 0,
    DOWN = 1,
    UP = 2,
    SLEEP = 3,
};

enum class NodeNotifyState {
    Reg = 0,
    Leave = 1,
    RMNotify = 2,
    nonRMNotify = 3,
};

enum class ASRelatationShip {
    Parent = 0,
    Peer = 1,
};

#define NOTINCLUDED 4;
#define NOCOMMON 4;

#define NewSIDAdded 0;
#define NewASAdded 1;
#define NewNodeAdded 2;
#endif /* EMUNLIST_H_ */
