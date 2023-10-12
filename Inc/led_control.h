#ifndef __LED_CONTROL_H
#define __LED_CONTROL_H

#include "lp50xx.h"

#define __OUT_ON(out)			LP50XX_SetOutColor(out, 255)
#define __OUT_OFF(out)			LP50XX_SetOutColor(out, 0)

#define __LED_USERBTN_OUTS__	LP50XX_OUT0,LP50XX_OUT1,LP50XX_OUT2
#define __LED_STATUS_OUTS__		LP50XX_OUT3,LP50XX_OUT4,LP50XX_OUT5

#define __COLOR_BLACK__			0,0,0
#define __COLOR_RED__			255,0,0
#define __COLOR_GREEN__			0,255,0
#define __COLOR_BLUE__			0,0,255
#define __COLOR_YELLOW__		255,255,0

#define LED_USERBTN_ON(color)	__setThreeOutColors(__LED_USERBTN_OUTS__, color)
#define LED_USERBTN_OFF()		__setThreeOutColors(__LED_USERBTN_OUTS__, __COLOR_BLACK__)

#define LED_STATUS_ON(color)	__setThreeOutColors(__LED_STATUS_OUTS__, color)
#define LED_STATUS_OFF()		__setThreeOutColors(__LED_STATUS_OUTS__, __COLOR_BLACK__)

#define LED_PWROK_ON()			__OUT_ON(LP50XX_OUT6)
#define LED_PWROK_OFF()			__OUT_OFF(LP50XX_OUT6)

#define LED_BOOT_SERIAL_ON()	__OUT_ON(LP50XX_OUT7)
#define LED_BOOT_SERIAL_OFF()	__OUT_OFF(LP50XX_OUT7)

#define LED_BOOT_FUSE_ON()		__OUT_ON(LP50XX_OUT8)
#define LED_BOOT_FUSE_OFF()		__OUT_OFF(LP50XX_OUT8)

#define LED_BOOT_DEV_ON()		__OUT_ON(LP50XX_OUT9)
#define LED_BOOT_DEV_OFF()		__OUT_OFF(LP50XX_OUT9)

#define LED_BOARD_RED_ON()		__OUT_ON(LP50XX_OUT10)
#define LED_BOARD_RED_OFF()		__OUT_OFF(LP50XX_OUT10)

#define LED_BOARD_GREEN_ON()	__OUT_ON(LP50XX_OUT11)
#define LED_BOARD_GREEN_OFF()	__OUT_OFF(LP50XX_OUT11)

static inline void __setThreeOutColors(uint8_t out1, uint8_t out2, uint8_t out3,
									   uint8_t color1, uint8_t color2, uint8_t color3)
{
	LP50XX_SetOutColor(out1, color1);
	LP50XX_SetOutColor(out2, color2);
	LP50XX_SetOutColor(out3, color3);
}

#endif
