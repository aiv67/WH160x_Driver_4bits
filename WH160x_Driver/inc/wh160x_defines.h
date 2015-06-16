#ifndef __WH160x_DEFINES
#define __WH160x_DEFINES

// ���������� ������ �������
#define LCD_SUCCESS					0
#define LCD_COMMON_ERROR			1

// ��� ������� WH1602 ��� WH1604, ���������� ����������
// ����� � �������� ��� ���������� ������
#define LCD_TYPE_WH1602

#ifdef LCD_TYPE_WH1602
#define LCD_LINES					2
#define LCD_COLUMNS					16
#endif /* LCD_TYPE=WH1602 */

#ifdef LCD_TYPE_WH1604
#define LCD_LINES 				4
#define LCD_COLUMNS 			16
#endif /* LCD_TYPE=WH1604 */

// ��� ������� DDRAM ������� �� 2 ������ �� 40 ��������
// ���������� �� ������ �������.
#define LCD_MAX_COLUMNS_IN_DDRAM	40

// ������� ����� ��� ��������� �����
#define BIT_7_MASK					0x80
#define BIT_6_MASK					0x40
#define BIT_5_MASK					0x20
#define BIT_4_MASK					0x10
#define BIT_3_MASK					0x08
#define BIT_2_MASK	    			0x04
#define BIT_1_MASK					0x02
#define BIT_0_MASK			 		0x01

// ��������� ���������
#define LCD_STROBE_DURATION			2		// ������������ �����-��������.
#define LCD_BYTE_PAUSE_DURATION		40		// ����� ����� �������.
#define LCD_LONG_COMMAND_DURATION	2000	// ����� ����� ������� � ���������� �������� (clear, set position).
#define LCD_START_PAUSE_DURATION	20000	// �������� ��� ��������� �������.

// ����, ������������ � ������� ��� �������������
#define LCD_INIT_BYTE 				0x30
// ������� ����� ������������� - ����, ������� ���� ��������� 
// � ����� ����� ������ �������������.
#define LCD_INIT_END				0x00

// ������ �������� ������ � �������
#define LCD_COMMAND_MODE			0x00
#define LCD_DATA_MODE				0x01

// ��������� 4-������� ������
#define LCD_DATA_MODE_4_BITS 		0x20

// ������� 2-��������, ������ �������� 5x8.
#define LCD_2_LINES					0x08

// ������� ��������� ������ � �������
#define LCD_COMMAND_DISPLAY			0x08

// ������� �������� ������
#define LCD_CLEAR_COMMAND			0x01

// ������� �������� � ���������� ���������: �������� ������� ���������� � 0,
// ��������� ���������� �� DDRAM, ����� ������� �������.
#define LCD_RETURN_HOME				0x02

// ��������� ������
#define LCD_ON						0x04

// ��������� ������� (������ �������������)
#define LCD_CURSOR_OFF				0x00
#define LCD_CURSOR_ON				0x02

// ��������� ������� �������
#define LCD_BLINK_OFF				0x00
#define LCD_BLINK_ON				0x01

// ����� (���������� ��� ������ �����).
#define LCD_RESET					0x0000

// ��������� ������
#define LCD_BUFFER_FULL				0x01 // ����� ������ ��������.
#define LCD_BUFFER_EMPTY			0x02 // ����� ������ ����.

// ��������� �������
#define LCD_INIT					0x01 // ����������� ������������� �������
#define LCD_NEXT_BYTE_DATA			0x02 // ��������� ���� ����� �������.
#define LCD_SEND_COMMAND			0x04 // �������� �������.
#define LCD_LONG_COMMAND			0x08 // �������� ���������� ������� (clear, set position).
#define LCD_FIRST_NIBBLE			0x10 // �������� ������� ���������.

// ��� �������, �����������, ��� ���������� �������.
#define LCD_ESC_CODE				0x1B

// NIBBLE - 4 ����. ����� ������������ ������� �����.
#define LCD_NIBBLE					0xF0

// ����� ��� ��������� ������� 4 ��� 32����������� �����.
#define LSB_MASK					0x0000000F

#endif /* __WH160x_DEFINES */
