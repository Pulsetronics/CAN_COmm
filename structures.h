#ifndef __STRUCTURES_H
#define __STRUCTURES_H


#include "main.h"
#include "enums.h"
#include <stdbool.h>


typedef struct can_t {
        uint8_t      txMsg[10][8];
        uint8_t      rxMsg[8];
        void         (*transmit) (uint32_t U32_transmitCANid, TypeofCANID U8_idType, uint8_t *U8_dataarr, uint8_t U8_DLC);
        void         (*write) (void);
        void         (*setup) (void);
        FDCAN_STATUS (*available) (void);
        FDCAN_RxHeaderTypeDef RxMessageBuf;
}can_t;



#endif