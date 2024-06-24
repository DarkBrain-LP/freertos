/*
 * commodos.c
 *
 *  Created on: 3 juin 2024
 *      Author: yyovo
 */
#include "can.h"
#include "commodos.h"

CAN_frame auto_frame;
extern CAN_frame CAN_RxMessage;
volatile int phare_state = 1;

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

void CAN1_RX0_IRQHandler( void )
{
	CAN_readFrame();
	if(CAN_RxMessage.STDID == COMMODO_PHARE_RETURN_ID){
		if(CAN_RxMessage.data[0] == COMMODO_PHARE_RETURN_VALUE){
			phare_state = 0;
		}else{
			phare_state = 1;
		}
	}
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
