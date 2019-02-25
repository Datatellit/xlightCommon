#include <stm8s.h>
#include "wwdg.h"
#include "debugDefine.h"

// Window Watchdog
#define WWDG_COUNTER                    0x7f
#define WWDG_WINDOW                     0x77

uint8_t feedingDog = 0;
// Initialize Window Watchdog
void wwdg_init() {
#ifndef DEBUG_NO_WWDG  
  WWDG_Init(WWDG_COUNTER, WWDG_WINDOW);
#endif  
}

// Feed the Window Watchdog
void feed_wwdg(void) {
#ifndef DEBUG_NO_WWDG  
  if(feedingDog == 1)
  {
    return;
  }
  else
  {
    feedingDog = 1;
    uint8_t cntValue = WWDG_GetCounter() & WWDG_COUNTER;
    if( cntValue < WWDG_WINDOW ) {
      WWDG_SetCounter(WWDG_COUNTER);
    }
    feedingDog = 0;
  }
#endif  
}