/*
 * commodos.c
 *
 *  Created on: 3 juin 2024
 *      Author: yyovo
 */
#include "can.h"
#include "commodos.h"
#include "cmsis_os.h"

CAN_frame auto_frame;
extern CAN_frame CAN_RxMessage;
volatile int phare_state = 1;
int nadekio = 0;
// freeRTOS
extern osThreadId canReceiverTaskHandle;
extern osThreadId canTrackerTaskHandle;
extern osMessageQId linCanQueueHandle;

void clignotant_droit(void){

}
void clignotant_gauche(void){

}
void clignotant_repos(void){

}
void clignotant_route_phare(void){

}
void appel_phare(void){
	  auto_frame.STDID = 0x10510310;
	  auto_frame.RTR = 1;
	  CAN_sendFrame(auto_frame);
}



//void phares(CAN_frame frame){
//	if(frame)
//}
//
//void CAN1_RX0_IRQHandler( void ){
//	CAN_readFrame();
//
//	if((CAN_RxMessage.STDID == COMMODO_CLIGNOTANT_RETURN) && (CAN_RxMessage.data[0] == COMMODO_CLIGNOTANT_RETURN_VALUE)){
//		appel_phare();
//	}
//
//}
