#ifndef __WH160x_PORT_DEF
#define __WH160x_PORT_DEF

/**
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * Перед использованием дисплея необходимо определить
 * выводы портов, к которым будет подключен дисплей
 * в файле wh160x_port_def.h.
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

// Таймер для формирования задержек и прерывание от него.
// Если таймер будет изменен, также надо будет переименовать
// процедуру обработки прерывания (TIM6_DAC_IRQHandler).
#define LCD_DELAY_TIMER 	TIM6
#define LCD_DELAY_TIMER_IT 	TIM6_DAC_IRQn

// Вывод ADJ (3) дисплея (регулировка яркости, можно подключить
// только к выводам PA4 или PA5 - аналоговые выходы DAC).
// Если регулировка с использованием аналогового выхода
// не используется, следующую строку надо закомментировать.
#define LCD_USE_ANALOG_PORT_TO_ADJUST_CONTRAST

#ifdef LCD_USE_ANALOG_PORT_TO_ADJUST_CONTRAST
#define LCD_ADJ_PORT		GPIOA		// Port
#define LCD_ADJ_PIN			GPIO_Pin_4	// Pin
#define LCD_INIT_CONTRAST 	0x400	// Начальное напряжение на выводе регулировки яркости,
									// 0 - черные прямоугольники, 0xFFFF - ничего не видно.
#endif /* LCD_USE_ANALOG_PORT_TO_ADJUST_CONTRAST */

// Вывод RS (4) дисплея
#define LCD_RS_PORT			GPIOC		// Port
#define LCD_RS_PIN			GPIO_Pin_4	// Pin
// Вывод RW (5) дисплея
#define LCD_RW_PORT			GPIOC		// Port
#define LCD_RW_PIN			GPIO_Pin_10	// Pin
// Вывод E (6) дисплея
#define LCD_E_PORT			GPIOC		// Port
#define LCD_E_PIN			GPIO_Pin_5	// Pin
// В четырехбитном режиме используются старшие 4 разряда.
// Вывод DB4 (11) дисплея
#define LCD_DB4_PORT		GPIOC		// Port
#define LCD_DB4_PIN			GPIO_Pin_0	// Pin
// Вывод DB5 (12) дисплея
#define LCD_DB5_PORT		GPIOC		// Port
#define LCD_DB5_PIN			GPIO_Pin_1	// Pin
// Вывод DB6 (13) дисплея
#define LCD_DB6_PORT		GPIOC		// Port
#define LCD_DB6_PIN			GPIO_Pin_2	// Pin
// Вывод DB7 (14) дисплея
#define LCD_DB7_PORT		GPIOC		// Port
#define LCD_DB7_PIN			GPIO_Pin_3	// Pin

#endif /* __WH160x_PORT_DEF */
