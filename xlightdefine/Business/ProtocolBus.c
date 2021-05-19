#include "_global.h"
#include "ProtocolBus.h"
#include "rf24l01.h"
#include "xliNodeConfig.h"

uint8_t bMsgReady = 0;
MyMessage_t g_rfReceivedMQ[RF_RCV_MQ_LEN];
uint8_t g_rfRcvMQ_W = 0;
uint8_t g_rfRcvMQ_R = 0;

// Assemble message
void build(uint8_t _destination, uint8_t _sensor, uint8_t _command, uint8_t _type, bool _enableAck, bool _isAck)
{
    sndMsg.header.version_length = PROTOCOL_VERSION;
    sndMsg.header.sender = gConfig.nodeID;
    sndMsg.header.destination = _destination;
    sndMsg.header.sensor = _sensor;
    sndMsg.header.type = _type;
    moSetCommand(_command);
    moSetRequestAck(_enableAck);
    moSetAck(_isAck);
}

bool isNodeIdRequired()
{
  return( isIdentityEmpty(gConfig.NetworkID, ADDRESS_WIDTH) || isIdentityEqual(gConfig.NetworkID, RF24_BASE_RADIO_ID, ADDRESS_WIDTH) );
}

void UpdateNodeAddress(const uint8_t _tx) {
  copyBuffer(rx_addr, gConfig.NetworkID, ADDRESS_WIDTH);
  rx_addr[0] = gConfig.nodeID;
  copyBuffer(tx_addr, gConfig.NetworkID, ADDRESS_WIDTH);
  if( _tx == NODEID_RF_SCANNER ) {
    tx_addr[0] = NODEID_RF_SCANNER;
  } else {
    //tx_addr[0] = (isNodeIdRequired() ? BASESERVICE_ADDRESS : NODEID_GATEWAY);
    tx_addr[0] = NODEID_GATEWAY;
  }
  RF24L01_setup(gConfig.rfChannel, gConfig.rfDataRate, gConfig.rfPowerLevel, BROADCAST_ADDRESS);
}

bool NeedUpdateRFAddress(const uint8_t _dest) {
  bool rc = TRUE;
  if( _dest == NODEID_RF_SCANNER && tx_addr[0] != NODEID_RF_SCANNER ) {
    UpdateNodeAddress(NODEID_RF_SCANNER);
  } else if( _dest != NODEID_RF_SCANNER && tx_addr[0] != NODEID_GATEWAY ) {
    UpdateNodeAddress(NODEID_GATEWAY);
  } else rc = FALSE;
  return rc;
}

typedef struct
{
  uint8_t offset;
  uint8_t size;
  uint8_t byUniqueid;  // whether getconfig by uniqueid
} CfgBlock;

#define OUT_CFG_MESSAGE_LEN           16
CfgBlock out_cfg_msg_buf[OUT_CFG_MESSAGE_LEN];
u8 cfg_msg_out_buf_read_ptr = 0;
u8 cfg_msg_out_buf_write_ptr = 0;

bool AddCfgOutputBuf(uint8_t offset,uint8_t size,uint8_t isNeedUniqueid) {  
  CfgBlock cfgblock;
  cfgblock.offset = offset;
  cfgblock.size = size;
  cfgblock.byUniqueid = isNeedUniqueid;
  out_cfg_msg_buf[cfg_msg_out_buf_write_ptr++] = cfgblock;
  cfg_msg_out_buf_write_ptr %= OUT_CFG_MESSAGE_LEN;
  return TRUE;
}

bool ProcessOutputCfgMsg() {
  // Send output Cfg msg
  while( cfg_msg_out_buf_read_ptr != cfg_msg_out_buf_write_ptr) {   
    CfgBlock cfgblock = out_cfg_msg_buf[cfg_msg_out_buf_read_ptr++];
    SendCfgBlock(cfgblock.offset,cfgblock.size,TRUE);
    cfg_msg_out_buf_read_ptr %= OUT_CFG_MESSAGE_LEN;
  }
  return TRUE;
}

