/*
* can.h
*
*  Created on: 2 mai 2024
*      Author: yyovo
*/

#ifndef INC_CAN_H_
#define INC_CAN_H_

#include <stdlib.h>
#include "stm32f4xx_hal.h"

typedef struct {
  uint32_t STDID; // ID s tanda rd (11 b i t s )
  uint8_t RTR; // Request ou Data frame
  uint8_t DLC; // nombre d ’ o c t e t s de données u t i l e s
  uint8_t data [ 8 ] ; // données
}CAN_frame;

#define BUFFER_LIMIT 10


//extern CAN_frame my_frame;
//extern CAN_frame received_frame;
//extern CAN_frame CAN_RxMessages[10];
//extern int read_index;
//extern uint8_t emergency_data_read;
//extern uint8_t can_read_data;
//volatile int total_received;
//int new_frame = 0;
//extern CAN_frame CAN_RxMessage;

void config_CAN();
//void CAN_config_filter(uint8_t IDE, uint16_t Filter_ID_high, uint16_t Filter_ID_low, uint16_t Filter_Mask_high, uint16_t Filter_mask_low);
void CAN_config_filter(uint8_t IDE, uint32_t Filter_ID_high, uint32_t Filter_ID_low,
		uint32_t Filter_Mask_high, uint32_t Filter_Mask_low);

void filtre_config2(uint8_t num_filtre, uint8_t IDE, uint8_t Filtre_mode, uint16_t Filter_ID_high, uint16_t Filter_ID_low, uint16_t Filter_Mask_high, uint16_t Filter_Mask_low);


uint8_t CAN_sendFrame (CAN_frame CAN_mess);
void read_buffer(void);
void CAN1_RX0_IRQHandler( void );

uint8_t CAN_readFrame(void);
void display_frame(CAN_frame received_frame);

#endif /* INC_CAN_H_ */
