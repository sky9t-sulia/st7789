#include "st7789.h"

void init_lcd(st7789_t *lcd)
{
	spi_bus_config_t buscfg = {
		.sclk_io_num = CONFIG_SCLK_GPIO,
		.mosi_io_num = CONFIG_MOSI_GPIO,
		.miso_io_num = -1,
		.quadwp_io_num = -1,
		.quadhd_io_num = -1,
		.max_transfer_sz = LCD_VER_RES * LCD_HOR_RES * 2 + 8};

	// Initialize the SPI bus
	ESP_ERROR_CHECK(spi_bus_initialize(LCD_HOST, &buscfg, SPI_DMA_CH_AUTO));

	esp_lcd_panel_io_spi_config_t io_config = {
		.dc_gpio_num = CONFIG_DC_GPIO,
		.cs_gpio_num = CONFIG_CS_GPIO,
		.pclk_hz = 40000000,
		.lcd_cmd_bits = 8,
		.lcd_param_bits = 8,
		.spi_mode = 0,
		.trans_queue_depth = 10,
	};

	// Attach the LCD to the SPI bus
	ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_HOST, &io_config, &lcd->io_handle));

	esp_lcd_panel_dev_config_t panel_config = {
		.reset_gpio_num = CONFIG_RESET_GPIO,
		.rgb_endian = LCD_RGB_ENDIAN_RGB,
		.bits_per_pixel = 16,
	};
	// Initialize the LCD configuration
	ESP_ERROR_CHECK(esp_lcd_new_panel_st7789(lcd->io_handle, &panel_config, &lcd->panel_handle));

	// Reset the display
	ESP_ERROR_CHECK(esp_lcd_panel_reset(lcd->panel_handle));

	// Initialize LCD panel
	ESP_ERROR_CHECK(esp_lcd_panel_init(lcd->panel_handle));

	// Landscape view
	ESP_ERROR_CHECK(esp_lcd_panel_swap_xy(lcd->panel_handle, true));

	// Invert display
	ESP_ERROR_CHECK(esp_lcd_panel_invert_color(lcd->panel_handle, true));

	// Turn display on
	ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(lcd->panel_handle, true));
}