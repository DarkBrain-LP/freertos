/*
 * phares.c
 *
 *  Created on: 3 juin 2024
 *      Author: yyovo
 */

#include "phares.h"
#include "can.h"

void allumer_phare_gauche(void){
	CAN_frame auto_frame;
	auto_frame.STDID = FEUX_AVANT_GAUCHE_PHARE_ID;//0x10520312;
	auto_frame.DLC = 1;
	auto_frame.RTR = 0;
	auto_frame.data[0] = 1<<FEUX_AVANT_GAUCHE_PHARE_POS;
	CAN_sendFrame(auto_frame);
}

void allumer_phare_droit(void){
	CAN_frame auto_frame;
	auto_frame.STDID = FEUX_AVANT_PHARE_DROIT_ID;//0x10520312;
	auto_frame.DLC = 1;
	auto_frame.RTR = 0;
	auto_frame.data[0] = 1<<FEUX_AVANT_PHARE_DROIT_POS;
	CAN_sendFrame(auto_frame);
}

void allumer_phares(void){
	allumer_phare_droit();
	HAL_Delay(10);
	allumer_phare_gauche();
}

void allumer_veilleuses(void){
	allumer_veilleuse_gauche();
	allumer_veilleuse_droite();
}
void allumer_veilleuse_gauche(void) {
	CAN_frame auto_frame;
	auto_frame.STDID = FEUX_AVANT_VEILLEUSE_GAUCHE_ID;//0x10520312;
	auto_frame.DLC = 1;
	auto_frame.RTR = 0;
	auto_frame.data[0] = 1<<FEUX_AVANT_VEILLEUSE_GAUCHE_POS;
	CAN_sendFrame(auto_frame);
}
void allumer_veilleuse_droite(void){
	CAN_frame auto_frame;
	auto_frame.STDID = FEUX_AVANT_VEILLEUSE_DROIT_ID;//0x10520312;
	auto_frame.DLC = 1;
	auto_frame.RTR = 0;
	auto_frame.data[0] = 1<<FEUX_AVANT_VEILLEUSE_DROIT_POS;
	CAN_sendFrame(auto_frame);
}

// eteindre phares
void eteindre_phares(void){
	eteindre_phare_gauche();
	HAL_Delay(10);
	eteindre_phare_droit();
}

void eteindre_phare_gauche(void){
	CAN_frame auto_frame;
		auto_frame.STDID = FEUX_AVANT_GAUCHE_PHARE_ID;//0x10520312;
		auto_frame.DLC = 1;
		auto_frame.RTR = 0;
		auto_frame.data[0] = (8 & ~(1<<FEUX_AVANT_GAUCHE_PHARE_POS));
		CAN_sendFrame(auto_frame);
}

void eteindre_phare_droit(void){
	CAN_frame auto_frame;
	auto_frame.STDID = FEUX_AVANT_PHARE_DROIT_ID;//0x10520312;
	auto_frame.DLC = 1;
	auto_frame.RTR = 0;
	auto_frame.data[0] = (8 & ~(1<<FEUX_AVANT_PHARE_DROIT_POS));
	CAN_sendFrame(auto_frame);
}


// allumer clignotants
void allumer_clignotants_avant(void){
	allumer_clignotant_droit();
	allumer_clignotant_gauche();
}

void allumer_clignotant_gauche(void){
	CAN_frame auto_frame;
	auto_frame.STDID = FEUX_AVANT_GAUCHE_CLIGNONTANT_ID;//0x10520312;
	auto_frame.DLC = 1;
	auto_frame.RTR = 0;
	auto_frame.data[0] = 1 << FEUX_AVANT_GAUCHE_CLIGNONTANT_POS;
	CAN_sendFrame(auto_frame);
}
void allumer_clignotant_droit(void){
	CAN_frame auto_frame;
	auto_frame.STDID = FEUX_AVANT_DROIT_CLIGNONTANT_ID;//0x10520312;
	auto_frame.DLC = 1;
	auto_frame.RTR = 0;
	auto_frame.data[0] = 1 << FEUX_AVANT_DROIT_CLIGNONTANT_POS;
	CAN_sendFrame(auto_frame);
}


void allumer_clignotants_arrirere(void){
	allumer_clignotant_arriere_droit();
	allumer_clignotant_arriere_gauche();
}
void allumer_clignotant_arriere_gauche(void){
	CAN_frame auto_frame;
	auto_frame.STDID = FEUX_ARRIERE_CLIGNOTANT_GAUCHE_ID;//0x10520312;
	auto_frame.DLC = 1;
	auto_frame.RTR = 0;
	auto_frame.data[0] = 1 << FEUX_ARRIERE_CLIGNOTANT_GAUCHE_POS;
	CAN_sendFrame(auto_frame);
}
void allumer_clignotant_arriere_droit(void){
	CAN_frame auto_frame;
	auto_frame.STDID = FEUX_ARRIERE_CLIGNOTANT_DROIT_ID;//0x10520312;
	auto_frame.DLC = 1;
	auto_frame.RTR = 0;
	auto_frame.data[0] = 1 << FEUX_ARRIERE_CLIGNOTANT_DROIT_POS;
	CAN_sendFrame(auto_frame);
}

