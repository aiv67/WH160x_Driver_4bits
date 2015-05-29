#include "wh160x_data_exchange.h"

extern uint8_t lcdState;
extern uint8_t bufState;
extern uint8_t dataMode;

uint8_t byteToSend;

void LCD_SendByte(void)
{
	// Проверяем, происходит ли инициализация.
	if (lcdState & LCD_INIT)
	{
		// Если буфер пуст, то запускаем паузу между байтами.
		// Фактически делаем периодическую проверку буфера
		// для отправки всех символов в дисплей.
		if (bufState & LCD_BUFFER_EMPTY)
		{
			LCD_TimeInterval(LCD_BYTE_PAUSE_DURATION);
			return;
		}

		// Если в буфере есть символ, получаем его.
		LCD_BufferPop(&byteToSend);

		if (byteToSend == LCD_INIT_END)
		{
			// Если инициализация завершена, подготавливаем
			// регистр состояния для нормальной работы.
			lcdState &= (~LCD_INIT & ~LCD_SEND_COMMAND);
			lcdState |= LCD_FIRST_NIBBLE | LCD_NEXT_BYTE_DATA;
			LCD_TimeInterval(LCD_BYTE_PAUSE_DURATION);
			return;
		}
		
		LCD_Out4BitsToPort();
		return;
	}
	
	// Проверяем, какую половину байта будем посылать,
	// вычисляем ее и устанавливаем соответствующий флаг.
	if (lcdState & LCD_FIRST_NIBBLE)
	{
		// Если буфер пуст, то запускаем паузу между байтами.
		// Фактически делаем периодическую проверку буфера
		// для отправки всех символов в дисплей.
		if (bufState & LCD_BUFFER_EMPTY)
		{
			LCD_TimeInterval(LCD_BYTE_PAUSE_DURATION);
			return;
		}

		// Если в буфере есть символ, получаем его.
		LCD_BufferPop(&byteToSend);

		// Если был установлен флаг байта-команды, передаем команду.
		if (lcdState & LCD_NEXT_BYTE_DATA)
		{
			if (byteToSend == LCD_ESC_CODE)
			{
				// Если код символа 27, то следующий байт - команда.
				// Сейчас в дисплей ничего не посылаем, готовимся к
				// передаче команды.
				lcdState &= ~LCD_NEXT_BYTE_DATA;
				LCD_TimeInterval(LCD_BYTE_PAUSE_DURATION / 10);
				return;
			}

			// Устанавливаем состояние в режим передачи данных.
			lcdState &= (~LCD_SEND_COMMAND & ~LCD_LONG_COMMAND);
		}
		else
		{
			// Проверяем команду. Если команда требует много времени
			// для работы, задаем большую длительность паузы.
			if (byteToSend == LCD_CLEAR_COMMAND || byteToSend == LCD_RETURN_HOME)
			{
				lcdState |= LCD_LONG_COMMAND;
			}

			// Устанавливаем состояние в режим передачи команды.
			lcdState |= (LCD_SEND_COMMAND | LCD_NEXT_BYTE_DATA);
		}

		// Следующей посылкой будет вторая половина байта.
		lcdState &= ~LCD_FIRST_NIBBLE;
	}
	else
	{
		// Готовим к передаче вторую половину байта.
		byteToSend = byteToSend << 4;
		lcdState |= LCD_FIRST_NIBBLE;
	}

	// Посылаем нужную половину байта.
	LCD_Out4BitsToPort();
}

void LCD_Out4BitsToPort(void)
{
	// Устанавливаем управляющие сигналы.
	if (lcdState & LCD_SEND_COMMAND)
		LCD_RS_PORT->BRR = LCD_RS_PIN;
	else
		LCD_RS_PORT->BSRR = LCD_RS_PIN;

	LCD_RW_PORT->BRR = LCD_RW_PIN;

	// Выводим старший полубайт.
	if (byteToSend & BIT_7_MASK)
		LCD_DB7_PORT->BSRR = LCD_DB7_PIN;
	else
		LCD_DB7_PORT->BRR = LCD_DB7_PIN;

	if (byteToSend & BIT_6_MASK)
		LCD_DB6_PORT->BSRR = LCD_DB6_PIN;
	else
		LCD_DB6_PORT->BRR = LCD_DB6_PIN;

	if (byteToSend & BIT_5_MASK)
		LCD_DB5_PORT->BSRR = LCD_DB5_PIN;
	else
		LCD_DB5_PORT->BRR = LCD_DB5_PIN;

	if (byteToSend & BIT_4_MASK)
		LCD_DB4_PORT->BSRR = LCD_DB4_PIN;
	else
		LCD_DB4_PORT->BRR = LCD_DB4_PIN;

	LCD_DataStrobe();
}

void TIM6_DAC_IRQHandler(void)
{
	// Обрабатываем прерывание таймера.
	if (LCD_DELAY_TIMER->SR & TIM_SR_UIF)
	{
		// Сброс флага обрабатываемого прерывания UIF.
		LCD_DELAY_TIMER->SR = LCD_RESET;

		// Проверка вывода строба.
		if (LCD_E_PORT->ODR & LCD_E_PIN)
		{
			// Сброс строба.
			LCD_E_PORT->BRR = LCD_E_PIN;

			// Формируем паузу в зависимости от передаваемого байта.
			if (lcdState & LCD_LONG_COMMAND)
			{
				LCD_TimeInterval(LCD_LONG_COMMAND_DURATION);
			}
			else
			{
				LCD_TimeInterval(LCD_BYTE_PAUSE_DURATION);
			}

			return;
		}

		LCD_SendByte();
	}
}

void LCD_TimeInterval(uint16_t time)
{
	LCD_DELAY_TIMER->ARR = time;			// Установка счетчика.
	LCD_DELAY_TIMER->EGR = TIM_EGR_UG;		// Принудительная инициализация счетных регистров.
	LCD_DELAY_TIMER->CR1 |= TIM_CR1_CEN;	// Запуск таймера.
}

void LCD_DataStrobe(void)
{
	// Установка сигнала строба.
	LCD_E_PORT->BSRR = LCD_E_PIN;
	// Задание длительности строб импульса.
    LCD_TimeInterval(LCD_STROBE_DURATION);
}
