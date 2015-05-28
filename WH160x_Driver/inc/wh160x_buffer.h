#ifndef __WH160x_BUFFER
#define __WH160x_BUFFER

#include "wh160x_include_libraries.h"

/**
 * Помещение символа в буфер.
 * @param c символ
 * @return SUCCESSFUL в случае успешного помещения,
 * 		   BUFFER_FULL, если буфер заполнен.
 */
uint8_t LCD_BufferPush(unsigned char c);

/**
 * Извлечение символа из буфера.
 * @param *c указатель на переменную, в которую будет помещен
 * 			извлеченный символ
 * @return SUCCESSFUL в случае успешного извлечения,
 * 		   BUFFER_EMPTY, если буфер пуст.
 */
uint8_t LCD_BufferPop(unsigned char *c);

#endif /* __WH160x_BUFFER */
