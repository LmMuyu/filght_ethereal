#pragma once

#define Rx_Max_Size 128

typedef struct Rx_Struct
{
  uint16_t max_size;
  uint16_t frame;
  uint8_t rx_store[Rx_Max_Size];
} rx_struct_t;
