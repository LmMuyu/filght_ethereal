#include "vofa.h"

struct Vofa vofa = {
    .tail = {0, 0, 0x80, 0x7f},
};

void Vofa_Just_Float(float ch[3])
{
  uint32_t vofa_bound_toal = 12;
  uint32_t ch_len = vofa_bound_toal / (uint32_t)sizeof(float);

  for (size_t i = 0; i < ch_len; i++)
  {
    Vofa_Protocol_Group(vofa.ch_vofa, ch[i], i * (size_t)sizeof(float));
  }

  vofa.ch_vofa[12] = vofa.tail[0];
  vofa.ch_vofa[13] = vofa.tail[1];
  vofa.ch_vofa[14] = vofa.tail[2];
  vofa.ch_vofa[15] = vofa.tail[3];

  Hal_Write_Float_Buffer((char *)vofa.ch_vofa, (int)sizeof(vofa.ch_vofa));
  Hal_SendData();
}

void Vofa_Fire_Water(char *pdata)
{
  Hal_Write_Buf(pdata);
  Hal_SendData();
}

void Vofa_Protocol_Group(char *protocol_group, float f_v, uint8_t index)
{
  vofa_just_float_t f_pdata;
  f_pdata.fv = f_v;
  memcpy(protocol_group + index, f_pdata.pdata, sizeof(float));
}