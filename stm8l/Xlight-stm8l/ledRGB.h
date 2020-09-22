#ifndef __LED_RBG_DISPLAY_H
#define __LED_RBG_DISPLAY_H

#include "stm8l15x.h"

typedef enum 
{
	T_CONSTANT_LIGHT,
	T_BREATH,
	T_FAST_BLINK,
	T_FAST_BREATH,
	T_BLINK,
	T_OFF
} eLightType;

typedef enum
{
	WHITE,
	RED,
	YELLOW,
	BLUE,
	GREEN,
	MAGENTA,
	CYAN
} COLOR;

typedef struct FsmST2LED_s
{
	uint8_t state;
	COLOR color;
	eLightType type; 
} FsmST2LED_t;

void initTimPWMFunction(const uint8_t _reversed);
uint8_t led_GetIsOnIdleState(void);
void led_SetIsOnIdleState(const uint8_t _on);

// Change RGB-LED to desire color and display type
/// Examples:
/// ledRGB_display(CYAN, T_CONSTANT_LIGHT);
/// ledRGB_display(BLUE, T_BREATH);
/// ledRGB_display(MAGENTA, T_BLINK);
void ledRGB_display(const COLOR color, const eLightType type);

// Refresh RGB-LED according to the LED-FSM
void ledRGB_changed(const uint8_t eState);

// This function should be called in a timer of 10-ms period to display dynamaic effect, like BREATH and BLINK
// If we only need constant display, no need to call this function.
void lightChangeProcess(void);

#endif // __LED_RBG_DISPLAY_H