// éteindre clignotants
void eteindre_clignotans_avant(void){
	eteindre_clignotant_avant_droit();
	eteindre_clignotant_avant_gauche();
}
void eteindre_clignotant_avant_gauche(void){
	CAN_frame auto_frame;
	auto_frame.STDID = FEUX_AVANT_GAUCHE_CLIGNONTANT_ID;//0x10520312;
	auto_frame.DLC = 1;
	auto_frame.RTR = 0;
	auto_frame.data[0] = (8 & ~(1<<FEUX_AVANT_GAUCHE_CLIGNONTANT_POS));
	CAN_sendFrame(auto_frame);
}
void eteindre_clignotant_avant_droit(void){
	CAN_frame auto_frame;
	auto_frame.STDID = FEUX_AVANT_DROIT_CLIGNONTANT_ID;//0x10520312;
	auto_frame.DLC = 1;
	auto_frame.RTR = 0;
	auto_frame.data[0] = (8 & ~(1<<FEUX_AVANT_DROIT_CLIGNONTANT_POS));
	CAN_sendFrame(auto_frame);
}

void eteindre_clignotans_arriere(void){
	eteindre_clignotant_arriere_droit();
	eteindre_clignotant_arriere_gauche();
}
void eteindre_clignotant_arriere_droit(void){
	CAN_frame auto_frame;
	auto_frame.STDID = FEUX_ARRIERE_STOP_DROIT_ID;// FEUX_ARRIERE_CLIGNOTANT_DROIT_ID;
	auto_frame.DLC = 1;
	auto_frame.RTR = 0;
	auto_frame.data[0] = (8 & ~(1<<FEUX_ARRIERE_STOP_DROIT_POS)); // FEUX_ARRIERE_CLIGNOTANT_DROIT_POS
	CAN_sendFrame(auto_frame);
}
void eteindre_clignotant_arriere_gauche(void){
	CAN_frame auto_frame;
	auto_frame.STDID = FEUX_ARRIERE_CLIGNOTANT_GAUCHE_ID;//0x10520312;
	auto_frame.DLC = 1;
	auto_frame.RTR = 0;
	auto_frame.data[0] = (8 & ~(1<<FEUX_ARRIERE_CLIGNOTANT_GAUCHE_POS));
	CAN_sendFrame(auto_frame);
}


/// feux arriere stop
void allumer_feux_arriere(void){
	allumer_feux_arriere_gauche();
	HAL_Delay(10);
	allumer_feux_arriere_droit();
}
void allumer_feux_arriere_gauche(void){
	CAN_frame auto_frame;
	auto_frame.STDID = FEUX_ARRIERE_STOP_GAUCHE_ID;
	auto_frame.DLC = 1;
	auto_frame.RTR = 0;
	auto_frame.data[0] = 1<<FEUX_ARRIERE_STOP_GAUCHE_POS;
	CAN_sendFrame(auto_frame);
}

void allumer_feux_arriere_droit(void){
	CAN_frame auto_frame;
	auto_frame.STDID = FEUX_ARRIERE_STOP_DROIT_ID;
	auto_frame.DLC = 1;
	auto_frame.RTR = 0;
	auto_frame.data[0] = 1<<FEUX_ARRIERE_STOP_DROIT_POS;
	CAN_sendFrame(auto_frame);
}

void eteindre_feux_arriere(void){
	eteindre_feux_arriere_droit();
	eteindre_feux_arriere_gauche();
}
void eteindre_feux_arriere_gauche(void){
	CAN_frame auto_frame;
	auto_frame.STDID = FEUX_ARRIERE_STOP_GAUCHE_ID;//0x10520312;
	auto_frame.DLC = 1;
	auto_frame.RTR = 0;
	auto_frame.data[0] = (8 & ~(1<<FEUX_ARRIERE_STOP_GAUCHE_POS));
	CAN_sendFrame(auto_frame);
}
void eteindre_feux_arriere_droit(void){
	CAN_frame auto_frame;
	auto_frame.STDID = FEUX_ARRIERE_STOP_DROIT_ID;//0x10520312;
	auto_frame.DLC = 1;
	auto_frame.RTR = 0;
	auto_frame.data[0] = (8 & ~(1<<FEUX_ARRIERE_STOP_DROIT_POS));
	CAN_sendFrame(auto_frame);
}

void allumer_clignotants_gauche(void){
	allumer_clignotant_gauche();
	HAL_Delay(10);
	allumer_clignotant_arriere_gauche();
}
void allumer_clignotants_droit(void){
	allumer_clignotant_droit();
	HAL_Delay(10);
	allumer_clignotant_arriere_droit();
}

void eteindre_clignotants_gauche(void){
	eteindre_clignotant_avant_gauche();
	HAL_Delay(10);
	eteindre_clignotant_arriere_gauche();
}
void eteindre_clignotants_droit(void){
	eteindre_clignotant_avant_droit();
	HAL_Delay(10);
	eteindre_clignotant_arriere_droit();
}
