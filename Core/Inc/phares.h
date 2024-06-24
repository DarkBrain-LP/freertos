/*
 * phares_avant.h
 *
 *  Created on: 3 juin 2024
 *      Author: yyovo
 */

#ifndef INC_PHARES_H_
#define INC_PHARES_H_

#define FEUX_AVANT_GAUCHE_PHARE_ID 0x10520312
#define FEUX_AVANT_GAUCHE_PHARE_POS 0

#define FEUX_AVANT_GAUCHE_VEILLEUSE_ID 0x10520312
#define FEUX_AVANT_GAUCHE_VEILLEUSE_POS 3

#define FEUX_AVANT_GAUCHE_CLIGNONTANT_ID 0x10520312
#define FEUX_AVANT_GAUCHE_CLIGNONTANT_POS 5

#define FEUX_AVANT_GAUCHE_CODE_ID 0x10520310
#define FEUX_AVANT_GAUCHE_CONTROL_ID 0x10520310
#define FEUX_AVANT_GAUCHE_NC_ID 0x10520310

#define FEUX_AVANT_PHARE_DROIT_ID 0x10520311
#define FEUX_AVANT_PHARE_DROIT_POS 5

#define FEUX_AVANT_DROIT_CLIGNONTANT_ID 0x10520311 //0x10520311
#define FEUX_AVANT_DROIT_CLIGNONTANT_POS 0

#define FEUX_AVANT_VEILLEUSE_DROIT_ID 0x10520311
#define FEUX_AVANT_VEILLEUSE_DROIT_POS 4

#define FEUX_AVANT_VEILLEUSE_GAUCHE_ID 0x10520312
#define FEUX_AVANT_VEILLEUSE_GAUCHE_POS 5


//=====================================
//=============FEUX ARRIERE============
//=====================================

// Droit
#define FEUX_ARRIERE_CLIGNOTANT_DROIT_ID 0x10530310
#define FEUX_ARRIERE_CLIGNOTANT_DROIT_POS 0

#define FEUX_ARRIERE_POSITION_DROIT_ID 0x10530310
#define FEUX_ARRIERE_POSITION_DROIT_POS 2

#define FEUX_ARRIERE_STOP_DROIT_ID 0x10530310
#define FEUX_ARRIERE_STOP_DROIT_POS 3

// Gauche

#define FEUX_ARRIERE_STOP_GAUCHE_ID 0x10530310
#define FEUX_ARRIERE_STOP_GAUCHE_POS 5

#define FEUX_ARRIERE_POSITION_GAUCHE_ID 0x10530312
#define FEUX_ARRIERE_POSITION_GAUCHE_POS 0

#define FEUX_ARRIERE_CLIGNOTANT_GAUCHE_ID 0x10530312
#define FEUX_ARRIERE_CLIGNOTANT_GAUCHE_POS 2

void allumer_phares(void);
void eteindre_phares(void);

void allumer_veilleuses(void);
void allumer_veilleuse_gauche(void);
void allumer_veilleuse_droite(void);

void allumer_phare_gauche(void);
void eteindre_phare_gauche(void);

void allumer_phare_droit(void);
void eteindre_phare_droit(void);

void allumer_clignotants_avant(void);
void allumer_clignotant_gauche(void);
void allumer_clignotant_droit(void);

void eteindre_clignotans_avant(void);
void eteindre_clignotant_avant_gauche(void);
void eteindre_clignotant_avant_droit(void);

// clignants arrieres
void allumer_clignotants_arrirere(void);
void allumer_clignotant_arriere_gauche(void);
void allumer_clignotant_arriere_droit(void);

void eteindre_clignotans_avant(void);
void eteindre_clignotant_arriere_gauche(void);
void eteindre_clignotant_avant_droit(void);

void allumer_clignotants_gauche(void);
void allumer_clignotants_droit(void);

void eteindre_clignotants_gauche(void);
void eteindre_clignotants_droit(void);


void allumer_feux_arriere(void);
void allumer_feux_arriere_gauche(void);
void allumer_feux_arriere_droit(void);
void eteindre_feux_arriere(void);
void eteindre_feux_arriere_gauche(void);
void eteindre_feux_arriere_droit(void);

void get_state(void);

#endif /* INC_PHARES_H_ */
