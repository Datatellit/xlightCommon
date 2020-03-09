#include "UsartDev.h"

void usart_config(uint32_t speed)
{
  CLK_PeripheralClockConfig (CLK_Peripheral_USART1,ENABLE);     // 开启USART1时钟
  USART_DeInit(USART1);         // 复位USART1
  USART_Init(USART1,speed,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No,USART_Mode_Tx | USART_Mode_Rx);  // USART初始化，波特率9600，8位数据长度，1位停止位，无校验位，发送模式
  USART_ClearITPendingBit(USART1, USART_IT_RXNE);       // 清除串口中断标志位
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);        // 开启接收中断
  //ITC_SetSoftwarePriority(USART1_RX_IRQn, ITC_PriorityLevel_2); // 设置优先级
  USART_Cmd(USART1, ENABLE);    // 使能USART1
}

void UsartEnableRxTx(bool xRxEnable, bool xTxEnable)
{
  /* If xRXEnable enable serial receive interrupts. If xTxENable enable
   * transmitter empty interrupts.
  */
  if(xRxEnable == TRUE)
  {
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  }
  else
  {
    USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
  }
  if(xTxEnable == TRUE)
  {
    USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
  }
  else
  {
    USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
  }
}
  
uint8_t UsartGetByte(uint8_t *pByte)
{
  *pByte = USART_ReceiveData8(USART1);
  return 1;
}

// Transimit a byte without waiting completion
void UsartPutByte(uint8_t data)
{
  USART_SendData8(USART1,(u8)data);//发送8位数据
}

uint8_t UsartSendByte(uint8_t data)
{
  USART_SendData8(USART1,(u8)data);//发送8位数据
  while(!USART_GetFlagStatus (USART1,USART_FLAG_TXE));//等待发送完毕
  return data;
}

void UsartSendByteByLen( uint8_t* TxBuffer, uint8_t Length )
{
  	while( Length-- )
	{
		while( RESET == USART_GetFlagStatus(USART1, USART_FLAG_TXE ));
		USART_SendData8(USART1, * TxBuffer );
		TxBuffer++;
	}
}

uint8_t UsartSendString(uint8_t *pBuf)
{
  unsigned char ucPos;
  if (!pBuf)
    return 0;
  
  ucPos = 0;
  do {
    if (pBuf[ucPos] != '\0') {
      UsartSendByte(pBuf[ucPos ++]);
    } else 
      break;
  } while (1);
  
  return 1;
}

void printlog(uint8_t *pBuf)
{
#ifdef DEBUG_LOG
  UsartSendString(pBuf);
#endif
}

void itoa(unsigned int n, char * buf)
{
        int i;
        
        if(n < 10)
        {
                buf[0] = n + '0';
                buf[1] = '\0';
                return;
        }
        itoa(n / 10, buf);

        for(i=0; buf[i]!='\0'; i++);
        
        buf[i] = (n % 10) + '0';
        
        buf[i+1] = '\0';
}

void printnum(unsigned int num)
{
#ifdef DEBUG_LOG
  char buf[10] = {0};
  itoa(num,buf);
  printlog(buf);
#endif
}
