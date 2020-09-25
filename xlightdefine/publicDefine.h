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
#define BOOL                      uint8_t

// Hardware: Node Type
typedef enum
{
    NODE_TYP_ALL = 0,
    NODE_TYP_LAMP = 1,              // 1
    NODE_TYP_CURTAIN,               // 2
    NODE_TYP_AC,                    // 3
    NODE_TYP_FAN,                   // 4
    NODE_TYP_AIRPURE,               // 5
    NODE_TYP_PANEL,                 // 6, Same as REMOTE
    NODE_TYP_REMOTE = 6,            // 6, Same as PANEL
    NODE_TYP_COLORBAR,              // 7, Same as Spotlight
    NODE_TYP_SPOTLIGHT = 7,         // 7, Same as Colorbar
    NODE_TYP_SWITCH,                // 8, Same as Socket
    NODE_TYP_SOCKET = 8,            // 8, Same as Smart Switch
    NODE_TYP_MODBUS,                // 9
    NODE_TYP_IR,                    // 10
    NODE_TYP_THIRDPARTY,            // 11, Same as Others
    NODE_TYP_OTHERS = 11,           // 11, Same as Thirdparty
    NODE_TYP_SENSOR,                // 12
    NODE_TYP_SPECIAL,               // 13
    NODE_TYP_GROUP,                 // 14
    NODE_TYP_GW,                    // 15
    NODE_TYP_SYSTEM,                // 16
    NODE_TYP_DUMMY = 255            // 255
} eNodeType_t;

// Hardware: Sensor Type
typedef enum
{
    SEN_TYP_ALL = 0,
    SEN_TYP_ZENSOR,                 // 1
    SEN_TYP_ALS,                    // 2
    SEN_TYP_PIR,                    // 3
    SEN_TYP_DHT,                    // 4
    SEN_TYP_SICK,                   // 5
    SEN_TYP_NOISE,                  // 6
    SEN_TYP_SMOKE,                  // 7 (actual the last one)
    SEN_TYP_DUMMY = 255             // 255
} eSensorType_t;

// NodeID Convention
#ifdef RF24
#define NODEID_GATEWAY          0
#define NODEID_MAINDEVICE       1
#else
#define NODEID_GATEWAY          1
#define NODEID_MAINDEVICE       NODEID_MIN_LAMP
#endif
#define NODEID_MIN_LAMP         8
#define NODEID_MAX_LAMP         31
#define NODEID_MIN_COLORBAR     32
#define NODEID_MAX_COLORBAR     39
#define NODEID_MIN_SWITCH       40
#define NODEID_MAX_SWITCH       63
#define NODEID_MIN_REMOTE       64
#define NODEID_MAX_REMOTE       79
#define NODEID_MIN_FAN          80
#define NODEID_MAX_FAN          95
#define NODEID_MIN_AC           96
#define NODEID_MAX_AC           111
#define NODEID_MIN_IR           112
#define NODEID_MAX_IR           123
#define NODEID_MIN_SUPERSENSOR  124
#define NODEID_MAX_SUPERSENSOR  139
#define NODEID_MIN_AIRPURE      140
#define NODEID_MAX_AIRPURE      147
#define NODEID_MIN_CURTAIN      148
#define NODEID_MAX_CURTAIN      179
#define NODEID_MIN_MODBUS       180
#define NODEID_MAX_MODBUS       185
#define NODEID_PROJECTOR        186
#define NODEID_KEYSIMULATOR     187
#define NODEID_SMARTPHONE       188
//  188-191: reserved for special devices
//  192-207: Node Groups
#define NODEID_MIN_GROUP        192
#define NODEID_MAX_GROUP        207
//  208-249: reserved for future extension
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
#define ZEN_TARGET_PIRSENSOR    0xC8
#define ZEN_TARGET_ALSSENSOR    0xC9

#define IS_TARGET_CURTAIN(nTag)         (nTag >= ZEN_TARGET_CURTAIN && nTag < ZEN_TARGET_AIRPURIFIER)
#define IS_TARGET_AIRPURIFIER(nTag)     (nTag >= ZEN_TARGET_AIRPURIFIER && nTag < ZEN_TARGET_AIRCONDITION)
#define IS_TARGET_AIRCONDITION(nTag)    (nTag >= ZEN_TARGET_AIRCONDITION && nTag < ZEN_TARGET_SWITCH)
#define IS_TARGET_SWITCH(nTag)          (((nTag) & 0xF0) == ZEN_TARGET_SWITCH)
#define IS_TARGET_SMARTSOCKET(nTag)     (((nTag) & 0xF0) == ZEN_TARGET_SMARTSOCKET)
#define IS_TARGET_FAN(nTag)             (nTag >= ZEN_TARGET_FAN && nTag < ZEN_TARGET_COLORFULBAR)
#define IS_TARGET_COLORFULBAR(nTag)     (nTag >= ZEN_TARGET_COLORFULBAR && nTag < ZEN_TARGET_SUPERSENSOR)

