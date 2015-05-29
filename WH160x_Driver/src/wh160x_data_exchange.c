#include "wh160x_data_exchange.h"

extern uint8_t lcdState;
extern uint8_t bufState;
extern uint8_t dataMode;

uint8_t byteToSend;

void LCD_SendByte(void)
{
	// ���������, ���������� �� �������������.
	if (lcdState & LCD_INIT)
	{
		// ���� ����� ����, �� ��������� ����� ����� �������.
		// ���������� ������ ������������� �������� ������
		// ��� �������� ���� �������� � �������.
		if (bufState & LCD_BUFFER_EMPTY)
		{
			LCD_TimeInterval(LCD_BYTE_PAUSE_DURATION);
			return;
		}

		// ���� � ������ ���� ������, �������� ���.
		LCD_BufferPop(&byteToSend);

		if (byteToSend == LCD_INIT_END)
		{
			// ���� ������������� ���������, ��������������
			// ������� ��������� ��� ���������� ������.
			lcdState &= (~LCD_INIT & ~LCD_SEND_COMMAND);
			lcdState |= LCD_FIRST_NIBBLE | LCD_NEXT_BYTE_DATA;
			LCD_TimeInterval(LCD_BYTE_PAUSE_DURATION);
			return;
		}
		
		LCD_Out4BitsToPort();
		return;
	}
	
	// ���������, ����� �������� ����� ����� ��������,
	// ��������� �� � ������������� ��������������� ����.
	if (lcdState & LCD_FIRST_NIBBLE)
	{
		// ���� ����� ����, �� ��������� ����� ����� �������.
		// ���������� ������ ������������� �������� ������
		// ��� �������� ���� �������� � �������.
		if (bufState & LCD_BUFFER_EMPTY)
		{
			LCD_TimeInterval(LCD_BYTE_PAUSE_DURATION);
			return;
		}

		// ���� � ������ ���� ������, �������� ���.
		LCD_BufferPop(&byteToSend);

		// ���� ��� ���������� ���� �����-�������, �������� �������.
		if (lcdState & LCD_NEXT_BYTE_DATA)
		{
			if (byteToSend == LCD_ESC_CODE)
			{
				// ���� ��� ������� 27, �� ��������� ���� - �������.
				// ������ � ������� ������ �� ��������, ��������� �
				// �������� �������.
				lcdState &= ~LCD_NEXT_BYTE_DATA;
				LCD_TimeInterval(LCD_BYTE_PAUSE_DURATION / 10);
				return;
			}

			// ������������� ��������� � ����� �������� ������.
			lcdState &= (~LCD_SEND_COMMAND & ~LCD_LONG_COMMAND);
		}
		else
		{
			// ��������� �������. ���� ������� ������� ����� �������
			// ��� ������, ������ ������� ������������ �����.
			if (byteToSend == LCD_CLEAR_COMMAND || byteToSend == LCD_RETURN_HOME)
			{
				lcdState |= LCD_LONG_COMMAND;
			}

			// ������������� ��������� � ����� �������� �������.
			lcdState |= (LCD_SEND_COMMAND | LCD_NEXT_BYTE_DATA);
		}

		// ��������� �������� ����� ������ �������� �����.
		lcdState &= ~LCD_FIRST_NIBBLE;
	}
	else
	{
		// ������� � �������� ������ �������� �����.
		byteToSend = byteToSend << 4;
		lcdState |= LCD_FIRST_NIBBLE;
	}

	// �������� ������ �������� �����.
	LCD_Out4BitsToPort();
}

void LCD_Out4BitsToPort(void)
{
	// ������������� ����������� �������.
	if (lcdState & LCD_SEND_COMMAND)
		LCD_RS_PORT->BRR = LCD_RS_PIN;
	else
		LCD_RS_PORT->BSRR = LCD_RS_PIN;

	LCD_RW_PORT->BRR = LCD_RW_PIN;

	// ������� ������� ��������.
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
	// ������������ ���������� �������.
	if (LCD_DELAY_TIMER->SR & TIM_SR_UIF)
	{
		// ����� ����� ��������������� ���������� UIF.
		LCD_DELAY_TIMER->SR = LCD_RESET;

		// �������� ������ ������.
		if (LCD_E_PORT->ODR & LCD_E_PIN)
		{
			// ����� ������.
			LCD_E_PORT->BRR = LCD_E_PIN;

			// ��������� ����� � ����������� �� ������������� �����.
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
	LCD_DELAY_TIMER->ARR = time;			// ��������� ��������.
	LCD_DELAY_TIMER->EGR = TIM_EGR_UG;		// �������������� ������������� ������� ���������.
	LCD_DELAY_TIMER->CR1 |= TIM_CR1_CEN;	// ������ �������.
}

void LCD_DataStrobe(void)
{
	// ��������� ������� ������.
	LCD_E_PORT->BSRR = LCD_E_PIN;
	// ������� ������������ ����� ��������.
    LCD_TimeInterval(LCD_STROBE_DURATION);
}