bool SendCfgBlock(uint8_t offset,uint8_t size,uint8_t isNeedUniqueid) {
  // Send output Cfg msg  
    build(NODEID_RF_SCANNER, gConfig.subID, C_INTERNAL, I_GET_NONCE_RESPONSE, 0, 1);
    // Common payload
    sndMsg.payload.data[0] = SCANNER_GETDEV_CONFIG;
    sndMsg.payload.data[1] = offset;
    uint8_t custom_playload = 2;
    if(isNeedUniqueid != 0) 
    {
      copyBuffer(sndMsg.payload.data + 2, _uniqueID, UNIQUE_ID_LEN);
      custom_playload += UNIQUE_ID_LEN;
    }  
    copyBuffer(sndMsg.payload.data + custom_playload, (UC *)((uint16_t)(&gConfig) + offset), size);
    moSetLength(size+custom_playload);
    moSetPayloadType(P_CUSTOM);
    bMsgReady = 1;
    SendMyMessage();
    return TRUE;
}

//----------------------------------------------
// RF Scanner Messages
//----------------------------------------------
// Probe ack message
void MsgScanner_ProbeAck(uint8_t _to) {
  uint8_t payl_len = UNIQUE_ID_LEN + 1;
  build(_to, 0x00, C_INTERNAL, I_GET_NONCE_RESPONSE, 0, 1);

  // Common payload
  sndMsg.payload.data[0] = SCANNER_PROBE;
  copyBuffer(sndMsg.payload.data + 1, _uniqueID, UNIQUE_ID_LEN);
  
  sndMsg.payload.data[payl_len++] = gConfig.version;
  sndMsg.payload.data[payl_len++] = gConfig.type;
  sndMsg.payload.data[payl_len++] = gConfig.nodeID;
  sndMsg.payload.data[payl_len++] = gConfig.subID;
  sndMsg.payload.data[payl_len++] = gConfig.rfChannel;
  sndMsg.payload.data[payl_len++] = (gConfig.rfDataRate << 2) + gConfig.rfPowerLevel;
  copyBuffer(sndMsg.payload.data + payl_len, gConfig.NetworkID, sizeof(gConfig.NetworkID));
  payl_len += sizeof(gConfig.NetworkID);
  
  moSetLength(payl_len);
  moSetPayloadType(P_CUSTOM);
  bMsgReady = 1;
}

