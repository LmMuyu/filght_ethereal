#ifndef _HAL_I2C_H_
#define _HAL_I2C_H_
#include "main.h"
#include "stm32f3xx_hal_i2c.h"
#include "usb.h"
#include "string.h"
#include "stdio.h"
#include "task.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "stdarg.h"

#define I2C_SCL GPIO_PIN_6
#define I2C_SDA GPIO_PIN_7
#define I2C_REG GPIOB

#define I2C1_ENABLE __HAL_RCC_GPIOB_CLK_ENABLE()
#define I2C1_DISABLE __HAL_RCC_GPIOB_CLK_DISABLE()

#define LEN(a, b) ((b / a))

#define Rx_Timeout 100
#define Tx_Timeout 100

#define Create_Mutex_handle() ((QueueHandle_t)xSemaphoreCreateMutex())

typedef struct I2c_Instance_Methods i2c_instance_methods_t;

#define I2C1_400khz 0x00E0257A // 400 Khz, 72Mhz Clock, Analog Filter Delay ON, Rise 100, Fall 10.
#define I2C1_200khz 0x2000090E

extern volatile uint8_t i2cIndex;
extern i2c_instance_methods_t *hi2c_instance_mts;
extern I2C_HandleTypeDef hi2c;

typedef enum Multiple_Data
{
    Multiple = 0x00U,
    NMultiple = 0x01U,
} multiple_data_t;

typedef struct I2c_Example
{
    I2C_HandleTypeDef *hi2c;
    multiple_data_t multiple; // i2c是否要修改寄存器的值
} i2c_example_t;

typedef struct I2c_Instance_Methods
{
    i2c_example_t *i2c_example;

    HAL_StatusTypeDef (*I2c_WriteData)(
        uint16_t slave_write_addr,
        uint8_t slave_reg,
        uint8_t *data,
        uint16_t len);

    HAL_StatusTypeDef (*I2c_ReadData)(
        uint16_t slave_read_addr,
        uint8_t *recv_buf, uint16_t len);

    HAL_StatusTypeDef (*I2c_Mem_ReadData)(
        uint8_t slave_addr,
        uint8_t mem_addr,
        uint8_t *recv_buf,
        uint16_t readlen);

    void (*I2c_Port_DeInit)();

} i2c_instance_methods_t;

i2c_instance_methods_t *I2c_CreateMethods(void);

/// @brief 初始化i2c1
/// @param
void I2c_Init(void);

/// @brief 初始化i2c1接口
/// @param hi2c
void I2c_Gpio_Init();

/// @brief 删除连接i2c的gpio接口
/// @param hi2c
void I2c_Port_DeInit();

HAL_StatusTypeDef I2c_WriteData(uint16_t slave_write_addr, uint8_t slave_reg, uint8_t *data, uint16_t len);
HAL_StatusTypeDef I2c_ReadData(uint16_t slave_read_addr, uint8_t *recv_buf, uint16_t len);
HAL_StatusTypeDef I2c_Mem_ReadData(
    uint8_t slave_addr,
    uint8_t mem_addr,
    uint8_t *recv_buf,
    uint16_t readlen);

uint8_t *Assem_Pdata(const int hi2c_reg_data, ...);

#endif
