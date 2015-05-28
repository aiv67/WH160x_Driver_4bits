#include "wh160x_buffer.h"

extern uint8_t bufState;

// Буфер символов для вывода на дисплей.
// Размер буфера 256, чтобы проще было реализовать индекс.
// Достаточно переменной типа uint8_t, чтобы одним инкрементированием
// обеспечить циклическую очередь. После значения 255 переход будет в
// значение 0, т.е. опять попадаем в начало буфера.
unsigned char lcdBuffer[256];

// Текущая позиция в буфере.
uint8_t bufPos = 0;

// Вершина буфера - первая свободная ячейка.
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