uint8_t ParseCommonProtocol(){
  uint8_t isProcessed = 0;
  
  uint8_t _version = miGetVersion();
  uint8_t _lenPayl = miGetLength();
  if(_version == 2)
  {
    if(!isIdentityEqual(rcvMsg.payload.data,_uniqueID,UNIQUE_ID_LEN))
    {
      //isProcessed = 1;
      return 1;
    }
    uint8_t realdata[MAX_PAYLOAD];
    _lenPayl = _lenPayl-UNIQUE_ID_LEN;
    copyBuffer(realdata, rcvMsg.payload.data + UNIQUE_ID_LEN, _lenPayl);
    copyBuffer(rcvMsg.payload.data, realdata, _lenPayl);
    miSetLength(_lenPayl);
  }
  uint8_t _cmd = miGetCommand();
  uint8_t _sender = rcvMsg.header.sender;  // The original sender
  uint8_t _type = rcvMsg.header.type;
  uint8_t _sensor = rcvMsg.header.sensor;
  //bool _isAck = (bool)miGetAck();
  uint8_t comMsg = 1;
  
  switch( _cmd ) {
  case C_INTERNAL:
    if( _type == I_REBOOT ) {
      isProcessed = 1;
      if( IS_MINE_SUBID(_sensor) ) {
          uint8_t lv_bColdReboot = 0;
          if(_lenPayl > 0) lv_bColdReboot = rcvMsg.payload.data[0];
          if( lv_bColdReboot ) WWDG->CR = 0x80;
          else gResetNode = TRUE;
          return 1;
      }
    }else if( _type == I_CONFIG ) {
      // Node Config
      switch( _sensor ) {
      case NCF_QUERY:
        isProcessed = 1;
        // Inform controller with version & NCF data
        Msg_NodeConfigData(_sender);
        return 1;
        break;     
      case NCF_DEV_SET_SUBID:
          isProcessed = 1;
          // [:subID][:devType]
          if(_lenPayl >= 1) {
            if( gConfig.subID != rcvMsg.payload.data[0] ) {
              gConfig.subID = rcvMsg.payload.data[0];
              gSubIDChanged = TRUE;
            }
            if(_lenPayl >= 2) {
              if( gConfig.type != rcvMsg.payload.data[1] ) {
                gConfig.type = rcvMsg.payload.data[1];
                gSubIDChanged = TRUE;
              }
            }
          }
          gResendPresentation = gSubIDChanged;          
          break;
      case NCF_DEV_MAX_NMRT:
        isProcessed = 1;
        gConfig.rptTimes = rcvMsg.payload.data[0];
        break;
      case NCF_DEV_SET_RF:
        isProcessed = 1;
        if( _lenPayl >= 1) {
           // Node base rf info Config
          uint8_t newNid = rcvMsg.payload.data[0];
          if(!isNodeIdInvalid(newNid)) {
            gConfig.nodeID = newNid;
            gResetRF = TRUE;
            gResendPresentation = TRUE;
          }
        }
        if(_lenPayl >= 2) {
          uint8_t rfchannel = rcvMsg.payload.data[1];
          if(rfchannel != 0) {
            gConfig.rfChannel = rfchannel;
            gResetRF = TRUE;
          }
        }
        if(_lenPayl >= 3) {
          uint8_t subid = rcvMsg.payload.data[2];
          if( gConfig.subID != subid ) {
            gConfig.subID = subid;
            gSubIDChanged = TRUE;
            gResendPresentation = TRUE;
          }
        }
        if(_lenPayl >= 4) {
          uint8_t netlen = _lenPayl - 3;
          uint8_t bnetvalid = 0;
          for(uint8_t i = 0;i<netlen;i++) {
            if(rcvMsg.payload.data[3+i] != 0) {
              bnetvalid = 1;
              break;
            }
          }
          if(bnetvalid) {
            clearBuffer(gConfig.NetworkID, sizeof(gConfig.NetworkID));
            for(uint8_t j = 0;j<netlen;j++) {
              gConfig.NetworkID[4-j] = rcvMsg.payload.data[3+j];
            }
            gResetRF = TRUE;
          }
        }
        break;
      default:
        comMsg = 0;
        break;
      }
      if(comMsg == 1) {
         gIsConfigChanged = TRUE;
         Msg_NodeConfigAck(_sender, _sensor);
         return 1;
      } else {
        return 0;
      }  
    } else if( _type == I_GET_NONCE ) {
        isProcessed = 1;
        // RF Scanner Probe
        if( _sender == NODEID_RF_SCANNER || _sender == NODEID_GATEWAY) {
          uint8_t _lenPayl = miGetLength();
          if( rcvMsg.payload.data[0] == SCANNER_PROBE ) {      
            MsgScanner_ProbeAck(_sender);
          } else if( rcvMsg.payload.data[0] == SCANNER_SETUP_RF ) {
            if(!IS_MINE_SUBID(_sensor)) return 0;  
            Process_SetupRF(rcvMsg.payload.data + 1,_lenPayl-1);
          } else if( rcvMsg.payload.data[0] == SCANNER_SETUPDEV_RF ) {
            if(!isIdentityEqual(rcvMsg.payload.data + 1,_uniqueID,UNIQUE_ID_LEN)) return 0;
            Process_SetupRF(rcvMsg.payload.data + 1 + UNIQUE_ID_LEN,_lenPayl-1 - UNIQUE_ID_LEN);
          } else if( rcvMsg.payload.data[0] == SCANNER_SETCONFIG ) {
            if(!IS_MINE_SUBID(_sensor)) return 0;          
            uint8_t cfg_len = _lenPayl - 2;
            Process_SetConfig(cfg_len);
          } else if( rcvMsg.payload.data[0] == SCANNER_SETDEV_CONFIG ) {  
            if(!isIdentityEqual(rcvMsg.payload.data + 2,_uniqueID,UNIQUE_ID_LEN)) return 0;
            uint8_t cfg_len = _lenPayl - 10;
            Process_SetDevConfig(cfg_len);
          } else if( rcvMsg.payload.data[0] == SCANNER_GETDEV_CONFIG ) {  
            uint8_t offset = rcvMsg.payload.data[1];
            uint8_t cfgblock_len = rcvMsg.payload.data[10];
            if(!isIdentityEqual(rcvMsg.payload.data + 2,_uniqueID,UNIQUE_ID_LEN)) return 0;
            MsgScanner_ConfigAck(offset,cfgblock_len,TRUE); 
          } else if( rcvMsg.payload.data[0] == SCANNER_GETCONFIG ) { 
            if(!IS_MINE_SUBID(_sensor)) return 0;  
            uint8_t offset = rcvMsg.payload.data[1];
            uint8_t cfgblock_len = rcvMsg.payload.data[2];
            MsgScanner_ConfigAck(offset,cfgblock_len,FALSE);
          }
          return 1;
        }      
      }
    break;   
  }
  return isProcessed;
}

