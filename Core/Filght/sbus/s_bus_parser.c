#include "s_bus_parser.h"
#include "usb.h"

sbus_protocol_frame_t s_bus_frame = {0};
sbus_channel_t channel = {0};

char pdata_uart3_patser[84];

void CallbackFn(sbus_protocol_frame_t *frame);

void Start_Sbus_Parser()
{
  Sbus_Init();
  Sbus_Pdata_Rx_Start(&s_bus_frame, CallbackFn);
}

void CallbackFn(sbus_protocol_frame_t *frame)
{
  channel.channel[0] = ((int16_t)frame->protocol_frame[1] >> 0 | ((int16_t)frame->protocol_frame[2] << 8)) & 0x07FF;
  channel.channel[1] = ((int16_t)frame->protocol_frame[2] >> 3 | ((int16_t)frame->protocol_frame[3] << 5)) & 0x07FF;
  channel.channel[2] = ((int16_t)frame->protocol_frame[3] >> 6 | ((int16_t)frame->protocol_frame[4] << 2) | (int16_t)frame->protocol_frame[5] << 10) & 0x07FF;
  channel.channel[3] = ((int16_t)frame->protocol_frame[5] >> 1 | ((int16_t)frame->protocol_frame[6] << 7)) & 0x07FF;
  channel.channel[4] = ((int16_t)frame->protocol_frame[6] >> 4 | ((int16_t)frame->protocol_frame[7] << 4)) & 0x07FF;
  channel.channel[5] = ((int16_t)frame->protocol_frame[7] >> 7 | ((int16_t)frame->protocol_frame[8] << 1) | (int16_t)frame->protocol_frame[9] << 9) & 0x07FF;
  channel.channel[6] = ((int16_t)frame->protocol_frame[9] >> 2 | ((int16_t)frame->protocol_frame[10] << 6)) & 0x07FF;
  channel.channel[7] = ((int16_t)frame->protocol_frame[10] >> 5 | ((int16_t)frame->protocol_frame[11] << 3)) & 0x07FF;
  channel.channel[8] = ((int16_t)frame->protocol_frame[12] << 0 | ((int16_t)frame->protocol_frame[13] << 8)) & 0x07FF;
  channel.channel[9] = ((int16_t)frame->protocol_frame[13] >> 3 | ((int16_t)frame->protocol_frame[14] << 5)) & 0x07FF;
  channel.channel[10] = ((int16_t)frame->protocol_frame[14] >> 6 | ((int16_t)frame->protocol_frame[15] << 2) | (int16_t)frame->protocol_frame[16] << 10) & 0x07FF;
  channel.channel[11] = ((int16_t)frame->protocol_frame[16] >> 1 | ((int16_t)frame->protocol_frame[17] << 7)) & 0x07FF;
  channel.channel[12] = ((int16_t)frame->protocol_frame[17] >> 4 | ((int16_t)frame->protocol_frame[18] << 4)) & 0x07FF;
  channel.channel[13] = ((int16_t)frame->protocol_frame[18] >> 7 | ((int16_t)frame->protocol_frame[19] << 1) | (int16_t)frame->protocol_frame[20] << 9) & 0x07FF;
  channel.channel[14] = ((int16_t)frame->protocol_frame[20] >> 2 | ((int16_t)frame->protocol_frame[21] << 6)) & 0x07FF;
  channel.channel[15] = ((int16_t)frame->protocol_frame[21] >> 5 | ((int16_t)frame->protocol_frame[22] << 3)) & 0x07FF;

  sprintf(pdata_uart3_patser, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
          channel.channel[0],
          channel.channel[1],
          channel.channel[2],
          channel.channel[3],
          channel.channel[4],
          channel.channel[5],
          channel.channel[6],
          channel.channel[7],
          channel.channel[8],
          channel.channel[9],
          channel.channel[10],
          channel.channel[11],
          channel.channel[12],
          channel.channel[13],
          channel.channel[14],
          channel.channel[15],
          channel.channel[16]);
  Hal_Write_Buf(pdata_uart3_patser);
  Hal_SendData();

  memset(frame->protocol_frame, 0x00, SBUS_PROTOCOL_SIZE);
}