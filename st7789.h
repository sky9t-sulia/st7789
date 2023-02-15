#ifndef MAIN_SSD1306_H_
#define MAIN_SSD1306_H_

#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_heap_caps.h"
#include "driver/spi_master.h"

#define PARALLEL_LINES 2

#if defined(CONFIG_SPI2_HOST)
#define LCD_HOST CONFIG_SPI2_HOST
#elif defined(CONFIG_SPI3_HOST)
#define LCD_HOST CONFIG_SPI3_HOST
#endif

typedef struct
{
	esp_lcd_panel_io_handle_t io_handle;
	esp_lcd_panel_handle_t panel_handle;
} st7789_t;

void init_lcd(st7789_t *);

#endif /* MAIN_SSD1306_H_ */