//////set rf /////////////////////////////////////////////////
//typedef struct
//{
//    uint8_t subtype;
//    uint8_t uniqueid[8];
//    uint8_t channel;
//    uint8_t datarate;
//    uint8_t powerlevel;
//    uint8_t network[6];
//    uint8_t nodeid;        //unnecessary data field£¬has this field£¬need change nodeid£¬0 indicate ignore this parameter
//    uint8_t subid;         //unnecessary data field£¬has this field£¬need change subid
//}MyMsgPayload_t
//////set rf /////////////////////////////////////////////////
void Process_SetupRF(const UC *rfData,uint8_t rflen)
{
  bool bNeedChangeCfg = FALSE;
  if(rflen > 0 &&(*rfData)>=0 && (*rfData)<=127)
  {
    if(gConfig.rfChannel != (*rfData))
    {
      gConfig.rfChannel = (*rfData);
      gResetRF = TRUE;
    } 
  }
  rfData++;
  if(rflen > 1 &&(*rfData)>=RF24_1MBPS && (*rfData)<= RF24_250KBPS)
  {
    if(gConfig.rfDataRate != (*rfData))
    {
      gConfig.rfDataRate = (*rfData);
      gResetRF = TRUE;
    } 
  }
  rfData++;
  if(rflen > 2 &&(*rfData)>=RF24_PA_MIN && (*rfData)<= RF24_PA_ERROR)
  {
    if(gConfig.rfPowerLevel != (*rfData))
    {
      gConfig.rfPowerLevel = (*rfData);
      gResetRF = TRUE;
    } 
  }
  rfData++;
  bool bValidNet = FALSE;
  UC newNetwork[6] = {0};
  if(rflen > 8)
  {  
    for(uint8_t i = 0;i<6;i++)
    {
      if(*(rfData+i) != 0)
      {
        bValidNet=TRUE;
        break;
      }
    }
    if(isIdentityEqual(rfData,gConfig.NetworkID,sizeof(gConfig.NetworkID)))
    {
      bValidNet=FALSE;
    }
    else
    {
      copyBuffer(newNetwork, rfData, sizeof(newNetwork));
    }
  }
  rfData = rfData + sizeof(gConfig.NetworkID);
  bool bNeedResetNode = FALSE;
  if(rflen > 9 && (* rfData) != 0)
  {
    if(gConfig.nodeID != (* rfData))
    {
      if(!isNodeIdInvalid(*rfData))
      {
        gConfig.nodeID = (* rfData);
        bNeedResetNode = TRUE;
      }    
    }
  }
  rfData++; 
  if(rflen > 10)
  {
    if(gConfig.subID != (* rfData ))
    {
      gConfig.subID = (*rfData);
      bNeedChangeCfg = TRUE;
    }
  }
  if(bValidNet)
  {// nodeid is valid,allow change networkid
    if(!isNodeIdInvalid(gConfig.nodeID))
    {
      copyBuffer(gConfig.NetworkID, newNetwork, sizeof(gConfig.NetworkID));
      bNeedResetNode = TRUE;
    }
  }
  if(bNeedResetNode) gResetNode = TRUE;
  if(gResetNode || gResetRF || bNeedChangeCfg) {
    gIsConfigChanged = TRUE;
  }
}
//----------------------------------------------
void Msg_NodeConfigAck(uint8_t _to, uint8_t _ncf) {
  build(_to, _ncf, C_INTERNAL, I_CONFIG, 0, 1);

  sndMsg.payload.data[0] = 1;      // OK
  moSetPayloadType(P_BYTE);
  moSetLength(1);
  bMsgReady = 1;
}

