
#ifndef _HAL_BMP280_TASK_
#define _HAL_BMP280_TASK_
#ifdef __cplusplus
extern "C"
{
#endif
#include "main.h"
#include "task.h"
#include "bmp280.h"
#include "string.h"
#include "hal_i2c.h"
#include "FreeRTOS.h"
#include "freertos_task.h"
#include "FreeRTOSConfig.h"
#include "freertos_mes_queue.h"

#define StandardAtmosphere 101.325f // 标准大气压(KPa)
#define Standar_Norm_Value 0.19022256f

#define Stack_Size 256 // FreeRtos  Stack Size

  typedef struct Bmp280_Read_Data
  {
    float pressure;    // 压强
    float temperature; // 温度
    float humidity;    // 湿度
  } bmp280_read_data_t;

  extern StackType_t Bmp280_Stack[Stack_Size];
  extern StaticTask_t Bmp280_Type;

  extern const char *bmp280_task_name;
  extern TaskHandle_t Bmp280_TaskHandle_t;

  /**
   * bmp280 FreeRtos Task
   */

  /**
   * 创建BMP280的freertos服务
   */

  void Bmp280_Init_Handle_Task(void);
  float Height_Computed(bmp280_read_data_t *bmp_data);

#ifdef __cplusplus
}
#endif
#endif