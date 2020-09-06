#include "ledRGB.h"
#include "publicDefine.h"           // Need System State Define

#define TIM1_PERIOD                 1999
#define TIM1_REPTETION_COUNTER      0

#define TIM3_PERIOD                 TIM1_PERIOD

#define LED_BR                      90   // LED br precent

#define FAST_DELAY_TIMER            15   // 150ms
#define FAST_BR_TIMER               2    // 20ms
#define NOL_DELAY_TIMER             5    // 50ms
#define BLINK_DELAY_TIMER           30   // 300ms
#define BR_CHANGE_VALUE             2    // change 2 br percent every time

const uint8_t Color_Tab[CYAN+1][3]=
{
	{255,255,255},
	{255,0,0},
	{255,255,0},
	{0,0,255},
	{0,255,0},
	{255,0,255},
	{0,255,255}
};

// RGB-LED-FSM
const FsmST2LED_t gFsm2LED_tab[SYS_ST_RESET + 1]=
{
	{	SYS_ST_INIT,	    GREEN,		T_CONSTANT_LIGHT},
	{	SYS_ST_RUNNING,	    CYAN,		T_CONSTANT_LIGHT},
	{	SYS_ST_ON_BATTERY,	CYAN,		T_BREATH},
	{	SYS_ST_LOW_BATTERY,	YELLOW,		T_BREATH},
	{	SYS_ST_UPDATE,	    MAGENTA,	T_BLINK},
	{	SYS_ST_ERROR,	    RED,	    T_BLINK},
	{	SYS_ST_SETUP,	    BLUE,	    T_CONSTANT_LIGHT},
	{	SYS_ST_SLEEP,	    WHITE,		T_OFF},
	{	SYS_ST_RESET,       RED,		T_CONSTANT_LIGHT}
};

uint8_t timer_tick = 0;       // tick, 10ms interval
uint8_t targetBr = 0;
uint8_t curBr = LED_BR;
eLightType lightType = T_CONSTANT_LIGHT;
COLOR curColor = CYAN;
uint8_t isTwinkleStart = 0;
uint8_t isOnIdleState = 1;

uint8_t led_GetIsOnIdleState()
{
	return isOnIdleState;
}

void led_SetIsOnIdleState(const uint8_t _on)
{
	if( _on != isOnIdleState ) isOnIdleState = _on;
}

void getRGB(const COLOR color, uint8_t *R, uint8_t *G, uint8_t *B)
{
	*R = Color_Tab[color][0];
	*G = Color_Tab[color][1];
	*B = Color_Tab[color][2];
}

void setLEDBR(const COLOR color, const uint8_t BRPercent)
{
	// color change to r,g,b
	uint8_t RValue = 0;
	uint8_t GValue = 0;
	uint8_t BValue = 0;
	uint8_t pluse = 0;
	uint32_t usPW_Scale = 0;
	uint16_t usPW_R = 0;
	uint16_t usPW_G = 0;
	uint16_t usPW_B = 0;
	
	getRGB(color, &RValue, &GValue, &BValue);
	// Percentage to Pulse Width
	pluse = (TIM1_PERIOD + 1) * (100 - LED_BR) / 100;
	usPW_Scale = BRPercent * pluse / 100;
	usPW_R = RValue * usPW_Scale / 255;
	usPW_G = GValue * usPW_Scale / 255;
	usPW_B = BValue * usPW_Scale / 255;
	
	TIM1_SetCompare1(usPW_R);
	TIM3_SetCompare2(usPW_G);
	TIM1_SetCompare2(usPW_B);
}

void ledRGB_blink(const COLOR color, const eLightType type)
{
	isTwinkleStart = 1;
	curColor = color;
	lightType = type;
	// blink, 20ms on<->off
	timer_tick = FAST_DELAY_TIMER;
	setLEDBR(curColor, LED_BR);
}

void ledRGB_normal(const COLOR color)
{
	isTwinkleStart = 0;
	curColor = color;
	lightType = T_CONSTANT_LIGHT;
	setLEDBR(curColor, LED_BR);
}

void ledRGB_off()
{
	isTwinkleStart = 0;
	lightType = T_OFF;
	setLEDBR(curColor, 0);
}

void ledRGB_stopTwinkle()
{
	isTwinkleStart = 0;
}

void ledRGB_twinkle(const uint8_t init, const uint8_t fast, const COLOR color)
{
	curColor = color;
	if(init) {
		isTwinkleStart = 1;
		targetBr = 0;
		curBr = LED_BR;
	} else {
		targetBr = LED_BR - curBr;
	}
	if(fast) {
		lightType = T_FAST_BREATH;
		timer_tick = FAST_BR_TIMER;
	} else {
		lightType = T_BREATH;
		timer_tick = NOL_DELAY_TIMER;
	}
}

// Change RGB-LED to desire color and display type
/// Examples:
/// ledRGB_display(CYAN, T_CONSTANT_LIGHT);
/// ledRGB_display(BLUE, T_BREATH);
/// ledRGB_display(MAGENTA, T_BLINK);
void ledRGB_display(const COLOR color, const eLightType type)
{
	if(type == T_CONSTANT_LIGHT) {
		ledRGB_normal(color);
	} else if(type == T_BLINK || type == T_FAST_BLINK ) {
		ledRGB_blink(color, type);
	}
	else if(type == T_OFF) {
		ledRGB_off();
	} else {
		ledRGB_twinkle(TRUE, type == T_FAST_BREATH, color);
	}
}

