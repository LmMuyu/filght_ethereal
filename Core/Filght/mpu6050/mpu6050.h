#ifndef _HAL_MPU6050_
#define _HAL_MPU6050_

#ifdef __cplusplus

extern "C"
{
#endif

#include "main.h"
#include "stdlib.h"
#include "hal_i2c.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "usb.h"
#include "freertos_task.h"
#include "flight_math.h"
#include "math.h"
#include "stdbool.h"
#include "mahony.h"
#include "vofa.h"
#include "motor_control.h"

#define _Debugger_ 1

// i2c_gpio
#define I2C_REGISTER GPIOB
#define PIN_SCL GPIO_PIN_6
#define PIN_SDA GPIO_PIN_7

// 读写地址
#define MPU6050_ADDR 0x68 // 地址

// 频率寄存器
#define Freq_Reg 0x19 // 频率寄存器
#define Freq 0x07     // 频率值

// 过滤器配置
#define Filter_Reg 0x1a // 过滤器地址
#define Filter 0x03     // 值

// FIFO
#define FIFO_EN_Reg 0x23
#define FIFO_EN 0x70

// 加速度测量值
#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40

// 陀螺仪测量值
#define GYRO_XOUT_H 0x43
#define GYRO_XOUT_L 0x44
#define GYRO_YOUT_H 0x45
#define GYRO_YOUT_L 0x46
#define GYRO_ZOUT_H 0x47
#define GYRO_ZOUT_L 0x48

// 温度传感器
#define TEMP_OUT_H 0x41
#define TEMP_OUT_L 0x42

// 配置地址
#define MPU_SETADDRI 0x75
#define MPU_ADDRI 0x68

// 电源管理1
#define PWR_MGMT_1_Reg 0x6B
#define PWR_MGMT 0x00

// 陀螺仪配置
#define Gcfg_Reg 0x1B
#define Gcfg_V 0x18
#define GYRO_LSB (Gcfg_V == 0x00 ? 131.0f : Gcfg_V == 0x08 ? 65.5f \
                                        : Gcfg_V == 0x10   ? 32.8f \
                                                           : 16.4f)

// 加速度配置
#define Acfg_Reg 0x1C
#define Acfg_V 0x10
#define ACCLE_LSB (Acfg_V == 0x00 ? 16384.0f : Acfg_V == 0x08 ? 8192.0f \
                                           : Acfg_V == 0x10   ? 4096.0f \
                                                              : 2048.0f)

#define Accle_LSB 4096.0f
/**FreeRtos*/
#define STACK_SIZE 512

/**serial*/
#define SERIAL_SIZE 128
#define MPU_DATA_SIZE 6

  extern const char *const Task_Name;
  extern TaskHandle_t xHandle;

  /**i2c*/
  extern i2c_instance_methods_t *hi2c_example;

  typedef struct Gyro_Type
  {
    uint8_t gyro_lists[MPU_DATA_SIZE];
    float gx, gy, gz;
  } gyro_type_t;

  typedef struct Accel_Type
  {
    uint8_t accel_lists[MPU_DATA_SIZE];
    int16_t ax, ay, az;
    float module_len;
  } accel_type_t;

  typedef struct Mpu6050_Type
  {
    char serial_port_list[SERIAL_SIZE];
    gyro_type_t *gyro;
    accel_type_t *accle;
  } mpu6050_type_t;

  /**utils Function*/
  void Mpu6050_Task_Handle(void);

  void Gyroscope_WatchData(uint8_t data[6]);

#ifdef __cplusplus
}
#endif

#endif
