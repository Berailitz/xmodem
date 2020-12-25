#include "led.h"

/* 显示的数字 */
int led_data = 0;
const int LED_MAX_DATE = 15;

/* 是否为模拟器 */
#if !defined(__MINGW32__) && defined(physical)
#define physical_led
#endif

/* 定义端口 */
#ifdef physical_led
#define rGPBCON    (*(volatile unsigned *)\
          0x56000010) /*Port B control*/
#define rGPBDAT    (*(volatile unsigned *)\
          0x56000014) /*Port B data*/
#define rGPBUP     (*(volatile unsigned *)\
          0x56000018) /*Pull-up control B*/
#endif

/* 写 */
void led_set(const int data) {
    pinfo("data=%d", data);
    led_data = data;

#ifdef physical_led
    rGPBDAT = (rGPBDAT & ~(0xf<<5)) | ((~data & 0xf)<<5);
#endif
}

/* 加1 */
void led_incr() {
    pinfo("");
    led_set((led_data + 1) % (LED_MAX_DATE + 1));
}

/* 初始化 */
void led_init() {
    pinfo("");
    led_set(0);

#ifdef physical_led
    rGPBCON	= (rGPBCON | (0xf<<5));
    rGPBUP 	= (rGPBUP & ~(0xf<<5));
    rGPBDAT 	= (rGPBDAT | (0xf<<5));
#endif
}