#define IS_VALID_NODEID(nID)        ((nID) > NODEID_GATEWAY && (nID) < NODEID_RF_SCANNER)
#define IS_LAMP_NODEID(nID)         (((nID) >= NODEID_MIN_LAMP && (nID) <= NODEID_MAX_LAMP) || (nID) == NODEID_MAINDEVICE)
#define IS_COLORBAR_NODEID(nID)     ((nID) >= NODEID_MIN_COLORBAR && (nID) <= NODEID_MAX_COLORBAR)
#define IS_SWITCH_NODEID(nID)       ((nID) >= NODEID_MIN_SWITCH && (nID) <= NODEID_MAX_SWITCH)
#define IS_REMOTE_NODEID(nID)       ((nID) >= NODEID_MIN_REMOTE && (nID) <= NODEID_MAX_REMOTE)
#define IS_NOT_REMOTE_NODEID(nID)   ((nID) < NODEID_MIN_REMOTE || (nID) > NODEID_MAX_REMOTE)
#define IS_FAN_NODEID(nID)          ((nID) >= NODEID_MIN_FAN && (nID) <= NODEID_MAX_FAN)
#define IS_AC_NODEID(nID)           ((nID) >= NODEID_MIN_AC && (nID) <= NODEID_MAX_AC)
#define IS_NOT_AC_NODEID(nID)       ((nID) < NODEID_MIN_AC || (nID) > NODEID_MAX_AC)
#define IS_IR_NODEID(nID)           ((nID) >= NODEID_MIN_IR && (nID) <= NODEID_MAX_IR)
#define IS_SENSOR_NODEID(nID)       ((nID) >= NODEID_MIN_SUPERSENSOR && (nID) <= NODEID_MAX_SUPERSENSOR)
#define IS_AIRPURE_NODEID(nID)      ((nID) >= NODEID_MIN_AIRPURE && (nID) <= NODEID_MAX_AIRPURE)
#define IS_CURTAIN_NODEID(nID)      ((nID) >= NODEID_MIN_CURTAIN && (nID) <= NODEID_MAX_CURTAIN)
#define IS_MODBUS_NODEID(nID)       ((nID) >= NODEID_MIN_MODBUS && (nID) <= NODEID_MAX_MODBUS)
#define IS_SPECIAL_NODEID(nID)      ((nID) >= NODEID_PROJECTOR && (nID) <= NODEID_SMARTPHONE)
#define IS_GROUP_NODEID(nID)        ((nID) >= NODEID_MIN_GROUP && (nID) <= NODEID_MAX_GROUP)
#define IS_SYSTEM_NODEID(nID)       (((nID) > NODEID_MAINDEVICE && (nID) < NODEID_MIN_LAMP) || ((nID) >= NODEID_RF_SCANNER && (nID) <= NODEID_DUMMY))
#define IS_NOT_DEVICE_NODEID(nID)   (IS_SYSTEM_NODEID(nID) || IS_SENSOR_NODEID(nID) || IS_REMOTE_NODEID(nID) || IS_GROUP_NODEID(nID))

#define IS_MINE_SUBID(nSID)        ((nSID) == 0 || ((nSID) & gConfig.subID))

// RF channel for the sensor net, 0-127
#define RF24_CHANNEL	   		100

// System Power/Battery Status
#define SYS_ST_INIT             0   // Also as Off state
#define SYS_ST_RUNNING          1   // Normal Working
#define SYS_ST_ON_BATTERY       2   // Working on battery
#define SYS_ST_LOW_BATTERY      3   // Working on low-battery
#define SYS_ST_CHARGING         4   // Working while charging
#define SYS_ST_UPDATE           5
#define SYS_ST_ERROR            6
#define SYS_ST_SETUP            7   // WAIT_NODEID, WAIT_PRESENTED
#define SYS_ST_SLEEP            8   // Low Power Consumption
#define SYS_ST_RESET            9

#define UNIQUE_ID_LEN           8

#define ENTER_CRITICAL_SECTION( )  disableInterrupts()
#define EXIT_CRITICAL_SECTION( )   enableInterrupts()

#endif /* _PUBLICDEFINE_H */
