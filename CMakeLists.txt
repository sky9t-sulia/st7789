set(component_srcs)

idf_component_register(SRCS "st7789.c" "functions.c" "${component_srcs}"
                       PRIV_REQUIRES esp_lcd driver
                       INCLUDE_DIRS ".")
