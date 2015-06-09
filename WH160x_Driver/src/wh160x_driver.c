#include "wh160x_driver.h"
#include "wh160x_char_table.h"

/**
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * ����� �������������� ������� ���������� ����������
 * ������ ������, � ������� ����� ��������� �������
 * � ����� wh160x_port_def.h.
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

/**
 * ������������� ������ �����������
 * @return ��� ������
 */
uint8_t LCD_MCPortsInit(void);

/**
 * �������� ������������ ���������������� ������.
 * @return ��� ������
 */
uint8_t LCD_CheckConfig(void);

volatile uint8_t lcdState = 0;
volatile uint8_t bufState = 0;

extern const unsigned char lcdBuffer[256];

uint8_t LCD_MCPortsInit(void)
{
	uint8_t checkConfigResult = LCD_CheckConfig();
	if (checkConfigResult)
		return checkConfigResult;

	RCC_ClocksTypeDef RCC_Clock;
	RCC_GetClocksFreq(&RCC_Clock);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

#ifdef LCD_USE_ANALOG_PORT_TO_ADJUST_CONTRAST
	// ���������� ���� ��� ����������� �������������.
	GPIO_Config(LCD_ADJ_PORT, LCD_ADJ_PIN, GPIO_Mode_AIN, GPIO_Speed_2MHz);
	DAC->CR |= DAC_CR_EN1;
	DAC->DHR12R1 = LCD_INIT_CONTRAST;
#endif /* LCD_USE_ANALOG_PORT_TO_ADJUST_CONTRAST */

	// ����������� �����.
	GPIO_Config(LCD_E_PORT, LCD_E_PIN, GPIO_Mode_Out_PP, GPIO_Speed_2MHz);
	GPIO_Config(LCD_RW_PORT, LCD_RW_PIN, GPIO_Mode_Out_PP, GPIO_Speed_2MHz);
	GPIO_Config(LCD_RS_PORT, LCD_RS_PIN, GPIO_Mode_Out_PP, GPIO_Speed_2MHz);

	// ����� ������.
	GPIO_Config(LCD_DB4_PORT, LCD_DB4_PIN, GPIO_Mode_Out_PP, GPIO_Speed_2MHz);
	GPIO_Config(LCD_DB5_PORT, LCD_DB5_PIN, GPIO_Mode_Out_PP, GPIO_Speed_2MHz);
	GPIO_Config(LCD_DB6_PORT, LCD_DB6_PIN, GPIO_Mode_Out_PP, GPIO_Speed_2MHz);
	GPIO_Config(LCD_DB7_PORT, LCD_DB7_PIN, GPIO_Mode_Out_PP, GPIO_Speed_2MHz);

	// ������ ��� ������������ ��������.
	LCD_DELAY_TIMER->CR1 = LCD_RESET; 	// ����� ��������� ����������.
	LCD_DELAY_TIMER->CR2 = LCD_RESET;
	LCD_DELAY_TIMER->CR1 |= TIM_CR1_ARPE 	// ������� ������������ �������� �����������.
			| TIM_CR1_OPM 					// �������������� ����� ������.
			| TIM_CR1_URS;					// ���������� ������������ ������ ��� ������������ ��������.
	LCD_DELAY_TIMER->PSC = RCC_Clock.SYSCLK_Frequency / 500000; // ����������� ������������.
	LCD_DELAY_TIMER->ARR = 4; 				// ��������� �������� ��������.
	LCD_DELAY_TIMER->SR = LCD_RESET;		// ����� ����� ����������.
	LCD_DELAY_TIMER->EGR = TIM_EGR_UG;		// �������������� ������������� ������� ���������.
	LCD_DELAY_TIMER->DIER = TIM_DIER_UIE; 	// ���������� ���������� �� �������.
	NVIC_EnableIRQ(LCD_DELAY_TIMER_IT);

	return 0;
}

uint8_t LCD_CheckConfig(void)
{
	return 0;
}

void GPIO_Config(GPIO_TypeDef* GPIOx,
				uint16_t pin,
				GPIOMode_TypeDef mode,
				GPIOSpeed_TypeDef speed)
{
	GPIO_InitTypeDef pinOut;

	pinOut.GPIO_Pin = pin;
	pinOut.GPIO_Mode = mode;
	pinOut.GPIO_Speed = speed;

	GPIO_Init(GPIOx, &pinOut);
}

