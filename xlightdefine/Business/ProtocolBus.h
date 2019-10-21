#ifndef _PROTOCOLBUS_DEV_H_
#define _PROTOCOLBUS_DEV_H_

void build(uint8_t _destination, uint8_t _sensor, uint8_t _command, uint8_t _type, bool _enableAck, bool _isAck);

void MsgScanner_ProbeAck(uint8_t _to);
uint8_t ParseCommonProtocol();
void Msg_NodeConfigAck(uint8_t _to, uint8_t _ncf);
void Msg_NodeConfigData(uint8_t _to);

void MsgScanner_ConfigAck(uint8_t offset,uint8_t cfglen,bool _isByUniqueid);
void Process_SetConfig(u8 _len);
void Process_SetDevConfig(u8 _len);
void Process_SetupRF(const UC *rfData,uint8_t rflen);

bool SendCfgBlock(uint8_t offset,uint8_t size,uint8_t isNeedUniqueid);

bool ProcessOutputCfgMsg();

extern uint8_t bMsgReady;
#endif