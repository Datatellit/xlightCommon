#ifndef _LIGHTPUBLICDEFINE_H
#define _LIGHTPUBLICDEFINE_H

#include "publicDefine.h"

#ifndef DEBUG_NO_WWDG
// light can't reset automatically
#define DEBUG_NO_WWDG
#endif

// Switch value for set power command
#define DEVICE_SW_OFF               0       // Turn Off
#define DEVICE_SW_ON                1       // Turn On
#define DEVICE_SW_TOGGLE            2       // Toggle
#define DEVICE_SW_DUMMY             3       // Detail followed

// Update operator for set brightness & CCT command
#define OPERATOR_SET                0
#define OPERATOR_ADD                1
#define OPERATOR_SUB                2
#define OPERATOR_MUL                3
#define OPERATOR_DIV                4

// Filter (special effect)
#define FILTER_SP_EF_NONE           0
#define FILTER_SP_EF_BREATH         1       // Normal breathing light
#define FILTER_SP_EF_FAST_BREATH    2       // Fast breathing light
#define FILTER_SP_EF_FLORID         3       // Randomly altering color
#define FILTER_SP_EF_FAST_FLORID    4       // Fast randomly altering color
#define FILTER_SP_EF_GRADUAL_CHANGE 5      // gradual change from x to y by time

// Delayed operation timers
#define DELAY_TIMERS            8
#define DELAY_TIM_ONOFF         0
#define DELAY_TIM_BR            1
#define DELAY_TIM_CCT           2
#define DELAY_TIM_RGB           3
#define DELAY_TIM_MSG           7

#define BR_MIN_VALUE            5
#define CT_MIN_VALUE            2700
#define CT_MAX_VALUE            6500
#define CT_SCOPE                38    
#define CT_STEP                 ((CT_MAX_VALUE-CT_MIN_VALUE)/10)
#define LIGHT_PWM_THRESHOLD     5

// Comment off the line to disable gradual brightness on or off
#define GRADUAL_ONOFF
#define GRADUAL_CCT
//#define GRADUAL_RGB
#define DEFAULT_BRIGHTNESS      65
#define BRIGHTNESS_STEP         1
#define MAX_STEP_TIMES          62
#define MAX_FASTSTEP_TIMES      20
#define CCT_STEP                50
#define RGB_STEP                3

// WATT regulation method
/// Option 0: no restriction
/// Option 1: percentage
/// Option 2: percentage + linear
/// Option 3: percentage + quadratic
/// Option 4: percentage + cubic function
/// Option 10: percentage + table
#define WATT_RM_NO_RESTRICTION          0
#define WATT_RM_PERCENTAGE              1
#define WATT_RM_LINEAR_PERCENTAGE       2
#define WATT_RM_QUADRATIC_PERCENTAGE    3
#define WATT_RM_CUBIC_PERCENTAGE        4
#define WATT_RM_TABLE_PERCENTAGE        10

#endif /* _LIGHTPUBLICDEFINE_H */
