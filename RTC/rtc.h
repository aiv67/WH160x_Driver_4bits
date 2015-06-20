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
RTC_Time timeNow;

uint32_t tmp;

volatile uint16_t buttonTime;   	// ����� ������� �� ������
volatile uint16_t buttonTimeLimit; 	//

unsigned char RtcInit (void);
void RTC_IRQHandler(void);
void SysTick_Handler(void);
void RtcToTime(uint32_t counter, RTC_Time *time);
void DisplayTime(RTC_Time *TimeNow);

#endif /* __RTC */
