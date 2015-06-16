#include "rtc.h"

//****************************************************************************//
// Функция инициализации RTC.                                                 //
// Параметры:                                                                 //
//   нет.                                                                     //
// Результат unsigned char:                                                   //
//   "1" - инициализация выполнена;                                           //
//   "0" - часы уже были инициализированы.                                    //
//****************************************************************************//
unsigned char RtcInit (void)
{
  //разрешить тактирование модулей управления питанием и управлением резервной областью
  RCC->APB1ENR |= RCC_APB1ENR_PWREN | RCC_APB1ENR_BKPEN;
  //разрешить доступ к области резервных данных
  PWR->CR |= PWR_CR_DBP;
  //если часы выключены - инициализировать их
  if ((RCC->BDCR & RCC_BDCR_RTCEN) != RCC_BDCR_RTCEN)
  {
    //выполнить сброс области резервных данных
    RCC->BDCR |=  RCC_BDCR_BDRST;
    RCC->BDCR &= ~RCC_BDCR_BDRST;

    //выбрать источником тактовых импульсов внешний кварц 32768 и подать тактирование
    RCC->BDCR |=  RCC_BDCR_RTCEN | RCC_BDCR_RTCSEL_LSE;

    //скорректировать ход часов на 1 (число тактовых импульсов RTC,
    //которые будут проигнорированы каждые 2^20 импульсов).
    //BKP->RTCCR |= 0&1;

    RTC->CRL  |=  RTC_CRL_CNF;
    RTC->PRLL  = 0x7FFF;         //регистр деления на 32768
    RTC->CRH  =  RTC_CRH_SECIE | RTC_CRH_ALRIE | RTC_CRH_OWIE;  //разрешить прерывание от секундных импульсов
    RTC->CRL  &=  ~RTC_CRL_CNF;

    //установить бит разрешения работы и дождаться установки бита готовности
    RCC->BDCR |= RCC_BDCR_LSEON;
    while ((RCC->BDCR & RCC_BDCR_LSEON) != RCC_BDCR_LSEON){}

    RTC->CRL &= (uint16_t)~RTC_CRL_RSF;
    while((RTC->CRL & RTC_CRL_RSF) != RTC_CRL_RSF){}

    return 1;
  }
  return 0;
}

//****************************************************************************//
// Функция обработки прерывания, генерируемого RTC                            //                      //
//****************************************************************************//
void RTC_IRQHandler(void)
{
  //причина прерывания - переполнение входного делителя (новая секунда)
  if(RTC->CRL & RTC_CRL_SECF)
  {
     RTC->CRL &= ~RTC_CRL_SECF;    // сбросить флаг (обязательно!!!)

     tmp = RTC_GetCounter();       // прочитать время из RTC
     RtcToTime(tmp, &TimeNow);         // преобразовать в "человеческий" формат

     DisplayTime(&TimeNow);
  }

  //причина прерывания - совпадение счетного и сигнального регистра
  if(RTC->CRL & RTC_CRL_ALRF)
  {
     RTC->CRL &= ~RTC_CRL_ALRF;    //сбросить флаг (обязательно!!!)
     //выполняем какие-то действия
  }

  //причина прерывания - переполнение счетного регистра
  if(RTC->CRL & RTC_CRL_OWF)
  {
     RTC->CRL &= ~RTC_CRL_OWF;     //сбросить флаг (обязательно!!!)
     //выполняем какие-то действия
  }

}

//****************************************************************************//
// Функция преобразования секунд в формат "Время" (если 0->00:00:00)          //
// Аргументы:                                                                 //
//   cnt  - значение счетчика                                                 //
//   time - указатель на струтуру "Время"                                     //
// Результат:                                                                 //
//   нет                                                                      //
//****************************************************************************//
void RtcToTime(uint32_t counter, RTC_Time *time)
{
	time->sec = counter % 60;   // получить секунды
	counter /= 60;              // количество минут
	time->min = counter % 60;   // получить минуты
	counter /= 60;              // количество дней
	time->hour = counter % 24;  // получить часы
}

void SysTick_Handler(void)
{
	  // Проверяем состояние кнопки - порт A0
	  if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)==1) // кнопка нажата
	  {
			  ButtonTime++;

	    if(ButtonTime == 20)
	    {
	      TempTime = RTC_GetCounter();       // прочитать время из RTC
	      TempTime = TempTime + 60;          // добавляем 60 секунд
	      TempTime = TempTime / 60;          // сбрасываем секунды, для этого производим
	      TempTime = TempTime * 60;          // целочисленное деление с последующим умножением на 60.
	      RTC_SetCounter(TempTime);          // записываем новое значение в RTC

	      RtcToTime(TempTime, &TimeNow);     // преобразовать в "человеческий" формат
	      DisplayTime(&TimeNow);
	    }

	    if (ButtonTime == 250)
	    	ButtonTime = 0;
	  }
	  else
	  {
		  if (ButtonTime > 0)
		  ButtonTime--;
	  }
}

void DisplayTime(RTC_Time *time)
{
	LCD_SetCursorPos(0, 0);
	LCD_PrintString("Время: ");
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

