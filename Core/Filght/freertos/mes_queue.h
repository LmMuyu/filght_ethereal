#ifndef _HAL_COMMON_MES_QUEUE_H_
#define _HAL_COMMON_MES_QUEUE_H_
#ifdef __cplusplus
extern "C"
{
#endif

  typedef enum queue_tag
  {
    MOTOR_TAG = 0x00,
    bmp_tag = 0x01,
    UART_2_TAG = 0x02,
    UART_2_RECE_ALL_TAG = 0x03,
    OTHER_TAG = 0xff,
  } queue_tag_t;

#ifdef __cplusplus
}
#endif
#endif