#ifndef TIMER_LED_H
#define TIMER_LED_H

#include <string.h>
#include "log.h"
#include "simenv.h"

/* 显示的数字 */
int led_data;
const int LED_MAX_DATE;

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
void led_set(const int data);

/* 加1 */
void led_incr();

/* 初始化 */
void led_init();

#endif //TIMER_LED_H
