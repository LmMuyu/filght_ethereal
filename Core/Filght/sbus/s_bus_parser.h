#ifndef _HAL_SBUS_PARSER_H_
#define _HAL_SBUS_PARSER_H_
#ifdef __cplusplus
extern "C"
{
#endif
#include "main.h"
#include "s_bus.h"
#include "string.h"

#define CHANNLE_SIZE 16

  typedef struct s_bus_parser
  {
    uint16_t channel[CHANNLE_SIZE];
  } sbus_channel_t;

  extern sbus_protocol_frame_t s_bus_frame;
  extern sbus_channel_t channel;

  void Start_Sbus_Parser();
  void Sbus_to_motor_pluse();

#ifdef __cplusplus
}
#endif
#endif