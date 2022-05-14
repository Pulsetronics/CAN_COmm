#ifndef __CAN_LIB_H
#define __CAN_LIB_H


#include "main.h"
#include "can.h"
#include "enums.h"
#include "structures.h"
#include "dr_devices.h"


void CAN_Filter_IDList(uint32_t U32_receiveCANid1, TypeofCANID U8_idType, FilterBanks_CAN2 U8_filterbank, FIFOs_CAN2 U8_fifo);
void Transmit_on_CAN(uint32_t U32_transmitCANid, TypeofCANID U8_idType, uint8_t *U8_dataarr, uint8_t U8_DLC);
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs);
void CAN_Write(void);
FDCAN_STATUS isFDCAN_Available(void);
void FDCAN_ApplicationSetup (void);
#endif