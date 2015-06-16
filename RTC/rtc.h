#ifndef __RTC
#define __RTC

#include "inc/wh160x_driver.h"
#include "stm_lib/inc/stm32f10x_rtc.h"

// ��������� ��� �������� ���������� �������
typedef struct
{
  uint8_t hour;   //����
  uint8_t min;	  //������
  uint8_t sec;	  //�������
} RTC_Time;

//��������� ��� ������ � ����� "�����"
RTC_Time TimeNow;

uint32_t tmp;

uint8_t ButtonTime;   // ����� ������� �� ������
uint32_t TempTime;    // ���������� ��� ������������� �������

unsigned char RtcInit (void);
void RTC_IRQHandler(void);
void SysTick_Handler(void);
void RtcToTime(uint32_t counter, RTC_Time *time);
void DisplayTime(RTC_Time *TimeNow);

#endif /* __RTC */