// Prepare NCF query ack message
void Msg_NodeConfigData(uint8_t _to) {
  uint8_t payl_len = 0;
  build(_to, NCF_QUERY, C_INTERNAL, I_CONFIG, 0, 1);

  sndMsg.payload.data[payl_len++] = gConfig.version;
  sndMsg.payload.data[payl_len++] = gConfig.subID;
  sndMsg.payload.data[payl_len++] = gConfig.type;
  sndMsg.payload.data[payl_len++] = gConfig.senMap % 256;
  sndMsg.payload.data[payl_len++] = gConfig.senMap / 256;
  sndMsg.payload.data[payl_len++] = gConfig.rptTimes;
  sndMsg.payload.data[payl_len++] = 0;     // Reservered
  sndMsg.payload.data[payl_len++] = 0;     // Reservered
  sndMsg.payload.data[payl_len++] = 0;     // Reservered
  sndMsg.payload.data[payl_len++] = 0;     // Reservered
  sndMsg.payload.data[payl_len++] = 0;     // Reservered
  sndMsg.payload.data[payl_len++] = 0;     // Reservered
  
  moSetLength(payl_len);
  moSetPayloadType(P_CUSTOM);
  bMsgReady = 1;
}

//typedef struct
//{
//    uint8_t subtype;
//    uint8_t offset;
//    uint8_t uniqueid[8];
//    UC ConfigBlock[15];
//}MyMsgPayload_t
#define CFGBLOCK_SIZE    15
#define CFGBLOCK_NO_UNIQUEID_SIZE CFGBLOCK_SIZE+UNIQUE_ID_LEN
void MsgScanner_ConfigAck(uint8_t offset,uint8_t cfglen,bool _isByUniqueid) {
  uint8_t cfg_end_offset = cfglen;
  if(cfglen == 0) cfg_end_offset = sizeof(Config_t)-1;
  else
  {
    cfg_end_offset = offset + cfglen > sizeof(Config_t)-1?sizeof(Config_t)-1:offset + cfglen;
  }  
  while( offset < cfg_end_offset )
  {
    uint8_t left_len = cfg_end_offset - offset;
    uint8_t payl_len = left_len < CFGBLOCK_SIZE ? left_len : CFGBLOCK_SIZE;
    if(_isByUniqueid) AddCfgOutputBuf(offset,payl_len,1);
    else  
    {
      payl_len = left_len < CFGBLOCK_NO_UNIQUEID_SIZE ? left_len : CFGBLOCK_NO_UNIQUEID_SIZE;
      AddCfgOutputBuf(offset,payl_len,0);
    }
    offset+=payl_len;
    offset %= sizeof(Config_t);
  }
}

//////set config by nodeid&subid data struct/////////////////////
//typedef struct
//{
//    uint8_t subtype;
//    uint8_t offset;  //config offset
//    UC ConfigBlock[23];
//}MyMsgPayload_t
//////set config by nodeid&subid data struct/////////////////////
void Process_SetConfig(u8 _len) {
  uint8_t offset = rcvMsg.payload.data[1];
  copyBuffer((UC *)((uint16_t)(&gConfig) + offset), rcvMsg.payload.data+2, _len);
  gIsConfigChanged = TRUE;
}
//////set config by uniqueid data struct/////////////////////
//typedef struct
//{
//    uint8_t subtype;
//    uint8_t offset;   //config offset
//    uint8_t uniqueid[8];
//    
//    UC ConfigBlock[15];
//}MyMsgPayload_t
//////set config by uniqueid data struct/////////////////////
void Process_SetDevConfig(u8 _len) {
    uint8_t offset = rcvMsg.payload.data[1];
    copyBuffer((UC *)((uint16_t)(&gConfig) + offset), rcvMsg.payload.data + 2 + UNIQUE_ID_LEN, _len);
    gIsConfigChanged = TRUE;
}
