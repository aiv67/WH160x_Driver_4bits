#ifndef __RTC
#define __RTC

#include "inc/wh160x_driver.h"
#include "stm_lib/inc/stm32f10x_rtc.h"

// Структура для хранения параметров времени
typedef struct
{
  uint8_t hour;   //часы
  uint8_t min;	  //минуты
  uint8_t sec;	  //секунды
} RTC_Time;

//Структура для работы с типом "время"
RTC_Time TimeNow;

uint32_t tmp;

uint8_t ButtonTime;   // Время нажатия на кнопку
uint32_t TempTime;    // Переменная для корректировки времени

unsigned char RtcInit (void);
void RTC_IRQHandler(void);
void SysTick_Handler(void);
void RtcToTime(uint32_t counter, RTC_Time *time);
void DisplayTime(RTC_Time *TimeNow);

#endif /* __RTC */
