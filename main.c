#ifndef _STDINT_H
#include "stdint.h"
#endif /* _STDINT_H */

#include "inc/wh160x_driver.h"
#include "rtc.h"


void Delay(uint64_t time)
{
    volatile uint64_t i = 2 * time;

    while (i-- > 0);
}

char *hex(char c, char *result)
{
	uint8_t digit = c & 0x0F;
	*(result + 4) = 0;
	if (digit < 10)
		*(result + 3) = '0' + digit;
	else
		*(result + 3) = '7' + digit;

	digit = c >> 4;
	if (digit < 10)
		*(result + 2) = '0' + digit;
	else
		*(result + 2) = '7' + digit;

	return result;
}

void printCode(char c)
{
	char buffer[5] = "0x00";
	LCD_PrintChar(c);
	LCD_PrintChar('=');
	LCD_PrintHex(c, 2);
	Delay(1000000);
}

int main(void)
{
	LCD_Init(LCD_CURSOR_OFF, LCD_BLINK_OFF);

	Delay(1000000);

	int i;

/*
	uint8_t row = 0;
	uint8_t col = 0xFF;
	for (i = 32; i < 256; i++)
	{
		int pos = i % 8;
		LCD_SetCursorPos(pos / 2, (pos % 2) * 8);
		printCode((char) i);
	}
*/

	SysTick_Config(24000);
	RtcInit();
	// –азрешение прерываний от RTC.
	RTC->CRL |= RTC_CRL_CNF;   // разрешить конфигурирование регистров RTC
	RTC->CRH = RTC_CRH_SECIE;  // разрешить прерывание от секундных импульсов
	RTC->CRL &= ~RTC_CRL_CNF;  // выйти из режима конфигурировани€

	NVIC_EnableIRQ (RTC_IRQn);

	char digits[6] = "00000\0";
	RCC_ClocksTypeDef RCC_Clock;
	uint32_t delayParam = 3000000;

    while(1)
    {
    	RCC_GetClocksFreq(&RCC_Clock);

    	uint32_t frequency = RCC_Clock.SYSCLK_Frequency / 1000;
    	LCD_SetCursorPos(1, 0);
    	LCD_PrintString("SYSCLK=");
    	LCD_PrintDec(frequency);
    	LCD_PrintString("к√ц");

    	Delay(delayParam);

    	frequency = RCC_Clock.HCLK_Frequency / 1000;
    	LCD_SetCursorPos(1, 0);
    	LCD_PrintString("HCLK  =");
    	LCD_PrintDec(frequency);
    	LCD_PrintString("к√ц");

    	Delay(delayParam);

    	frequency = RCC_Clock.PCLK1_Frequency / 1000;
    	LCD_SetCursorPos(1, 0);
    	LCD_PrintString("PCLK1 =");
    	LCD_PrintDec(frequency);
    	LCD_PrintString("к√ц");

    	Delay(delayParam);

    	frequency = RCC_Clock.PCLK2_Frequency / 1000;
    	LCD_SetCursorPos(1, 0);
    	LCD_PrintString("PCLK2 =");
    	LCD_PrintDec(frequency);
    	LCD_PrintString("к√ц");

    	Delay(delayParam);

    	frequency = RCC_Clock.ADCCLK_Frequency / 1000;
    	LCD_SetCursorPos(1, 0);
    	LCD_PrintString("ADCCLK=");
    	LCD_PrintDec(frequency);
    	LCD_PrintString("к√ц");

    	Delay(delayParam);
    }
}
