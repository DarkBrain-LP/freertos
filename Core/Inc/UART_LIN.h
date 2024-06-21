/* 
 * File:   UART_LIN.h
 * Author: aravey
 *
 * Created on 12 april 2020, 13:42
 */

#ifndef UART_LIN_H
#define	UART__LIN_H
#include <stdlib.h>
#include "stm32f4xx_hal.h"
#include "rtc.h"


//Declaration to Link External Functions & Variables:

/*--- Lin message structures and flag ---*/

typedef struct
{
  uint8_t ID;
  uint8_t length;
  uint8_t data[10];
}LINMSG;




// Macros
// Macros
#define LIN_SLAVE_WRITE_RTC_PID 69
#define LIN_SLAVE_WRITE_RTC_LENGTH 6

#define LIN_SLAVE_READ_ACC_PID 68
#define LIN_SLAVE_READ_ACC_LENGTH 3

#define CAN_ACC_ID 0x2
#define CAN_RTC_ID 0x3

// ACTIONS
#define ACTION_READ_ID 0
#define ACTION_SEND_MESSAGE 1
#define ACTION_READ_MESSAGE 2
#define ACTION_NOTHING -1
#define LIN_SYNCH 0x55

//Variables with Global Scope  
extern  unsigned char DisplayData[];
extern  LINMSG Tx_Msg;
extern  LINMSG Rx_Msg;
extern int new_request;

extern int should_read_id;
extern int deactivate_interrupt;
extern int id_readed;
extern int should_read_data;
extern int should_checksum;
extern int should_synch;
extern int i;

// RTC
extern TimeTypeDef rtc_time;
extern DateTypeDef rtc_date;

//Functions :
void UART_Init (void);
void SendMessage(LINMSG *msg);
void SendRequest(LINMSG *msg);
int slave_response(void);
int slave_response_RTC(void);
void UART_PutChar(uint8_t data);
void sync_break(void);
uint8_t checksum(uint8_t length, uint8_t *data);



#endif	/* UART_LIN_H */

