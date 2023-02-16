#include "st7789.h"

void init_lcd(st7789_t *lcd)
{
	printf("LCD Configuration:\n");
	printf(
		"MOSI %d, SCLK: %d, RST: %d, DC: %d CS: %d\n", 
		CONFIG_LCD_MOSI_GPIO, CONFIG_LCD_SCLK_GPIO, 
		CONFIG_LCD_RESET_GPIO, CONFIG_LCD_DC_GPIO, 
		CONFIG_LCD_CS_GPIO
	);

	printf(
		"%dx%d px | %d MHz | Mode: %d | Inverted: %s | Color format: %s\n", 
		CONFIG_LCD_VER_RES, CONFIG_LCD_HOR_RES, 
		CONFIG_LCD_SPI_FREQUENCY, LCD_SPI_MODE,
		LCD_INVERT ? "y" : "n",
		LCD_COLOR_FORMAT == LCD_RGB_ENDIAN_BGR ? "BGR" : "RGB"
	);

	lcd->width = CONFIG_LCD_HOR_RES;
	lcd->height = CONFIG_LCD_VER_RES;
	lcd->buff_size = sizeof(uint16_t) * (CONFIG_LCD_HOR_RES * CONFIG_LCD_VER_RES);
    lcd->disp_buff = heap_caps_malloc(lcd->buff_size, MALLOC_CAP_DMA);

	if (lcd->disp_buff) {
		printf("Display buffer initialized\n");
	}

	spi_bus_config_t buscfg = {
		.sclk_io_num = CONFIG_LCD_SCLK_GPIO,
		.mosi_io_num = CONFIG_LCD_MOSI_GPIO,
		.miso_io_num = -1,
		.quadwp_io_num = -1,
		.quadhd_io_num = -1,
		.max_transfer_sz = CONFIG_LCD_VER_RES * CONFIG_LCD_HOR_RES * 2 + 8};

	// Initialize the SPI bus
	ESP_ERROR_CHECK(spi_bus_initialize(LCD_HOST, &buscfg, SPI_DMA_CH_AUTO));

	esp_lcd_panel_io_spi_config_t io_config = {
		.dc_gpio_num = CONFIG_LCD_DC_GPIO,
		.cs_gpio_num = CONFIG_LCD_CS_GPIO,
		.pclk_hz = CONFIG_LCD_SPI_FREQUENCY * 1000 * 1000,
		.lcd_cmd_bits = 8,
		.lcd_param_bits = 8,
		.spi_mode = LCD_SPI_MODE,
		.trans_queue_depth = 10,
	};

	// Attach the LCD to the SPI bus
	ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_HOST, &io_config, &lcd->io_handle));

	esp_lcd_panel_dev_config_t panel_config = {
		.reset_gpio_num = CONFIG_LCD_RESET_GPIO,
		.rgb_endian = LCD_COLOR_FORMAT,
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

	if (LCD_INVERT) {
		// Invert display
		ESP_ERROR_CHECK(esp_lcd_panel_invert_color(lcd->panel_handle, true));
	}

	// Turn display on
	ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(lcd->panel_handle, true));
}

void fill_screen(st7789_t *lcd, uint16_t color) {
	if (LCD_SWAP_BITS) {
		color = swap_bits(color);
	}
	memset16(lcd->disp_buff, color, lcd->buff_size);
	esp_lcd_panel_draw_bitmap(lcd->panel_handle, 0, 0, lcd->width, lcd->height, lcd->disp_buff);
}