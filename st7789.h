#ifndef ST7789_H_
#define ST7789_H_

#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_heap_caps.h"
#include "driver/spi_master.h"
#include "functions.h"

#if defined(CONFIG_LCD_SPI2_HOST)
#define LCD_HOST CONFIG_LCD_SPI2_HOST
#else
#define LCD_HOST CONFIG_LCD_SPI3_HOST
#endif

#if defined(CONFIG_LCD_SPI_MODE_DIO)
#define LCD_SPI_MODE SPI_TRANS_MODE_DIO
#else
#define LCD_SPI_MODE SPI_TRANS_MODE_QIO
#endif

#if defined(CONFIG_LCD_COLOR_FORMAT_RGB)
#define LCD_COLOR_FORMAT LCD_RGB_ENDIAN_RGB
#else
#define LCD_COLOR_FORMAT LCD_RGB_ENDIAN_BGR
#endif

#if defined(CONFIG_LCD_INVERT)
#define LCD_INVERT 1
#else
#define LCD_INVERT 0
#endif

#if defined(CONFIG_LCD_SWAP_BITS)
#define LCD_SWAP_BITS 1
#else
#define LCD_SWAP_BITS 0
#endif

typedef struct
{
	esp_lcd_panel_io_handle_t io_handle;
	esp_lcd_panel_handle_t panel_handle;
	int buff_size;
	uint16_t *disp_buff;
	uint16_t width;
	uint16_t height;
} st7789_t;

void init_lcd(st7789_t *);
void fill_screen(st7789_t *lcd, uint16_t color);

#endif /* MAIN_SSD1306_H_ */
