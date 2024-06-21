/*
 * can.h
 *
 *  Created on: 2 mai 2024
 *      Author: yyovo
 */

#include "can.h"

#include "stm32f4xx_hal.h"

#include "usart2.h"


extern CAN_frame my_frame;
extern CAN_frame received_frame;
extern CAN_frame CAN_RxMessages[10];
extern int read_index;
extern uint8_t emergency_data_read;
extern uint8_t can_read_data;
volatile int total_received;
int new_frame = 0;
extern CAN_frame CAN_RxMessage;
//void config_CAN(){
//    GPIO_InitTypeDef GPIO_InitStruct = { 0};
//
//    //Enable CAN CLock
//    __HAL_RCC_CAN1_CLK_ENABLE( ) ;
//
//    __HAL_RCC_GPIOB_CLK_ENABLE( ) ;
//
//    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//    GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
//    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct ) ;
//
//    // clear Sleep bit to wakeup
//    CAN1->MCR &= 0xFFFFFFFD;
//    // wait for can to wakeup
//    while(CAN1->MSR & 0x00000002);
//    CAN1->MCR |= 0x00000001;
//
//    // Wait for config mod
//    while(!(CAN1->MSR & 0x1));
//
//    // Set config
//    CAN1->MCR &= 0x00000001;
//
//    // setting inputs
////  CAN1->BTR = 0x40250010; // loopback
//    CAN1->BTR = 0x00250010;
//
//    /// wait for config mod
//    CAN1->RF0R |= CAN_RF0R_RFOM0;
//
//    // Se t I n t e r r u p t RX FIFO0 (FMPIE0) and TX box empty
//    CAN1->IER |= 0x2 ; //
//    HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn) ;
//
//    /// A c ti va t e I n t e r r u p t on NVIC
//    HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 0, 1);
//
//    // Go to normoal mode
//    CAN1->MCR &= 0xFFFFFFFE;
//
//    // wait for normal moode
//    while (CAN1->MCR & 0x1 ) ;
//
//    CAN1->FS1R |= 0x00000003 ;// 32 bits fil t e r f o r f i l t e r 0 ,1
//    CAN1->FM1R |= 0x00000002 ;// F i l t e r 1 i n L i s t mod
//    CAN1->FM1R &= 0xFFFFFFFE;// F i l t e r 0 i n mask mod
//
//    CAN1->FFA1R &= 0xFFFFFFFC; // A s sign f i l t e r 0 ,1 to FIFO0
//
//    // Configure Filter 0
//    CAN1->sFilterRegister[0].FR1 = 0;    // ID
//    CAN1->sFilterRegister[0].FR2 = 0;    // Mask (set all bits to 1)
//
//    // Activate Filter 0 and 1
//    CAN1->FA1R |= 0x00000003;
//
//    // Leave Filter init
//    CAN1->FMR &= 0x0;
//
//}
//
void CAN_config_filter(uint8_t IDE, uint32_t Filter_ID_high, uint32_t Filter_ID_low,
  uint32_t Filter_Mask_high, uint32_t Filter_Mask_low) {
  //    config_CAN();

  // ////////////////////////////////////////////////////////
  // Filter:
  // We will use filter 0 for 0x100 to 0x10F (Mask mode)
  // Filter 1 for 0x200 and 0x205 (ID List mode)
  // ////////////////////////////////////////////////////////

  // Deactivate Filter 0 and 1
  CAN1 -> FA1R &= 0xFFFFFFFC;
  // initialize mode for all filters
  CAN1 -> FMR |= 0x1;

  if (IDE == 0) {
    // ///////////////////////////
    // Filters with 32 bits mode//
    // ///////////////////////////

    CAN1 -> FS1R |= 0x00000003; // 32 bits filter for filter 0 ,1

    //CAN1->FM1R |= 0x00000002; // Filter 1 in List mode
    CAN1 -> FM1R &= 0xFFFFFFFC; // filter 1 and 0 in mask mode // Filter 0 in mask mode
    CAN1 -> FFA1R &= 0xFFFFFFFC; // Assign filter 0,1 to FIFO0

    // Configure the filter bank
    /*
     **/
    CAN1 -> sFilterRegister[0].FR1 = (Filter_ID_high << 21); //ID
    CAN1 -> sFilterRegister[0].FR2 = (Filter_Mask_high << 21); // Mask

    CAN1 -> sFilterRegister[1].FR1 = (Filter_ID_low << 21); //ID
    CAN1 -> sFilterRegister[1].FR2 = (Filter_Mask_low << 21); // ID
    //CAN1->sFilterRegister[1].FR2 |= 0x2; //ID with RTR = 1

  } else {
    // Filters with dual 16 bits config
    CAN1 -> FS1R &= 0xFFFFFFC; // Dual 16 bits filter for filter 0 ,1
    //CAN1->FM1R |= 0x00000002; // Filter 1 in List mode
    CAN1 -> FM1R &= 0xFFFFFFFC; // filter 1 and 0 in mask mode // Filter 0 in mask mode
    CAN1 -> FFA1R &= 0xFFFFFFFC; // Assign filter 0,1 to FIFO0

    // Configure the filter bank
    //CAN1->sFilterRegister[0].FR1 =  (Filter_ID_low << 5) ; //((Filter_Mask_low <<5) <<16) || (Filter_ID_low << 5); //ID
    CAN1 -> sFilterRegister[0].FR1 = (Filter_Mask_low << 16) | (Filter_ID_low); //((Filter_Mask_low <<5) <<16); // Mask

  }

  // Activate Filter 0 and 1
  CAN1 -> FA1R |= 0x00000003;
  // Leave Filter init
  CAN1 -> FMR &= 0xFFFFFFFE;

}

