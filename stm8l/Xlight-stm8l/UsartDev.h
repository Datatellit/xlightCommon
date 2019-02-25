#ifndef _UART2_DEV_H_
#define _UART2_DEV_H_
#include "common.h"
#include "debugDefine.h"
uint8_t UsartSendByte(uint8_t data);
void usart_config(uint32_t speed);
uint8_t UsartSendString(uint8_t *pBuf);
void UsartSendByteByLen( uint8_t* TxBuffer, uint8_t Length );

void printlog(uint8_t *pBuf);
void printnum(unsigned int num);

#endif