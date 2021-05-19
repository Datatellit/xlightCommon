#ifndef _COMMON_H
#define _COMMON_H

#include "publicDefine.h"
#include "xliSensorMap.h"

// Return NodeType by giving NodeID
UC NodeID2Type(const UC _nid);

void clearBuffer(UC *_buf, const US _len);
void copyBuffer(UC *_toBuf, const UC *_fromBuf, const US _len);

bool isIdentityEmpty(const UC *pId, const UC nLen);
bool isIdentityEqual(const UC *pId1, const UC *pId2, const UC nLen);

#endif /* _COMMON_H */