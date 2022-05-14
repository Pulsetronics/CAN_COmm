#include "can_lib.h"

can_t can = {
          .txMsg     = {{0},{0},{0},{0},{0},{0},{0},{0},{0},{0}},
		  .rx_flag   = false,
		  .verifier  = 0,
		  .write     = CAN_Write,
		  .transmit  = Transmit_on_CAN,
		  .available = isFDCAN_Available, 
		  .setup     = FDCAN_ApplicationSetup,
	
};

/* Function to Transmit CAN Ids on CAN1 Port
 *
 * Returns:
 * 		Void (Check Error Handler for Error Messages if any)
 *
 * Parameters:
 * 		U32_transmitCANid: 11-bit or 29-bit CAN ID for Transmitting
 * 		U8_idType: If U8_idType == 0, then Extended ID is transmitted. Else Standard ID.
 * 		U8_dataarr: CAN Data Bytes for transmission (Range: data_arr[0] to data_arr[7])
 * 		U8_DLC: Data Length Count (Range: 0 to 8). No CAN-FD support by CAN Transceiver.
 * */
void Transmit_on_CAN(uint32_t U32_transmitCANid, TypeofCANID U8_idType, uint8_t *U8_dataarr, uint8_t U8_DLC) {
	CAN_TxHeaderTypeDef	TxMsg;

	if(U8_idType == E)
	{
		TxMsg.ExtId = U32_transmitCANid;
		TxMsg.IDE = CAN_ID_EXT;
	}
	else if(U8_idType == S)
	{
		TxMsg.StdId = U32_transmitCANid;
		TxMsg.IDE = CAN_ID_STD;
	}

	TxMsg.RTR = CAN_RTR_DATA;
	TxMsg.DLC = U8_DLC;
	TxMsg.TransmitGlobalTime = DISABLE;

	HAL_CAN_AddTxMessage(CAN1Port, &TxMsg, U8_dataarr, (uint32_t*)CAN_TX_MAILBOX0);
	// uint32_t internalTickVal = 0;
	// internalTickVal = HAL_GetTick();
	// while(((hcan1.Instance->TSR & CAN_TSR_TXOK0) == RESET) && ((hcan1.Instance->TSR & CAN_TSR_TXOK1) == RESET) && ((hcan1.Instance->TSR & CAN_TSR_TXOK2) == RESET) && (HAL_GetTick() - internalTickVal < 2));

}

/* Function to configure Hardware CAN Filters by ID Filtering method. Function configured for CAN1 only.
 *
 * Returns:
 * 		Void (Check Error Handler for Error Messages if any)
 *
 * Parameters:
 * 		U32_receiveCANid1: 11-bit or 29-bit CAN ID for filtering (First ID)
 * 		U32_receiveCANid2: 11-bit or 29-bit CAN ID for filtering (Second ID)
 * 		U8_idType: If U8_idType == 0, then Extended ID is configured. Else Standard ID. Both IDs have to be of same type for the filter to work.
 * 		U8_filterbank: Selection of Filter Bank (Range: 0 to 13).  No overlap of filters should be there from ID Masking techniques if used.
 * 		U8_fifo: If U8_fifo = 0, FIFO0 is selected. If U8_fifo = 1, FIFO1 is selected.
 * 		FunctionalState: ENABLE to initialize and start filtering. DISABLE to deinit the filter and stop filtering.
 * */

void CAN_Filter_IDList(uint32_t U32_receiveCANid, uint32_t U32_receiveCANid2, TypeofCANID U8_idType, FilterBanks_CAN2 U8_filterbank, FIFOs_CAN2 U8_fifo) {
	CAN_FilterTypeDef sFilterConfig;

	sFilterConfig.FilterActivation = ENABLE;

	sFilterConfig.FilterBank           = (uint32_t)(U8_filterbank);	//32-bit MASK Filter Bank (Reference Manual, Pg. 1099, Fig. 388)
	sFilterConfig.SlaveStartFilterBank = (uint32_t)(U8_filterbank);
	sFilterConfig.FilterMode           = CAN_FILTERMODE_IDLIST;
	sFilterConfig.FilterScale          = CAN_FILTERSCALE_32BIT;
	if(U8_fifo == FIFO0_CAN1) {
		sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	}
	else if(U8_fifo == FIFO1_CAN1) {
		sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO1;
	}
	if(U8_idType == E) {
		sFilterConfig.FilterIdHigh     = (U32_receiveCANid1 & 0x1FFFE000)>>13;
		sFilterConfig.FilterIdLow      = ((U32_receiveCANid1 & 0x00001FFF)<<3)|0x04;
		sFilterConfig.FilterMaskIdHigh = (U32_receiveCANid2 & 0x1FFFE000)>>13;
		sFilterConfig.FilterMaskIdLow  = ((U32_receiveCANid2 & 0x00001FFF)<<3)|0x04;
	}
	else if(U8_idType == S) {
		sFilterConfig.FilterIdHigh     = U32_receiveCANid1 << 5;
		sFilterConfig.FilterIdLow      = 0;
		sFilterConfig.FilterMaskIdHigh = U32_receiveCANid2 << 5;
		sFilterConfig.FilterMaskIdLow  = 0;
	}

	if(HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK)
	{
		//Error Handler Code to be added later to act as redundancy.
		Error_Handler();
	}
}



void CAN_Write(void) {
     can.transmit(tx_Controller_705, S, can.txMsg[0],  8);
	 can.transmit(tx_Controller_706, S, can.txMsg[1],  8);
	 can.transmit(tx_Controller_708, S, can.txMsg[2],  8);
	 can.transmit(tx_Controller_710, S, can.txMsg[3],  8);
	 can.transmit(tx_Controller_715, S, can.txMsg[4],  8);
	 can.transmit(tx_Controller_716, S, can.txMsg[5],  8);
	 can.transmit(tx_Controller_717, S, can.txMsg[6],  8);
	 can.transmit(tx_Controller_724, S, can.txMsg[7],  8);
	 can.transmit(tx_Controller_725, S, can.txMsg[8],  8);
	 can.transmit(tx_Controller_726, S, can.txMsg[9],  8);
	 can.transmit(tx_Controller_727, S, can.txMsg[10], 8);

}


FDCAN_STATUS isFDCAN_Available(void) {
	         FDCAN_STATUS returnVal;

			 returnVal   = can.rx_flag;
			 can.rx_flag = false;
			 return returnVal;
}

void FDCAN_ApplicationSetup (void) {

  /* Configure Rx filter */
  CAN_Filter_IDList(rx_Controller_704,rx_BMS_111, S, FBANK0, FIFO0_CAN1);
  
  /* Start the FDCAN module */
  if(HAL_FDCAN_Start(&hcan1) != HAL_OK) {
	  Error_Handler();
  };

  HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);

}

