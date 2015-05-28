#include "wh160x_driver.h"
#include "wh160x_char_table.h"

/**
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * Перед использованием дисплея необходимо определить
 * выводы портов, к которым будет подключен дисплей
 * в файле wh160x_port_def.h.
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

/**
 * Инициализация портов контроллера
 * @return код ошибки
 */
uint8_t LCD_MCPortsInit(void);

/**
 * Проверка корректности конфигурирования портов.
 * @return код ошибки
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
	// Аналоговый порт для регулировки контрастности.
	GPIO_Config(LCD_ADJ_PORT, LCD_ADJ_PIN, GPIO_Mode_AIN, GPIO_Speed_2MHz);
	DAC->CR |= DAC_CR_EN1;
	DAC->DHR12R1 = LCD_INIT_CONTRAST;
#endif /* LCD_USE_ANALOG_PORT_TO_ADJUST_CONTRAST */

	// Управляющие порты.
	GPIO_Config(LCD_E_PORT, LCD_E_PIN, GPIO_Mode_Out_PP, GPIO_Speed_2MHz);
	GPIO_Config(LCD_RW_PORT, LCD_RW_PIN, GPIO_Mode_Out_PP, GPIO_Speed_2MHz);
	GPIO_Config(LCD_RS_PORT, LCD_RS_PIN, GPIO_Mode_Out_PP, GPIO_Speed_2MHz);

	// Порты данных.
	GPIO_Config(LCD_DB4_PORT, LCD_DB4_PIN, GPIO_Mode_Out_PP, GPIO_Speed_2MHz);
	GPIO_Config(LCD_DB5_PORT, LCD_DB5_PIN, GPIO_Mode_Out_PP, GPIO_Speed_2MHz);
	GPIO_Config(LCD_DB6_PORT, LCD_DB6_PIN, GPIO_Mode_Out_PP, GPIO_Speed_2MHz);
	GPIO_Config(LCD_DB7_PORT, LCD_DB7_PIN, GPIO_Mode_Out_PP, GPIO_Speed_2MHz);

	// Таймер для формирования задержек.
	LCD_DELAY_TIMER->CR1 = LCD_RESET; 	// Сброс регистров управления.
	LCD_DELAY_TIMER->CR2 = LCD_RESET;
	LCD_DELAY_TIMER->CR1 |= TIM_CR1_ARPE 	// Регистр перезагрузки счетчика буферизован.
			| TIM_CR1_OPM 					// Одноимпульсный режим работы.
			| TIM_CR1_URS;					// Прерывания генерируются только при переполнении счетчика.
	LCD_DELAY_TIMER->PSC = RCC_Clock.SYSCLK_Frequency / 500000; // Коэффициент предделителя.
	LCD_DELAY_TIMER->ARR = 4; 				// Начальное значение счетчика.
	LCD_DELAY_TIMER->SR = LCD_RESET;		// Сброс флага прерывания.
	LCD_DELAY_TIMER->EGR = TIM_EGR_UG;		// Принудительная инициализация счетных регистров.
	LCD_DELAY_TIMER->DIER = TIM_DIER_UIE; 	// Разрешение прерывания от таймера.
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
	// Инициализация переменных состояния.
	lcdState = LCD_INIT | LCD_SEND_COMMAND;
	bufState = LCD_BUFFER_EMPTY;

	uint8_t initPortsResult = LCD_MCPortsInit();
	if (initPortsResult)
		return initPortsResult;

	// Установка 0 на управляющих выводах.
	LCD_E_PORT->BRR = LCD_E_PIN;
	LCD_RS_PORT->BRR = LCD_RS_PIN;
	LCD_RW_PORT->BRR = LCD_RW_PIN;

	// Стартовая пауза.
	LCD_TimeInterval(LCD_START_PAUSE_DURATION);

	// Байты инициализации дисплея.
	while(LCD_PrintByte(LCD_INIT_BYTE) > 0);
	while(LCD_PrintByte(LCD_INIT_BYTE) > 0);
	while(LCD_PrintByte(LCD_INIT_BYTE) > 0);
	while(LCD_PrintByte(LCD_DATA_MODE_4_BITS) > 0);
	// Байт завершения инициализации.
	while(LCD_PrintByte(LCD_INIT_END) > 0);
	
	// Установка режима дисплея (4-битный или 8-битный).
	// Дисплей 2-строчный, размер символов 5x8.
	LCD_PrintCommand(LCD_DATA_MODE_4_BITS | LCD_2_LINES);

	// Очистка дисплея.
	LCD_PrintCommand(LCD_CLEAR_COMMAND);

	// Включаем дисплей (курсор включен, мигает).
	LCD_PrintCommand(LCD_COMMAND_DISPLAY | LCD_ON | cursorOn | cursorBlink);

	return 0;
}

void LCD_SetCursorPos(uint8_t row, uint8_t col)
{

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