void CAN_config(void) {

  GPIO_InitTypeDef GPIO_InitStruct = {
    0
  };

  //EnableCANCLock
  __HAL_RCC_CAN1_CLK_ENABLE();

  //ConfigureGPIOs
  __HAL_RCC_GPIOB_CLK_ENABLE();
  /*CAN1GPIOConfiguration
  PB8------>CAN1_RX
  PB9------>CAN1_TX*/

  GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
  HAL_GPIO_Init(GPIOB, & GPIO_InitStruct);

  //ClearSleepbittowakeup
  CAN1 -> MCR &= 0xFFFFFFFD;
  //WaitforCANtowakeup
  while (CAN1 -> MSR & 0x00000002);
  //Switchtoconfigmode
  CAN1 -> MCR |= 0x00000001;
  //Waitforconfigmod
  while (!(CAN1 -> MSR & 0x1));

  //Setconfig
  CAN1 -> MCR &= 0x00000001;

  CAN1 -> BTR = 0x00250010; //Par ordre : loop back activé

  //INTERRUPTION//
  //Set Interrupt RXFIFO0 (FMPIE0) and TX box empty

  CAN1 -> IER |= 0x2;
  ///Activate Interrupton NVIC
  HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 6, 1);
  HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
  //HAL_NVIC_SetPriority(CAN1_TX_IRQn,0,2);
  //HAL_NVIC_EnableIRQ(CAN1_TX_IRQn);
  ////////////////
  // Go to normal mode
  CAN1 -> MCR &= 0xFFFFFFFE;
  //Wait f o r normal mod
  while (CAN1 -> MSR & 0x1);

  //    filtre_config2(1,1,1,0x8002,0x8800,0xFFFF,0xF800);
  /*
        //DeactivateFilter0and1
        CAN1->FA1R&=0xFFFFFFFC;
        //initializemodeforallfilters
        CAN1->FMR|=0x1;

        CAN1->FS1R|=0x00000003;//32bitsfilterforfilter0,1
        CAN1->FM1R|=0x00000002;//Filter1inListmod
        CAN1->FM1R&=0xFFFFFFFE;//Filter0inmaskmod
        CAN1->FFA1R&=0xFFFFFFFC;//Assignfilter0,1toFIFO0
        //Configurethefilterbank
        CAN1->sFilterRegister[0].FR1=(0x000<<21);//ID
        CAN1->sFilterRegister[0].FR2=(0x400<<21);//Mask

        CAN1->sFilterRegister[1].FR1=(0x200<<21);//ID
        CAN1->sFilterRegister[1].FR2=(0x205<<21);//ID
        CAN1->sFilterRegister[1].FR2|=0x2;//IDwithRTR=1

        //ActivateFilter0and1
        CAN1->FA1R|=0x00000003;
        //LeaveFilterinit
        CAN1->FMR&=0x0;
  */

  //      initializeBuffer(&farriere_portA);
  //      initializeBuffer(&farriere_portB);
  //      initializeBuffer(&farriere_portC);
  //      initializeBuffer(&etat_commodo);
}

void filtre_config2(uint8_t num_filtre, uint8_t IDE, uint8_t Filtre_mode, uint16_t Filter_ID_high, uint16_t Filter_ID_low, uint16_t Filter_Mask_high, uint16_t Filter_Mask_low) {

  //Desactivation du filtre à configurer
  CAN1 -> FA1R &= (0xFFFFFFFF >> (32 - (num_filtre - 1)) | 0xFFFFFFFF << num_filtre);
  //initializemodeforallfilters
  CAN1 -> FMR |= 0x1;

  /////////////////////////////
  //Filterswith32bitsmode//
  /////////////////////////////

  if (IDE == 1) {

    CAN1 -> FS1R |= (0xFFFFFFFF >> (32 - num_filtre) & 0xFFFFFFFF << (num_filtre - 1)); //32 bits pour filtre num_filter
  } else {
    CAN1 -> FS1R &= (0xFFFFFFFF >> (32 - (num_filtre - 1)) | 0xFFFFFFFF << num_filtre); //16 bits
  }
  if (Filtre_mode == 0) {
    CAN1 -> FM1R |= (0xFFFFFFFF >> (32 - num_filtre) & 0xFFFFFFFF << (num_filtre - 1)); // mode liste filtre i
  } else {
    CAN1 -> FM1R &= (0xFFFFFFFF >> (32 - (num_filtre - 1)) | 0xFFFFFFFF << num_filtre); // mode mask
  }

  CAN1 -> FFA1R &= (0xFFFFFFFF >> (32 - (num_filtre - 1)) | 0xFFFFFFFF << num_filtre); //Assigne filtre au FIFO0

  //CONFIGURE LE FILTRE
  CAN1 -> sFilterRegister[num_filtre - 1].FR1 = (Filter_ID_high << 16 | Filter_ID_low); //ID
  CAN1 -> sFilterRegister[num_filtre - 1].FR2 = (Filter_Mask_high << 16 | Filter_Mask_low); //Mask

  // Activation du filtre
  CAN1 -> FA1R |= (0xFFFFFFFF >> (32 - num_filtre) & 0xFFFFFFFF << (num_filtre - 1));
  // Leave F i l t e r i n i t
  CAN1 -> FMR &= 0x0;

}

