#ifndef __WH160x_BUFFER
#define __WH160x_BUFFER

#include "wh160x_include_libraries.h"

/**
 * ��������� ������� � �����.
 * @param c ������
 * @return SUCCESSFUL � ������ ��������� ���������,
 * 		   BUFFER_FULL, ���� ����� ��������.
 */
uint8_t LCD_BufferPush(unsigned char c);

/**
 * ���������� ������� �� ������.
 * @param *c ��������� �� ����������, � ������� ����� �������
 * 			����������� ������
 * @return SUCCESSFUL � ������ ��������� ����������,
 * 		   BUFFER_EMPTY, ���� ����� ����.
 */
uint8_t LCD_BufferPop(unsigned char *c);

#endif /* __WH160x_BUFFER */