uint8_t LCD_Init(uint8_t cursorOn,
			uint8_t cursorBlink)
{
	// ������������� ���������� ���������.
	lcdState = LCD_INIT | LCD_SEND_COMMAND;
	bufState = LCD_BUFFER_EMPTY;

	uint8_t initPortsResult = LCD_MCPortsInit();
	if (initPortsResult)
		return initPortsResult;

	// ��������� 0 �� ����������� �������.
	LCD_E_PORT->BRR = LCD_E_PIN;
	LCD_RS_PORT->BRR = LCD_RS_PIN;
	LCD_RW_PORT->BRR = LCD_RW_PIN;

	// ��������� �����.
	LCD_TimeInterval(LCD_START_PAUSE_DURATION);

	// ����� ������������� �������.
	while(LCD_PrintByte(LCD_INIT_BYTE) > 0);
	while(LCD_PrintByte(LCD_INIT_BYTE) > 0);
	while(LCD_PrintByte(LCD_INIT_BYTE) > 0);
	while(LCD_PrintByte(LCD_DATA_MODE_4_BITS) > 0);
	// ���� ���������� �������������.
	while(LCD_PrintByte(LCD_INIT_END) > 0);
	
	// ��������� ������ ������� (4-������ ��� 8-������).
	// ������� 2-��������, ������ �������� 5x8.
	LCD_PrintCommand(LCD_DATA_MODE_4_BITS | LCD_2_LINES);

	// ������� �������.
	LCD_PrintCommand(LCD_CLEAR_COMMAND);

	// �������� ������� (������ �������, ������).
	LCD_PrintCommand(LCD_COMMAND_DISPLAY | LCD_ON | cursorOn | cursorBlink);

	return 0;
}

void LCD_SetCursorPos(uint8_t row, uint8_t col)
{
	if (row > LCD_LINES || col > LCD_COLUMNS)
		return;

#ifdef LCD_TYPE_WH1602
	uint8_t address = row * 0x40 + col;
#endif /* LCD_TYPE_WH1602 */

#ifdef LCD_TYPE_WH1604
	uint8_t address = (row / 2) * 0x10 + (row % 2) * 0x40 + col;
/*
	uint8_t address;
	switch (row)
	{
	case 0:
		address = col;
		break;
	case 1:
		address = 0x40 + col;
		break;
	case 2:
		address = 0x10 + col;
		break;
	case 3:
		address = 0x50 + col;
		break;
	}
*/
#endif /* LCD_TYPE_WH1604 */

	address |= 0x80;
	LCD_PrintCommand(address);
}

void LCD_PrintCommand(unsigned char command)
{
	while(LCD_PrintByte(LCD_ESC_CODE) > 0);
	while(LCD_PrintByte(command) > 0);
}

uint8_t LCD_PrintByte(unsigned char byte)
{
	return LCD_BufferPush(byte);
}

uint8_t LCD_PrintChar(unsigned char c)
{
	return LCD_BufferPush(LCD_charTable[c]);
}

void LCD_PrintString(unsigned char *string)
{
	while(*string) {
		while(LCD_PrintChar(*string) > 0);
		string++;
	}
}

void LCD_PrintHex(long number, uint8_t length)
{
	unsigned char stringForPrint[length + 3];
	uint8_t i;
	for (i = 0; i < length + 2; i++)
	{
		stringForPrint[i] = '0';
	}
	stringForPrint[1] = 'x';
	stringForPrint[length + 2] = 0;

	i = length + 1;
	uint8_t digit;
	while (number)
	{
		digit = number & LSB_MASK;
		stringForPrint[i] = (digit < 10) ? '0' + digit : '7' + digit;
//		if (digit < 10)
//			stringForPrint[i] = '0' + digit;
//		else
//			stringForPrint[i] = '7' + digit;
		if (--i == 1)
		{
			break;
		}
		number = number >> 4;
	}

	LCD_PrintString(stringForPrint);
}

void LCD_PrintDec(long number)
{
	unsigned char stringTemplate[21] = "00000000000000000000";

	unsigned char *stringForPrint;
	for (stringForPrint = stringTemplate + 19; ; stringForPrint--)
	{
		*stringForPrint = (number % 10) + '0';
		number /= 10;
		if (!number)
		{
			break;
		}
	}

	LCD_PrintString(stringForPrint);
}