uint8_t CAN_sendFrame(CAN_frame CAN_mess) {
  //Check i f TxMailbox0 i s empty
  if (CAN1 -> TSR & 0x04000000) {

    //fillthemailbox
    CAN1 -> sTxMailBox[0].TIR = (CAN_mess.STDID << 3) | (CAN_mess.RTR << 1) | (1 << 2);
    CAN1 -> sTxMailBox[0].TDTR = CAN_mess.DLC;
    CAN1 -> sTxMailBox[0].TDLR = CAN_mess.data[3] << 24 | CAN_mess.data[2] << 16 | CAN_mess.data[1] << 8 | CAN_mess.data[0];
    CAN1 -> sTxMailBox[0].TDHR = CAN_mess.data[7] << 24 | CAN_mess.data[6] << 16 | CAN_mess.data[5] << 8 | CAN_mess.data[4];
    CAN1 -> sTxMailBox[0].TIR |= 1; //Sendthemessage
  } else return (0);
}

uint8_t CAN_readFrame() {
  CAN_RxMessage.STDID = CAN1 -> sFIFOMailBox[0].RIR >> 3;
  CAN_RxMessage.RTR = (CAN1 -> sFIFOMailBox[0].RIR >> 1) & 0x1;
  CAN_RxMessage.DLC = CAN1 -> sFIFOMailBox[0].RDTR & 0xF;
  CAN_RxMessage.data[0] = CAN1 -> sFIFOMailBox[0].RDLR & 0xFF;
  CAN_RxMessage.data[1] = (CAN1 -> sFIFOMailBox[0].RDLR >> 8) & 0xFF;
  CAN_RxMessage.data[2] = (CAN1 -> sFIFOMailBox[0].RDLR >> 16) & 0xFF;
  CAN_RxMessage.data[3] = (CAN1 -> sFIFOMailBox[0].RDLR >> 24) & 0xFF;
  CAN_RxMessage.data[4] = CAN1 -> sFIFOMailBox[0].RDHR & 0xFF;
  CAN_RxMessage.data[5] = (CAN1 -> sFIFOMailBox[0].RDHR >> 8) & 0xFF;
  CAN_RxMessage.data[6] = (CAN1 -> sFIFOMailBox[0].RDHR >> 16) & 0xFF;
  CAN_RxMessage.data[7] = (CAN1 -> sFIFOMailBox[0].RDHR >> 24) & 0xFF;

  CAN_RxMessages[total_received] = CAN_RxMessage;

  total_received = (total_received + 1) % BUFFER_LIMIT;
  can_read_data = 1;
  if (total_received >= (BUFFER_LIMIT - 3)) { //== read_index
    emergency_data_read = 1;
  }

  CAN1 -> RF0R |= 0x20;

}

void display_frame(CAN_frame received_frame) {
  char converted_data[2];
  serial_puts("=============");
  newLine();
  serial_puts("type: ");
  if (received_frame.RTR == 0) {
    serial_puts("Data Frame");
  } else {
    serial_puts("Request Frame");
  }
  newLine();

  int2string(received_frame.STDID, converted_data);
  serial_puts("Identifiant: ");
  serial_puts(converted_data);
  // test.id;
  newLine();

  serial_puts("Donnée: ");
  for (int i = 0; i < 8; i++) {
    int2string(received_frame.data[i], converted_data);
    serial_puts(converted_data);
    serial_puts("_");
  }
  newLine();
  serial_puts("=============");
  newLine();
}

void read_buffer(void) {
  if (total_received == read_index) {
    // Pas de nouvelles données à lire
    can_read_data = 0;
    return;
  }

  // Lire le message CAN du buffer
  received_frame = CAN_RxMessages[read_index];
  read_index = (read_index + 1) % BUFFER_LIMIT;

  // Déterminer si plus de données sont disponibles
  can_read_data = (read_index != total_received);

  /*
  received_frame = CAN_RxMessages[read_index];
  read_index++;
  if(read_index >= BUFFER_LIMIT){
        read_index = 0;
  }

  if(read_index == total_received){
        can_read_data = 0;
  }else{
        can_read_data = 1;
  }*/
}