uint8_t isTargetBrCompleted() {
	uint8_t bFinished = 0;
	if( targetBr > curBr ) {
		// Up
		curBr += BR_CHANGE_VALUE;
		if(curBr >= targetBr ) {
			curBr = targetBr;
			bFinished = 1;
		}
	} else {
		// Down
		if(curBr <= BR_CHANGE_VALUE) {
			curBr = 0;
			bFinished = 1;
		} else {
			curBr -= BR_CHANGE_VALUE;
		}
	}
	setLEDBR(curColor, curBr);
	return bFinished;
}

// Refresh RGB-LED according to the LED-FSM
// 1. Query LED-FSM to retrieve corresponding LED color and display type according to current state
// 2. Call ledRGB_display() to change LED
void ledRGB_changed(const uint8_t eState)
{
    // Search LED-FSM
    eLightType lv_lightType = gFsm2LED_tab[eState].type;
    if( eState == SYS_ST_ON_BATTERY ) {
        if( !isOnIdleState ) lv_lightType = T_OFF;
    }
    ledRGB_display(gFsm2LED_tab[eState].color, lv_lightType);
}

// This function should be called in a timer of 10-ms period to display dynamaic effect, like BREATH and BLINK
// If we only need constant display, no need to call this function.
void lightChangeProcess()
{
	if(isTwinkleStart) {
        if(timer_tick == 0) {
            if(lightType == T_BLINK || lightType == T_FAST_BLINK) {
				timer_tick = (lightType == T_BLINK ? BLINK_DELAY_TIMER : FAST_DELAY_TIMER);
				if(curBr > 0) {
					setLEDBR(curColor, 0);
					curBr = 0;
				} else {
					setLEDBR(curColor, LED_BR);
					curBr = LED_BR;
				}
			} else {
				// change br
				timer_tick = NOL_DELAY_TIMER;
				if(isTargetBrCompleted()) {
					ledRGB_twinkle(0, lightType == T_FAST_BREATH, curColor);
				}	
			}	
		} else {
			timer_tick--;
		}
	}	
	// Take care of power indicator
	//Power_LED_Blink();
}

static void TIMLightPwm_ClkGpioConfig(void)
{
  /* Enable TIM1 clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM1, ENABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM3, ENABLE);
  GPIO_Init(GPIOD, GPIO_Pin_4 | GPIO_Pin_2  , GPIO_Mode_Out_PP_High_Fast);
  GPIO_Init(GPIOD, GPIO_Pin_0 , GPIO_Mode_Out_PP_High_Fast);
}

static void TIMPWMFunction_Config(void)
{
  TIM1_DeInit();
  TIM3_DeInit();
  /* Time base configuration */
  TIM1_TimeBaseInit(7, TIM1_CounterMode_Up, TIM1_PERIOD, TIM1_REPTETION_COUNTER);
  //Õ¼¿Õ±È tim_plus/(peirod+1)
  TIM1_OC1Init(TIM1_OCMode_PWM1, TIM1_OutputState_Enable, TIM1_OutputNState_Disable,
               20, TIM1_OCPolarity_Low, TIM1_OCNPolarity_Low, TIM1_OCIdleState_Set,
               TIM1_OCNIdleState_Set);
  TIM1_OC1PreloadConfig(ENABLE);

  TIM1_OC2Init(TIM1_OCMode_PWM1, TIM1_OutputState_Enable, TIM1_OutputNState_Disable,
               20, TIM1_OCPolarity_Low, TIM1_OCNPolarity_Low, TIM1_OCIdleState_Set,
               TIM1_OCNIdleState_Set);
  TIM1_OC2PreloadConfig(ENABLE);

  TIM1_ARRPreloadConfig(ENABLE);
  
  TIM3_TimeBaseInit(TIM3_Prescaler_8, TIM3_CounterMode_Up,TIM3_PERIOD);
  TIM3_OC2Init(TIM3_OCMode_PWM1, TIM3_OutputState_Enable, 200, TIM3_OCPolarity_Low,TIM3_OCIdleState_Set);
  TIM3_OC2PreloadConfig(ENABLE);
  TIM3_ARRPreloadConfig(ENABLE);

  /* TIM1 Interrupt enable */ // must delete this code
  //TIM1_ITConfig(TIM1_IT_CC1, ENABLE);
  //TIM1_ITConfig(TIM1_IT_CC2, ENABLE);

  /* Enable TIM1 outputs */
  TIM1_CtrlPWMOutputs(ENABLE);
  TIM3_CtrlPWMOutputs(ENABLE);
  /* TIM1 enable counter */
  TIM1_Cmd(ENABLE);
  TIM3_Cmd(ENABLE);
}

void initTimPWMFunction (void)
{
  TIMLightPwm_ClkGpioConfig ();
  TIMPWMFunction_Config ();
}
