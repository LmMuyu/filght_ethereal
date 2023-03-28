#include "bmp280_task.h"

void Bmp280_freertos_task(void *args);

const char *bmp280_task_name = "bmp280";
TaskHandle_t Bmp280_TaskHandle_t;
StackType_t Bmp280_Stack[Stack_Size];
StaticTask_t Bmp280_Type;

void Bmp280_Init_Handle_Task()
{
  Bmp280_TaskHandle_t = xTaskCreateStatic(Bmp280_freertos_task, bmp280_task_name, Stack_Size, NULL, Task_Priority_Level_6, Bmp280_Stack, &Bmp280_Type);
}

void Bmp280_freertos_task(void *args)
{
  uint16_t size;
  uint8_t Data[64];
  uint32_t Timeout = Rx_Timeout * 10;
  bmp280_read_data_t bmp_data;
  i2c_instance_methods_t *i2c_methods;

  BMP280_HandleTypedef bmp280;

  i2c_methods = I2c_CreateMethods();

  bmp280_init_default_params(&bmp280.params);
  bmp280.addr = BMP280_I2C_ADDRESS_0;
  bmp280.i2c = i2c_methods->i2c_example->hi2c;

  while (!bmp280_init(&bmp280, &bmp280.params))
  {
    return;
  }

  bool bme280p = bmp280.id == BME280_CHIP_ID;
  int32_t correction_sec = 300;
  queue_node_t bmp_node;
  bmp_node.pdata = (float *)malloc(sizeof(float));

  while (1)
  {
    while (HAL_I2C_GetState(bmp280.i2c) == HAL_I2C_STATE_BUSY)
    {
      if (Timeout <= 0)
        break;

      Timeout--;
    }

    if (Timeout <= 0 || !bmp280_read_float(&bmp280, &bmp_data.temperature, &bmp_data.pressure, &bmp_data.humidity))
    {
      Hal_Write_Buf("bmp280_read_float Read Error\r\n");
      Hal_SendData();

      Timeout = Rx_Timeout * 10;
      continue;
    }

    Timeout = Rx_Timeout * 10;

    float Height = Height_Computed(&bmp_data);

    if (correction_sec > 0)
    {
      correction_sec -= 1;

      if (correction_sec == 0)
      {
        bmp_node.pdata = &Height;
        strncat((char *)bmp_node.tag, (char *)bmp_tag, 1);
        Add_Queue_Node(&bmp_node);
      }

      continue;
    }

    bmp_node.pdata = &Height;

    // size = sprintf((char *)Data, "Height:%.2f\n", Height);

    // Hal_Write_Buf((char *)Data);
    // Hal_SendData();

    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

float Height_Computed(bmp280_read_data_t *bmp_data)
{
  /*
   h =[(p_0 / p)^1/5.257 - 1]  * (  T + 273.15 ) /  0.0065
  */

  /**
    P_0为标准大气压强，取值 101.325 kPa 101.325 {\rm kPa}101.325kPa；P PP为实际测量的大气压强，单位 k P a {\rm kPa}kPa；T TT为实际测量温度，单位 ℃。
    上式中( T + 273.15 )是将摄氏度转化为华氏度。该公式同时考虑为温度和压强计算海拔高度。
  */

  static float height;

  bmp_data->pressure = bmp_data->pressure / 1000.0f;

  float Pressure_Diff = StandardAtmosphere / bmp_data->pressure;

  float left_value = pow(Pressure_Diff, Standar_Norm_Value) - 1;
  float Celsius_To_Fahrenheit = bmp_data->temperature + 273.15;

  height = left_value * Celsius_To_Fahrenheit / 0.0065;

  return height;
}