#ifndef __WH160x_PORT_DEF
#define __WH160x_PORT_DEF

/**
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * ����� �������������� ������� ���������� ����������
 * ������ ������, � ������� ����� ��������� �������
 * � ����� wh160x_port_def.h.
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

// ������ ��� ������������ �������� � ���������� �� ����.
// ���� ������ ����� �������, ����� ���� ����� �������������
// ��������� ��������� ���������� (TIM6_DAC_IRQHandler).
#define LCD_DELAY_TIMER 	TIM6
#define LCD_DELAY_TIMER_IT 	TIM6_DAC_IRQn

// ����� ADJ (3) ������� (����������� �������, ����� ����������
// ������ � ������� PA4 ��� PA5 - ���������� ������ DAC).
// ���� ����������� � �������������� ����������� ������
// �� ������������, ��������� ������ ���� ����������������.
#define LCD_USE_ANALOG_PORT_TO_ADJUST_CONTRAST

#ifdef LCD_USE_ANALOG_PORT_TO_ADJUST_CONTRAST
#define LCD_ADJ_PORT		GPIOA		// Port
#define LCD_ADJ_PIN			GPIO_Pin_4	// Pin
#define LCD_INIT_CONTRAST 	0x400	// ��������� ���������� �� ������ ����������� �������,
									// 0 - ������ ��������������, 0xFFFF - ������ �� �����.
#endif /* LCD_USE_ANALOG_PORT_TO_ADJUST_CONTRAST */

// ����� RS (4) �������
#define LCD_RS_PORT			GPIOC		// Port
#define LCD_RS_PIN			GPIO_Pin_4	// Pin
// ����� RW (5) �������
#define LCD_RW_PORT			GPIOC		// Port
#define LCD_RW_PIN			GPIO_Pin_10	// Pin
// ����� E (6) �������
#define LCD_E_PORT			GPIOC		// Port
#define LCD_E_PIN			GPIO_Pin_5	// Pin
// � ������������� ������ ������������ ������� 4 �������.
// ����� DB4 (11) �������
#define LCD_DB4_PORT		GPIOC		// Port
#define LCD_DB4_PIN			GPIO_Pin_0	// Pin
// ����� DB5 (12) �������
#define LCD_DB5_PORT		GPIOC		// Port
#define LCD_DB5_PIN			GPIO_Pin_1	// Pin
// ����� DB6 (13) �������
#define LCD_DB6_PORT		GPIOC		// Port
#define LCD_DB6_PIN			GPIO_Pin_2	// Pin
// ����� DB7 (14) �������
#define LCD_DB7_PORT		GPIOC		// Port
#define LCD_DB7_PIN			GPIO_Pin_3	// Pin

#endif /* __WH160x_PORT_DEF */
