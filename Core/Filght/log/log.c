#include "log.h"

uint8_t *ErrorFileInfo(uint8_t *file, uint32_t line);

uint8_t *ErrorFileInfo(uint8_t *file, uint32_t line)
{
  char *errorpos = "错误地址：%s文件，第%d行";

  Hal_Write_Buf(errorpos);
  Hal_SendData();

  return (uint8_t *)errorpos;
}
