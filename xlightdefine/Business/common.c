#include "common.h"

// Return NodeType by giving NodeID
UC NodeID2Type(const UC _nid) {
    eNodeType_t lv_ntype;
    if( _nid == NODEID_GATEWAY ) lv_ntype = NODE_TYP_GW;
    else if( IS_LAMP_NODEID(_nid) ) lv_ntype = NODE_TYP_LAMP;
    else if( IS_COLORBAR_NODEID(_nid) ) lv_ntype = NODE_TYP_COLORBAR;
    else if( IS_SWITCH_NODEID(_nid) ) lv_ntype = NODE_TYP_SWITCH;
    else if( IS_REMOTE_NODEID(_nid) ) lv_ntype = NODE_TYP_REMOTE;
    else if( IS_FAN_NODEID(_nid) ) lv_ntype = NODE_TYP_FAN;
    else if( IS_AC_NODEID(_nid) ) lv_ntype = NODE_TYP_AC;
    else if( IS_IR_NODEID(_nid) ) lv_ntype = NODE_TYP_IR;
    else if( IS_SENSOR_NODEID(_nid) ) lv_ntype = NODE_TYP_SENSOR;
    else if( IS_AIRPURE_NODEID(_nid) ) lv_ntype = NODE_TYP_AIRPURE;
    else if( IS_CURTAIN_NODEID(_nid) ) lv_ntype = NODE_TYP_CURTAIN;
    else if( IS_MODBUS_NODEID(_nid) ) lv_ntype = NODE_TYP_MODBUS;
    else if( IS_SPECIAL_NODEID(_nid) ) lv_ntype = NODE_TYP_SPECIAL;
    else if( IS_GROUP_NODEID(_nid) ) lv_ntype = NODE_TYP_GROUP;
    else if( IS_SYSTEM_NODEID(_nid) ) lv_ntype = NODE_TYP_SYSTEM;
    else lv_ntype = NODE_TYP_THIRDPARTY;
    return (UC)lv_ntype;
}

// Clear buffer, to replace memset() so we don't have to include <sting.h>
void clearBuffer(UC *_buf, const US _len) {
  for( US i = 0; i < _len; i++ ) _buf[i] = 0x00;
}

// Copy buffer, to replace memcpy() so we don't have to include <sting.h>
void copyBuffer(UC *_toBuf, const UC *_fromBuf, const US _len) {
  for( US i = 0; i < _len; i++ ) _toBuf[i] = _fromBuf[i];
}

bool isIdentityEmpty(const UC *pId, const UC nLen)
{
  for( UC i = 0; i < nLen; i++ ) { if(pId[i] > 0) return FALSE; }
  return TRUE;
}

bool isIdentityEqual(const UC *pId1, const UC *pId2, const UC nLen)
{
  for( UC i = 0; i < nLen; i++ ) { if(pId1[i] != pId2[i]) return FALSE; }
  return TRUE;
}
