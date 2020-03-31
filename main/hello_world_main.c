/*
Written by Tom Kaasenbrood
Commented lines: 
Code Lines:
*/
//general includes
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "driver/gpio.h"
#include "driver/i2c.h"
//components
#include "smbus.h"
#include "i2c-lcd1602.h"
#include "mcp23017.h"

//I2C
#define I2C_MASTER_NUM I2C_NUM_0
#define I2C_MASTER_TX_BUF_LEN 0
#define I2C_MASTER_RX_BUF_LEN 0
#define I2C_MASTER_FREQ_HZ 20000
#define I2C_MASTER_SDA_IO 18
#define I2C_MASTER_SCL_IO 23
//LCD screen
#define LCD_Address 0x27
#define LCD_NUM_ROWS 4
#define LCD_NUM_COLUMNS 40
#define LCD_NUM_VIS_COLUMNS 20

i2c_lcd1602_info_t *lcd_info;

//initialize i2c master
static void i2c_master_init(void)
{
    int i2c_master_port = I2C_MASTER_NUM;
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_MASTER_SDA_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = I2C_MASTER_SCL_IO;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
    i2c_param_config(i2c_master_port, &conf);
    i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_LEN, I2C_MASTER_TX_BUF_LEN, 0);
}

// WARNING: ESP32 does not support blocking input from stdin yet, so this polls
// the UART and effectively hangs up the SDK.
static uint8_t _wait_for_user(void)
{
    uint8_t c = 0;
#ifdef USE_STDIN
    while (!c)
    {
        STATUS s = uart_rx_one_char(&c);
        if (s == OK)
        {
            printf("%c", c);
        }
    }
#else
    vTaskDelay(1000 / portTICK_RATE_MS);
#endif
    return c;
}

void lcd1602_task(void *pvParameter)
{
    smbus_info_t *smbus_info = smbus_malloc();
    smbus_init(smbus_info, I2C_MASTER_NUM, LCD_Address);
    smbus_set_timeout(smbus_info, 1000 / portTICK_RATE_MS);

    //setup screen
    lcd_info = i2c_lcd1602_malloc();
    i2c_lcd1602_init(lcd_info, smbus_info, true, LCD_NUM_ROWS, LCD_NUM_COLUMNS, LCD_NUM_VIS_COLUMNS);
    _wait_for_user();
    i2c_lcd1602_set_display(lcd_info, true);
    i2c_lcd1602_clear(lcd_info);
    i2c_lcd1602_move_cursor(lcd_info, 0, 0);
    i2c_lcd1602_clear(lcd_info);

    //draw initial screen
    i2c_lcd1602_move_cursor(lcd_info, 0, 0);
    i2c_lcd1602_write_string(lcd_info, "Het weer");
    i2c_lcd1602_move_cursor(lcd_info, 0, 1);
    i2c_lcd1602_write_string(lcd_info, "Temperatuur:");
    i2c_lcd1602_move_cursor(lcd_info, 0, 2);
    i2c_lcd1602_write_string(lcd_info, "20C");
    i2c_lcd1602_move_cursor(lcd_info, 0, 3);
    i2c_lcd1602_write_string(lcd_info, "-----");
    vTaskDelete(NULL);
}

void app_main()
{
    i2c_master_init();
    xTaskCreate(&lcd1602_task, "LCD_task", 1024 * 2, NULL, 1, NULL);
}
