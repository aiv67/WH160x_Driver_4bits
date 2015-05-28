#include "wh160x_buffer.h"

extern uint8_t bufState;

// ����� �������� ��� ������ �� �������.
// ������ ������ 256, ����� ����� ���� ����������� ������.
// ���������� ���������� ���� uint8_t, ����� ����� ������������������
// ���������� ����������� �������. ����� �������� 255 ������� ����� �
// �������� 0, �.�. ����� �������� � ������ ������.
unsigned char lcdBuffer[256];

// ������� ������� � ������.
uint8_t bufPos = 0;

// ������� ������ - ������ ��������� ������.
uint8_t bufTop = 0;

uint8_t LCD_BufferPush(unsigned char c)
{
	if (bufState & LCD_BUFFER_FULL)
	{
		return LCD_BUFFER_FULL;
	}

	lcdBuffer[bufTop++] = c;
	if (bufTop == bufPos)
	{
		bufState |= LCD_BUFFER_FULL;
	}

	bufState &= ~LCD_BUFFER_EMPTY;

	return LCD_SUCCESS;
}

uint8_t LCD_BufferPop(unsigned char *c)
{
	if (bufState & LCD_BUFFER_EMPTY)
	{
		return LCD_BUFFER_EMPTY;
	}

	*c = lcdBuffer[bufPos++];
	if (bufPos == bufTop)
	{
		bufState |= LCD_BUFFER_EMPTY;
	}

	bufState &= ~LCD_BUFFER_FULL;

	return LCD_SUCCESS;
}
