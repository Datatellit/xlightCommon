#ifndef __XLI_NODECONFIG_H
#define __XLI_NODECONFIG_H

// Node config protocol
/// MySensor Cmd = C_INTERNAL
/// MySensor Type = I_CONFIG

/// Use MySensor Sensor as Node Config Field (NCF), and payload as config data
#define NCF_QUERY                       0       // Query NCF, payload length = 0 (query) or n (ack)
#define NCF_MAP_SENSOR                  1       // Sensor Bitmap, payload length = 2
#define NCF_MAP_FUNC                    2       // Function Bitmap, payload length = 2

#define NCF_DEV_ASSOCIATE               10      // Associate node to device(s), payload length = 2 to 8, a device per uint16_t
#define NCF_DEV_EN_SDTM                 11      // Simple Direct Test Mode flag, payload length = 2
#define NCF_DEV_MAX_NMRT                12      // Max. Node Message Repeat Times, payload length = 2
#define NCF_DEV_SET_SUBID               13      // Set device subid, payload length = 2 [:subID][:devType]
#define NCF_DEV_CONFIG_MODE             14      // Put Device into Config Mode, payload length = 2
#define NCF_DEV_SET_RELAY_NODE          15      // Set relay node id & subID, payload length = 2
#define NCF_DEV_SET_RELAY_KEYS          16      // Set relay keys, payload length = 2 to 4

#define NCF_DEV_SET_RF                  17      // Set base rf info(nodeid[:rfchannel][:subid])
#define NCF_DEV_SET_REMOTE              18      // Set remote uniqueid
#define NCF_DEV_SET_SENSOR              19      // Set sensor adjust formula

#define NCF_PAN_SET_BTN_1               20      // Set Panel Button Action, payload length = 2
#define NCF_PAN_SET_BTN_2               21      // Set Panel Button Action, payload length = 2
#define NCF_PAN_SET_BTN_3               22      // Set Panel Button Action, payload length = 2
#define NCF_PAN_SET_BTN_4               23      // Set Panel Button Action, payload length = 2
#define NCF_PAN_SET_BTN_5               24      // Set Panel Button Action
#define NCF_PAN_SET_BTN_6               25      // Set Panel Button Action
#define NCF_PAN_SET_BTN_7               26      // Set Panel Button Action
#define NCF_PAN_SET_BTN_8               27      // Set Panel Button Action
#define NCF_PAN_SET_BTN_9               28      // Set Panel Button Action
#define NCF_PAN_SET_BTN_A               29      // Set Panel Button Action
#define NCF_PAN_SET_BTN_B               30      // Set Panel Button Action
#define NCF_PAN_SET_BTN_C               31      // Set Panel Button Action
#define NCF_PAN_SET_BTN_D               32      // Set Panel Button Action
#define NCF_PAN_SET_BTN_E               33      // Set Panel Button Action
#define NCF_PAN_SET_BTN_F               34      // Set Panel Button Action
#define NCF_PAN_SET_BTN_G               35      // Set Panel Button Action
#define NCF_PAN_SET_BTN_LED             41      // Set Button Backgroung LED

#define NCF_DATA_ALS_RANGE              50      // Lower and upper threshholds of ALS, payload length = 2
#define NCF_DATA_TEMP_RANGE             51      // Tempreture threshholds, payload length = 2
#define NCF_DATA_HUM_RANGE              52      // Humidity threshholds, payload length = 2
#define NCF_DATA_PM25_RANGE             53      // PM2.5 threshholds, payload length = 2
#define NCF_DATA_PIR_RANGE              54      // PIR control brightness (off br, on br), payload length = 2
#define NCF_DATA_FN_SCENARIO            60      // Scenario ID for Remote Fn keys (b1=fn_id, b2=scenario_id), payload length = 2
#define NCF_DATA_FN_HUE                 61      // Hue for Remote Fn keys (b1_7-4=bmDevice, b1_3-0=fn_id, b2-11=Hue), payload length = 12
#define NCF_CFG_PIRTIMEOUT              62      // Set pir delay-off timeout
#define NCF_CFG_PIR_MSG_MAX_INTERVAL    63      // Set pir msg max interval (keepalive)

#define NCF_LEN_DATA_FN_HUE             12

typedef enum
{
  controlPIR            = ((uint16_t)0x0001),   // Use PIR to control device on / off
  controlALS            = ((uint16_t)0x0002),   // Use ALS threshold to control device on / off
  constALS              = ((uint16_t)0x0004),   // Constant brightness level
  constTEMP             = ((uint16_t)0x0008),   // Constant tempreture
  constHUM              = ((uint16_t)0x0010),   // Constant humidity
  constPM25             = ((uint16_t)0x0020),   // Constant air quality
  constMIC              = ((uint16_t)0x0040),   // Use MIC threshold to control device on / off
  func_All              = ((uint8_t)0xFFFF)
}functions_bit_t;


// I_GET_NONCE sub-type
enum {
    SCANNER_PROBE = 0,
    SCANNER_SETUP_RF,           // by NodeID & SubID
    SCANNER_SETUPDEV_RF,        // by UniqueID
    
    SCANNER_GETCONFIG = 8,      // by NodeID & SubID
    SCANNER_SETCONFIG,
    SCANNER_GETDEV_CONFIG,      // by UniqueID
    SCANNER_SETDEV_CONFIG,
    
    SCANNER_TEST_NODE = 16,     // by NodeID & SubID
    SCANNER_TEST_DEVICE,        // by UniqueID
};

#endif /* __XLI_NODECONFIG_H */
