/*
 * commodos.h
 *
 *  Created on: 3 juin 2024
 *      Author: yyovo
 */

#ifndef INC_COMMODOS_H_
#define INC_COMMODOS_H_


#define COMMODO_CLIGNOTANT_RETURN_ID 0x10005110
#define COMMODO_CLIGNOTANT_RETURN_VALUE 0x14

#define COMMODO_PHARE_RETURN_ID 0x801a8884
#define COMMODO_PHARE_RETURN_VALUE 0x34

#define COMMODO_CLIGNOTANTS_ID 0x10510310
#define COMMODO_CLIGNOTANT_DROIT_Pos 2
#define COMMODO_CLIGNOTANT_REPOS_Pos 3
#define COMMODO_CLIGNOTANT_GAUCHE_Pos 4

#define COMMODO_ROUTE_PHARES_ID 0x10510310
#define COMMODO_ROUTE_PHARES_ID 1

#define COMMODO_APPEL_PHARES_ID 0x10510311
#define COMMODO_APPEL_PHARES_Pos 6

#define COMMODO_PHARES_ID 0x10510311
#define COMMODO_PHARES_Pos 7

#define COMMODO_ESSUIE_GLACE_ID 0x10510311
#define COMMODO_ESSUIE_GLACE_Pos 4

void clignotant_droit(void);
void clignotant_gauche(void);
void clignotant_repos(void);
void clignotant_route_phare(void);
void appel_phare(void);
void phares(void);

#endif /* INC_COMMODOS_H_ */
