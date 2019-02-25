#ifndef _RELAYPUBLICDEFINE_H
#define _RELAYPUBLICDEFINE_H

#include "publicDefine.h"

// Switch value for set power command
#define DEVICE_SW_OFF               0       // Turn Off
#define DEVICE_SW_ON                1       // Turn On
#define DEVICE_SW_TOGGLE            2       // Toggle
#define DEVICE_SW_DUMMY             3       // Detail followed

// Key operation style
#define KEY_OP_STYLE_PRESS              'p'      // Normal press, 200ms
#define KEY_OP_STYLE_FAST_PRESS         'f'      // Fast press, 100ms
#define KEY_OP_STYLE_LONG_PRESS         'l'      // Long-press, 500ms
#define KEY_OP_STYLE_VLONG_PRESS        'v'      // Very long press, 2550ms
#define KEY_OP_STYLE_HOLD               'h'      // Hold
#define KEY_OP_STYLE_RELEASE            'r'      // Release
#define KEY_OP_STYLE_DBL_CLICK          'd'      // Double-click

// Key interval delimitor
#define KEY_DELI_NO_PAUSE               '+'      // No delay
#define KEY_DELI_SMALL_PAUSE            ','      // Small pause, 200ms
#define KEY_DELI_NORMAL_PAUSE           '.'      // Normal pause, 600ms
#define KEY_DELI_LONG_PAUSE             '='      // Long pause, 2s
#define KEY_DELI_VLONG_PAUSE            '~'      // Very long pause, 5s
#define KEY_DELI_SAME_TIME              '_'      // Press two keys at the same time

// Panel Button Operation(bit:7,6,5), Object(bit:4,3,2) & Action(bit:1,0)
#define BTN_OP_SHORT_PRESS              0       // Short press
#define BTN_OP_DBL_PRESS                1       // Double press
#define BTN_OP_LONG_HOLD                2       // Long hold
#define BTN_OP_LONG_RELEASE             3       // Long hold-release
#define BTN_OP_VLONG_HOLD               4       // Very long hold
#define BTN_OP_VLONG_RELEASE            5       // Very long hold-release

#define BTN_OBJ_DEFAULT_KEY             0       // corresponding relay key
#define BTN_OBJ_SCAN_KEY_MAP            1       // scan key map and act on keys one by one
#define BTN_OBJ_LOOP_KEY_MAP            2       // get one key from key map, act on it, and move to the next key

#define BTN_ACT_TOGGLE                  0       // toggle relay key
#define BTN_ACT_ON                      1       // turn on relay key
#define BTN_ACT_OFF                     2       // turn off relay key

#endif /* _RELAYPUBLICDEFINE_H */
