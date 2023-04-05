
#ifndef _HAL_QMC5883_PROJCE_H_
#define _HAL_QMC5883_PROJCE_H_
#ifdef __cplusplus
extern "C"
{
#endif
#include "main.h"
#include "task.h"
#include "freertos_task.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "usb.h"

#define Qmc_Stack_Size configMINIMAL_STACK_SIZE // FreeRtos  Stack Size

  typedef struct QMC5883_PDATA
  {
    int16_t magx;
    int16_t magy;
    int16_t magz;
    float parse_mag;
  } qmc5883_pdata_t;

  void qmc5883_Init_Handle_Task();

#ifdef __cplusplus
}
#endif
#endif