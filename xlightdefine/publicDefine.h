#ifndef _PUBLICDEFINE_H
#define _PUBLICDEFINE_H

//#include "stdio.h"
//#include "string.h"

// Common Data Type
#define UC                        uint8_t
#define US                        uint16_t
#define UL                        uint32_t
#define SHORT                     int16_t
#define LONG                      int32_t

// Node type
#define NODE_TYP_GW               'g'
#define NODE_TYP_LAMP             'l'
#define NODE_TYP_REMOTE           'r'
#define NODE_TYP_SYSTEM           's'
#define NODE_TYP_AC               'a'
#define NODE_TYP_CURTAIN          'c'
#define NODE_TYP_AIRPURE          'p'
#define NODE_TYP_SWITCH           'w'
#define NODE_TYP_THIRDPARTY       't'

// NodeID Convention
#ifdef RF24
#define NODEID_GATEWAY          0
#else
#define NODEID_GATEWAY          1
#endif
#define NODEID_MIN_LAMP         8
#define NODEID_MAX_LAMP         31
#define NODEID_MIN_COLORFULBAR  32
#define NODEID_MAX_COLORFULBAR  39
#define NODEID_MIN_SWITCH       40
#define NODEID_MAX_SWITCH       63
#define NODEID_MIN_REMOTE       64
#define NODEID_MAX_REMOTE       71
#define NODEID_MIN_AC           72
#define NODEID_MAX_AC           79
#define NODEID_MIN_FAN          80
#define NODEID_MAX_FAN          95
#define NODEID_MIN_IR           96
#define NODEID_MAX_IR           127
#define NODEID_PROJECTOR        128
#define NODEID_KEYSIMULATOR     129
#define NODEID_MIN_SUPERSENSOR  130
#define NODEID_MAX_SUPERSENSOR  138
#define NODEID_SMARTPHONE       139
#define NODEID_MIN_AIRPURE      140
#define NODEID_MAX_AIRPURE      147
#define NODEID_MIN_CURTAIN      148
#define NODEID_MAX_CURTAIN      179
#define NODEID_MIN_GROUP        192
#define NODEID_MAX_GROUP        223
#define NODEID_MIN_TRANS        224
#define NODEID_MAX_TRANS        231
#define NODEID_RF_SCANNER       250
#define NODEID_DUMMY            255
#define BASESERVICE_ADDRESS     0xFE
#define BROADCAST_ADDRESS       0xFF

// Target Type (mask)
#define ZEN_TARGET_CURTAIN      0x80
#define ZEN_TARGET_AIRPURIFIER  0x88
#define ZEN_TARGET_AIRCONDITION 0x8C
#define ZEN_TARGET_SWITCH       0x90
#define ZEN_TARGET_SMARTSOCKET  0xA0
#define ZEN_TARGET_FAN          0xB0
#define ZEN_TARGET_COLORFULBAR  0xB8
#define ZEN_TARGET_SUPERSENSOR  0xC0

#define IS_TARGET_CURTAIN(nTag)         (nTag >= ZEN_TARGET_CURTAIN && nTag < ZEN_TARGET_AIRPURIFIER)
#define IS_TARGET_AIRPURIFIER(nTag)     (nTag >= ZEN_TARGET_AIRPURIFIER && nTag < ZEN_TARGET_AIRCONDITION)
#define IS_TARGET_AIRCONDITION(nTag)    (nTag >= ZEN_TARGET_AIRCONDITION && nTag < ZEN_TARGET_SWITCH)
#define IS_TARGET_SWITCH(nTag)          (((nTag) & 0xF0) == ZEN_TARGET_SWITCH)
#define IS_TARGET_SMARTSOCKET(nTag)     (((nTag) & 0xF0) == ZEN_TARGET_SMARTSOCKET)
#define IS_TARGET_FAN(nTag)             (nTag >= ZEN_TARGET_FAN && nTag < ZEN_TARGET_COLORFULBAR)
#define IS_TARGET_COLORFULBAR(nTag)     (nTag >= ZEN_TARGET_COLORFULBAR && nTag < ZEN_TARGET_SUPERSENSOR)

#define IS_GROUP_NODEID(nID)        (nID >= NODEID_MIN_GROUP && nID <= NODEID_MAX_GROUP)
#define IS_SPECIAL_NODEID(nID)      (nID >= NODEID_PROJECTOR && nID <= NODEID_SMARTPHONE)
#define IS_LAMP_NODEID(nID)          (nID >= NODEID_MIN_LAMP && nID <= NODEID_MAX_LAMP)
#define IS_SWITCH_NODEID(nID)        (nID >= NODEID_MIN_SWITCH && nID <= NODEID_MAX_SWITCH)
#define IS_REMOTE_NODEID(nID)       (nID >= NODEID_MIN_REMOTE && nID <= NODEID_MAX_REMOTE)
#define IS_AC_NODEID(nID)           (nID >= NODEID_MIN_AC && nID <= NODEID_MAX_AC)
#define IS_FAN_NODEID(nID)           (nID >= NODEID_MIN_FAN && nID <= NODEID_MAX_FAN)
#define IS_AIRPURE_NODEID(nID)       (nID >= NODEID_MIN_AIRPURE && nID <= NODEID_MAX_AIRPURE)
#define IS_CURTAIN_NODEID(nID)       (nID >= NODEID_MIN_CURTAIN && nID <= NODEID_MAX_CURTAIN)
#define IS_COLORFULBAR_NODEID(nID)   (nID >= NODEID_MIN_COLORFULBAR && nID <= NODEID_MAX_COLORFULBAR)
#define IS_TRANSMODULE_NODEID(nID)   (nID >= NODEID_MIN_TRANS && nID <= NODEID_MAX_TRANS)
#define IS_SENSOR_NODEID(nID)        (nID >= NODEID_MIN_SUPERSENSOR && nID <= NODEID_MAX_SUPERSENSOR)

#define IS_MINE_SUBID(nSID)        ((nSID) == 0 || ((nSID) & gConfig.subID))

// RF channel for the sensor net, 0-127
#define RF24_CHANNEL	   		100

// System Startup Status
#define SYS_INIT                        0
#define SYS_RESET                       1
#define SYS_WAIT_NODEID                 2
#define SYS_WAIT_PRESENTED              3
#define SYS_RUNNING                     5

#define UNIQUE_ID_LEN           8

#endif /* _PUBLICDEFINE_H */
