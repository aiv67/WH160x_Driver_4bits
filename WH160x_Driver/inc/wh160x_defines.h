#ifndef __WH160x_DEFINES
#define __WH160x_DEFINES

// Завершение работы функции
#define LCD_SUCCESS					0
#define LCD_COMMON_ERROR			1

// Тип дисплея WH1602 или WH1604, определяем количество
// строк и столбцов для конкретной модели
#define LCD_TYPE_WH1602

#ifdef LCD_TYPE_WH1602
#define LCD_LINES					2
#define LCD_COLUMNS					16
#endif /* LCD_TYPE=WH1602 */

#ifdef LCD_TYPE_WH1604
#define LCD_LINES 				4
#define LCD_COLUMNS 			16
#endif /* LCD_TYPE=WH1604 */

// Вся область DDRAM разбита на 2 строки по 40 символов
// независимо от модели дисплея.
#define LCD_MAX_COLUMNS_IN_DDRAM	40

// Битовые маски для одиночных битов
#define BIT_7_MASK					0x80
#define BIT_6_MASK					0x40
#define BIT_5_MASK					0x20
#define BIT_4_MASK					0x10
#define BIT_3_MASK					0x08
#define BIT_2_MASK	    			0x04
#define BIT_1_MASK					0x02
#define BIT_0_MASK			 		0x01

// Временные параметры
#define LCD_STROBE_DURATION			10		// Длительность строб-импульса.
#define LCD_BYTE_PAUSE_DURATION		400		// Пауза между байтами.
#define LCD_LONG_COMMAND_DURATION	10000	// Пауза между байтами в длительных командах (clear, set position).
#define LCD_START_PAUSE_DURATION	20000	// Задержка при включении дисплея.

// Байт, отправляемый в дисплей при инициализации
#define LCD_INIT_BYTE 				0x30
// Признак конца инициализации - байт, который надо поместить 
// в буфер после байтов инициализации.
#define LCD_INIT_END				0x00

// Режимы передачи данных в дисплей
#define LCD_COMMAND_MODE			0x00
#define LCD_DATA_MODE				0x01

// Включение 4-битного режима
#define LCD_DATA_MODE_4_BITS 		0x20

// Дисплей 2-строчный, размер символов 5x8.
#define LCD_2_LINES					0x08

// Команда включения экрана и курсора
#define LCD_COMMAND_DISPLAY			0x08

// Команда стирания экрана
#define LCD_CLEAR_COMMAND			0x01

// Включение экрана
#define LCD_ON						0x04

// Включение курсора (символ подчеркивания)
#define LCD_CURSOR_ON				0x02

// Включение мигания курсора
#define LCD_BLINK_ON				0x01

// Сброс (объявление для разных целей).
#define LCD_RESET					0x0000

// Состояние буфера
#define LCD_BUFFER_FULL				0x01 // Буфер вывода заполнен.
#define LCD_BUFFER_EMPTY			0x02 // Буфер вывода пуст.

// Состояние дисплея
#define LCD_INIT					0x01 // Выполняется инициализация дисплея
#define LCD_NEXT_BYTE_DATA			0x02 // Следующий байт будет данными.
#define LCD_SEND_COMMAND			0x04 // Отправка команды.
#define LCD_LONG_COMMAND			0x08 // Отправка длительной команды (clear, set position).
#define LCD_FIRST_NIBBLE			0x10 // Отправка первого полубайта.

// Код символа, указывающий, что начинаются команды.
#define LCD_ESC_CODE				0x1B

// NIBBLE - 4 бита. Здесь используется старший ниббл.
#define LCD_NIBBLE					0xF0

#endif /* __WH160x_DEFINES */
