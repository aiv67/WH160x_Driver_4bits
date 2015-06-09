#ifndef __WH160x_DRIVER
#define __WH160x_DRIVER

#include "wh160x_include_libraries.h"

/**
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * ����� �������������� ������� ���������� ����������
 * ������ ������, � ������� ����� ��������� �������
 * � ����� wh1602_port_def.h.
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

// ������� ����������
/**
 * ���������������� ���������� ������ ����� �����������.
 * @param GPIOx ��������������� ����
 * @param pin ��������������� �����
 * @param mode ����� ������ ������
 * @param speed �������� ������ ������
 */
void GPIO_Config(GPIO_TypeDef* GPIOx,
				uint16_t pin,
				GPIOMode_TypeDef mode,
				GPIOSpeed_TypeDef speed);

/**
 * ������������ �������.
 * @param cursorOn ��������� �������
 * @param cursorBlink ����� �������
 * @return ��� ������
 */
uint8_t LCD_Init(uint8_t cursorOn,
			uint8_t cursorBlink);

// ������� ������
/**
 * ������� �������
 */
void LCDClean(void);

/**
 * ���������������� �������
 * @param row ������
 * @param col �������
 */
void LCD_SetCursorPos(uint8_t row, uint8_t col);

/**
 * ����� �������
 * @param command ��������� �������
 */
void LCD_PrintCommand(unsigned char command);

/**
 * ����� ����� � ������� �������.
 * @param byte ��������� ����
 * @return ��� ������
 */
uint8_t LCD_PrintByte(unsigned char byte);

/**
 * ����� ������� � ������� �������.
 * ������ ������������� � ��� ������� ����� �������
 * �������������� LCD_charTable[].
 * @param c ��������� ������
 * @return ��� ������
 */
uint8_t LCD_PrintChar(unsigned char c);

/**
 * ����� ������.
 * ������ ������ ������������� \0. ����� ������������
 * ���� ��� ������ �� ����� �������� � �����.
 * @param string ��������� ������
 */
void LCD_PrintString(unsigned char *string);

/*
 * ����� ����� � ����������������� ������� 0xNNNN.
 * ���� ����� ����� ��������� �������� ���������� ��������,
 * ������� ������� �������������.
 * @param number - ��������� �����.
 * @param length - ���������� �������� ����� 0x.
 */
void LCD_PrintHex(long number, uint8_t length);

/**
 * ����� ������ ����� � ���������� ����.
 * @param number - ��������� �����.
 */
void LCD_PrintDec(long number);

#endif /* __WH160x_DRIVER */
