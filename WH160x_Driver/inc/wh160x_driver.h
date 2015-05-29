#ifndef __WH160x_DRIVER
#define __WH160x_DRIVER

#include "wh160x_include_libraries.h"

/**
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * Перед использованием дисплея необходимо определить
 * выводы портов, к которым будет подключен дисплей
 * в файле wh1602_port_def.h.
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

// Функции управления
/**
 * Конфигурирование отдельного вывода порта контроллера.
 * @param GPIOx конфигурируемый порт
 * @param pin конфигурируемый вывод
 * @param mode режим работы вывода
 * @param speed скорость работы вывода
 */
void GPIO_Config(GPIO_TypeDef* GPIOx,
				uint16_t pin,
				GPIOMode_TypeDef mode,
				GPIOSpeed_TypeDef speed);

/**
 * Инциализация дисплея.
 * @param cursorOn включение курсора
 * @param cursorBlink режим курсора
 * @return код ошибки
 */
uint8_t LCD_Init(uint8_t cursorOn,
			uint8_t cursorBlink);

// Функции вывода
/**
 * Очистка дисплея
 */
void LCDClean(void);

/**
 * Позиционирование курсора
 * @param row строка
 * @param col колонка
 */
void LCD_SetCursorPos(uint8_t row, uint8_t col);

/**
 * Вывод команды
 * @param command выводимая команда
 */
void LCD_PrintCommand(unsigned char command);

/**
 * Вывод байта в позицию курсора.
 * @param byte выводимый байт
 * @return код ошибки
 */
uint8_t LCD_PrintByte(unsigned char byte);

/**
 * Вывод символа в позицию курсора.
 * Символ преобразуется в код дисплея через таблицу
 * преобразования LCD_charTable[].
 * @param c выводимый символ
 * @return код ошибки
 */
uint8_t LCD_PrintChar(unsigned char c);

/**
 * Вывод строки.
 * Строка должна заканчиваться \0. Вывод продолжается
 * пока вся строка не будет помещена в буфер.
 * @param string выводимая строка
 */
void LCD_PrintString(unsigned char *string);

/*
 * Вывод числа в шестнадцатиричном формате 0xNNNN.
 * Если длина числа превышает заданное количество символов,
 * старшие разряды отбрасываются.
 * @param number - выводимое число.
 * @param length - количество символов после 0x.
 */
void LCD_PrintHex(long number, uint8_t length);

#endif /* __WH160x_DRIVER */
