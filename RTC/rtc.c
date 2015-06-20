#include "rtc.h"

//****************************************************************************//
// ������� ������������� RTC.                                                 //
// ���������:                                                                 //
//   ���.                                                                     //
// ��������� unsigned char:                                                   //
//   "1" - ������������� ���������;                                           //
//   "0" - ���� ��� ���� ����������������.                                    //
//****************************************************************************//
unsigned char RtcInit (void)
{
  //��������� ������������ ������� ���������� �������� � ����������� ��������� ��������
  RCC->APB1ENR |= RCC_APB1ENR_PWREN | RCC_APB1ENR_BKPEN;
  //��������� ������ � ������� ��������� ������
  PWR->CR |= PWR_CR_DBP;
  //���� ���� ��������� - ���������������� ��
  if ((RCC->BDCR & RCC_BDCR_RTCEN) != RCC_BDCR_RTCEN)
  {
    //��������� ����� ������� ��������� ������
    RCC->BDCR |=  RCC_BDCR_BDRST;
    RCC->BDCR &= ~RCC_BDCR_BDRST;

    //������� ���������� �������� ��������� ������� ����� 32768 � ������ ������������
    RCC->BDCR |=  RCC_BDCR_RTCEN | RCC_BDCR_RTCSEL_LSE;

    RTC->CRL  |=  RTC_CRL_CNF;
    //��������������� ��� ����� (����� �������� ��������� RTC,
    //������� ����� ��������������� ������ 2^20 ���������).
    BKP->RTCCR &= 0xFFC0;
    BKP->RTCCR |= 32;

    RTC->PRLL  = 0x7FFE;         //������� ������� �� 32768
    RTC->CRH  =  RTC_CRH_SECIE | RTC_CRH_ALRIE | RTC_CRH_OWIE;  //��������� ���������� �� ��������� ���������
    RTC->CRL  &=  ~RTC_CRL_CNF;

    //���������� ��� ���������� ������ � ��������� ��������� ���� ����������
    RCC->BDCR |= RCC_BDCR_LSEON;
    while ((RCC->BDCR & RCC_BDCR_LSEON) != RCC_BDCR_LSEON){}

    RTC->CRL &= (uint16_t)~RTC_CRL_RSF;
    while((RTC->CRL & RTC_CRL_RSF) != RTC_CRL_RSF){}

    return 1;
  }
  return 0;
}

//****************************************************************************//
// ������� ��������� ����������, ������������� RTC                            //                      //
//****************************************************************************//
void RTC_IRQHandler(void)
{
  //������� ���������� - ������������ �������� �������� (����� �������)
  if(RTC->CRL & RTC_CRL_SECF)
  {
     RTC->CRL &= ~RTC_CRL_SECF;    // �������� ���� (�����������!!!)

     tmp = RTC_GetCounter();       // ��������� ����� �� RTC
     RtcToTime(tmp, &timeNow);         // ������������� � "������������" ������

     DisplayTime(&timeNow);
  }

  //������� ���������� - ���������� �������� � ����������� ��������
  if(RTC->CRL & RTC_CRL_ALRF)
  {
     RTC->CRL &= ~RTC_CRL_ALRF;    //�������� ���� (�����������!!!)
     //��������� �����-�� ��������
  }

  //������� ���������� - ������������ �������� ��������
  if(RTC->CRL & RTC_CRL_OWF)
  {
     RTC->CRL &= ~RTC_CRL_OWF;     //�������� ���� (�����������!!!)
     //��������� �����-�� ��������
  }

}

//****************************************************************************//
// ������� �������������� ������ � ������ "�����" (���� 0->00:00:00)          //
// ���������:                                                                 //
//   cnt  - �������� ��������                                                 //
//   time - ��������� �� �������� "�����"                                     //
// ���������:                                                                 //
//   ���                                                                      //
//****************************************************************************//
void RtcToTime(uint32_t counter, RTC_Time *time)
{
	time->sec = counter % 60;   // �������� �������
	counter /= 60;              // ���������� �����
	time->min = counter % 60;   // �������� ������
	counter /= 60;              // ���������� ����
	time->hour = counter % 24;  // �������� ����
}

void SysTick_Handler(void)
{
	register uint32_t tempTime;    // ���������� ��� ������������� �������

	// ��������� ��������� ������ - ���� A0
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 1) // ������ ������
	{
		buttonTime++;

	    if (buttonTime == 10)
	    {
	    	GPIOC->BSRR = GPIO_Pin_8;
	    	tempTime = RTC_GetCounter();       // ��������� ����� �� RTC
	    	tempTime = tempTime + 60;          // ��������� 60 ������
	    	tempTime = tempTime / 60;          // ���������� �������, ��� ����� ����������
	    	tempTime = tempTime * 60;          // ������������� ������� � ����������� ���������� �� 60.
	    	RTC_SetCounter(tempTime);          // ���������� ����� �������� � RTC

	    	RtcToTime(tempTime, &timeNow);     // ������������� � "������������" ������
	    	DisplayTime(&timeNow);
	    	GPIOC->BRR = GPIO_Pin_8;
	    	return;
	    }

	    if (buttonTime > buttonTimeLimit)
	    {
	    	buttonTime = 0;
	    	buttonTimeLimit = 100;
			GPIOC->BSRR = GPIO_Pin_9;

	    	return;
	    }
	}
	else
	{
		buttonTime = 0;
		buttonTimeLimit = 1000;
		GPIOC->BRR = GPIO_Pin_9;
	}
}

void DisplayTime(RTC_Time *time)
{
	LCD_SetCursorPos(0, 0);
	LCD_PrintString("�����: ");
	if (time->hour < 10)
		LCD_PrintChar('0');
	LCD_PrintDec(time->hour);
	LCD_PrintChar(':');
	if (time->min < 10)
		LCD_PrintChar('0');
	LCD_PrintDec(time->min);
	LCD_PrintChar(':');
	if (time->sec < 10)
		LCD_PrintChar('0');
	LCD_PrintDec(time->sec);
	LCD_PrintChar('  ');
}

