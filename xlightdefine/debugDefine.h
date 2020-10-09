#ifndef _DEBUGDEFINE_H
#define _DEBUGDEFINE_H

// Uncomment this line if in debug mode
//#define DEBUG_NO_WWDG
//#define DEBUG_LOG


//#define TEST
#ifdef TEST
#define     PB5_Low                GPIO_WriteLow(GPIOB , GPIO_PIN_5)
#define     PB4_Low                GPIO_WriteLow(GPIOB , GPIO_PIN_4)
#define     PB3_Low                GPIO_WriteLow(GPIOB , GPIO_PIN_3)
#define     PB2_Low                GPIO_WriteLow(GPIOB , GPIO_PIN_2)
#define     PB1_Low                GPIO_WriteLow(GPIOB , GPIO_PIN_1)
#define     PB0_Low                GPIO_WriteLow(GPIOB , GPIO_PIN_0)
#define     PD1_Low                GPIO_WriteLow(GPIOD , GPIO_PIN_1)
#define     PD2_Low                GPIO_WriteLow(GPIOD , GPIO_PIN_2)
#define     PD7_Low                GPIO_WriteLow(GPIOD , GPIO_PIN_7)
#define     PC1_Low                GPIO_WriteLow(GPIOC , GPIO_PIN_1)
#define     PB5_High                GPIO_WriteHigh(GPIOB , GPIO_PIN_5)
#define     PB4_High                GPIO_WriteHigh(GPIOB , GPIO_PIN_4)
#define     PB3_High                GPIO_WriteHigh(GPIOB , GPIO_PIN_3)
#define     PB2_High                GPIO_WriteHigh(GPIOB , GPIO_PIN_2)
#define     PB1_High                GPIO_WriteHigh(GPIOB , GPIO_PIN_1)
#define     PB0_High                GPIO_WriteHigh(GPIOB , GPIO_PIN_0)
#define     PD1_High                GPIO_WriteHigh(GPIOD , GPIO_PIN_1)
#define     PD2_High                GPIO_WriteHigh(GPIOD , GPIO_PIN_2)
#define     PD7_High                GPIO_WriteHigh(GPIOD , GPIO_PIN_7)
#define     PC1_High                GPIO_WriteHigh(GPIOC , GPIO_PIN_1)
#endif

#endif /* _DEBUGDEFINE_H */
