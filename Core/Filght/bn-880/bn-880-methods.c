#include "bn-880-methods.h"

void gps_pdata_clear(gps_struct_ptr gps_rx_pdata);
bool Gps_Parser(gps_struct_ptr gps_ptr, uint8_t str);

gps_datastruct_t *Create_Gps_Parser_Struct()
{
  static gps_datastruct_t gps_struct;

  gps_struct.start_symbol = false;
  gps_struct.Gs_Parser = Gps_Parser;
  gps_struct.clear = gps_pdata_clear;
  gps_struct.index = 0;

  return &gps_struct;
}

bool Gps_Parser(gps_struct_ptr gps_ptr, uint8_t str)
{
  volatile bool flag = false;
  bool is_start_symbol = strcmp((const char *)&str, "$") == 0 ? true : false;

  if (gps_ptr->index + 1 > RX_PDAT_SIZE)
  {
    gps_ptr->index = RX_PDAT_SIZE - 1;
  }

  if (is_start_symbol && (gps_ptr->start_symbol != true))
  {
    gps_ptr->gps_pdata[gps_ptr->index++] = (uint8_t)str;
    gps_ptr->start_symbol = true;
  }
  // 第二次接收到"$",开始组合字符串
  else if (is_start_symbol && (gps_ptr->start_symbol == true))
  {
    gps_ptr->start_symbol = false;
    flag = true;
  }
  else
  {
    *(gps_ptr->gps_pdata + gps_ptr->index) = str;
    gps_ptr->index += 1;
  }

  return flag;
}

void gps_pdata_clear(gps_struct_ptr gps_rx_pdata)
{
  memset(gps_rx_pdata->gps_pdata, 0x00, gps_rx_pdata->index);
  gps_rx_pdata->index = 0;
  gps_rx_pdata->gps_pdata[gps_rx_pdata->index++] = '$';
  gps_rx_pdata->start_symbol = true;
}