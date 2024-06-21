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
	allumer_phare_gauche();
}

// eteindre phares
void eteindre_phares(void){
	eteindre_phare_gauche();
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
	auto_frame.STDID = FEUX_ARRIERE_CLIGNOTANT_DROIT_ID;//0x10520312;
	auto_frame.DLC = 1;
	auto_frame.RTR = 0;
	auto_frame.data[0] = (8 & ~(1<<FEUX_ARRIERE_CLIGNOTANT_DROIT_POS));
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
