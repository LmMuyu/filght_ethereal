#include "hal_qmc5883.h"
#include "qmc5883.h"

void Qmc5883_freertos_task(void *args);

TaskHandle_t qmc5883_TaskHandle_t;
StackType_t qmc5883_Stack[Qmc_Stack_Size];
StaticTask_t qmc5883_Type;

void qmc5883_Init_Handle_Task()
{
  qmc5883_TaskHandle_t = xTaskCreateStatic(
      Qmc5883_freertos_task,
      "qmc5883",
      Qmc_Stack_Size,
      NULL,
      Task_Priority_Level_10,
      qmc5883_Stack,
      &qmc5883_Type);
}

void Qmc5883_freertos_task(void *args)
{
  qmc5883_pdata_t qmc_pdata;
  QMC5883L_Initialize(MODE_CONTROL_CONTINUOUS, OUTPUT_DATA_RATE_200HZ, FULL_SCALE_8G, OVER_SAMPLE_RATIO_64);

  char pdata[16];

  while (1)
  {
    QMC5883L_Read_Data(&qmc_pdata.magx, &qmc_pdata.magy, &qmc_pdata.magz);
    qmc_pdata.parse_mag = QMC5883L_Heading(qmc_pdata.magx, qmc_pdata.magy, qmc_pdata.magz);

    sprintf(pdata, "mag:%f\n", qmc_pdata.parse_mag);
    Hal_Write_Buf(pdata);
    Hal_SendData();

    QMC5883L_ResetCalibration();

    vTaskDelay(pdMS_TO_TICKS(15));
  }
}
