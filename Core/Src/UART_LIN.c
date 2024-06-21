/* 
 * File:   UART_LIN.c
 * Author: aravey
 *
 * Created on 12 april 2020, 13:42
 */

#include "UART_LIN.h"
#include "stm32f4xx_hal.h"
#include "usart2.h"
//#include "rtc.h"
#include "lis3dsh.h"
#include "can.h"
#include "phares.h"
int action = 0;
extern int reading_acc;
extern int lin_readed_index;
//Functions

//UART_Init() sets up the UART for a 8-bit data, No Parity, 1 Stop bit
//at 9600 baud with transmitter interrupts enabled
void UART_Init (void)
{

	// Enable USART3 clock
	__HAL_RCC_USART3_CLK_ENABLE();

	// Enable GPIOB clock
	__HAL_RCC_GPIOB_CLK_ENABLE();

	// Configure USART3 TX and RX pins
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_11;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	// Configure USART3
	USART3->CR1 &= ~USART_CR1_UE; // Disable USART3

	// Communication bidirectionnelle, 8 bits de donnée, 1 bit de stop, pas de parité
	USART3->CR1 &= ~USART_CR1_M; // 8 bits de donnée
	USART3->CR1 &= ~USART_CR1_PCE; // Pas de parité
	USART3->CR2 &= ~USART_CR2_STOP; // 1 bit de stop

	USART3->CR1 |= USART_CR1_RXNEIE; // Activate RXNIE for interruptions
	USART3->CR2 &= ~USART_CR2_LBDL; // 10 bits for break
	//USART3->CR2 |= USART_CR2_LBDIE; // Activate LBDIE
	//USART3->CR1 |= USART_CR1_TXEIE; // activate interrupt on transmit

	// Vitesse de communication de 9600bauds et oversampling à 16
	// BRR = fclk / (16 * baudrate)
	// Ici, fclk = 84MHz, baudrate = 9600
	// BRR = 84MHz / (16 * 9600) = 546.875
	// Integer part = 546, Fractional part = 0.875 * 16 = 14
	USART3->BRR = 0x00001117; // 273d (546d = 0x222)
	USART3->CR1 |= USART_CR1_TE | USART_CR1_RE; // Enable Tx and Rx

	USART3->CR1 |= USART_CR1_UE;

	HAL_Delay(1); // Attente de 1ms
	NVIC_SetPriority(USART3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),6, 0));
	NVIC_EnableIRQ(USART3_IRQn);

	//
}


/*--- Transmit LIN Message ---*/
void SendMessage(LINMSG *msg)
{
	USART3->CR1 &= ~USART_CR1_RXNEIE;
	for(int i = 0; i <= msg->length; i++)
	{
		UART_PutChar(msg->data[i]);
		char tmp[2];
		int data = msg->data[i];

		int2string(i, tmp);
		serial_puts("\t    data[");
		serial_puts(tmp);
		serial_puts("] = ");

		int2string(data, tmp);
		serial_puts(tmp);
		newLine();
	}
	USART3->CR1 |= USART_CR1_RXNEIE;
}

/*--- Transmit LIN Request ---*/
void sync_break(void)
{
	// Send break character
	USART3->CR1 |= USART_CR1_SBK; // Send break
	//while (!(USART3->SR & USART_SR_TC)); // Wait for transmission complete
//	serial_puts("SYNCH_BREAK: Sent break");
//	newLine();
    // Send Sync field (0x55)
    USART3->DR = 0x55;
//	serial_puts("SYNCH_BREAK: Sent synch");
//	newLine();
    //while (!(USART3->SR & USART_SR_TC)); // Wait for transmission complete

}

void SendHeader(LINMSG *msg)
{
	sync_break(); // send break and synch
	UART_PutChar(msg->ID); // send Id
//	serial_puts("HEADER: Sent header");
//	newLine();
}

void SendRequest(LINMSG *msg)
{
	USART3->CR1 |= USART_CR1_RXNEIE;
	SendHeader(msg);
	// activate interrupt
}

/*
int slave_response(void){

	// desactivate interupt
	// receive data
	int len = Rx_Msg.length;
	for(int i = 0; i < len; i++){
		// attendre que la donnée suivante soit prête
		while(!(USART3->SR & USART_SR_RXNE));
		// lire la donnée
		Rx_Msg.data[i] = USART3->DR;
	}
}
*/

int slave_response(void){
	/*if(USART3->SR & USART_SR_LBD){
		USART3->SR &= ~USART_SR_LBD;
		deactivate_interrupt = 1;
		new_request = 1;
		should_read_id = 1;
		int data = USART3->DR;
		if(data == 0){
			//USART3->CR1 &= ~USART_CR1_RXNEIE;
			return;
		}

	}
	else{
	*/
	if(USART3->SR & USART_SR_RXNE){
			int data = USART3->DR;
			char tmp[2];



			if(data == LIN_SYNCH){
				action = ACTION_READ_ID;
//				serial_puts("Received SYNCH");
			}
			else if((data == 0) && (action != ACTION_READ_MESSAGE)){
				action = ACTION_READ_ID;
//				serial_puts("Received BREAK");
//				newLine();
				return;
			}
			else{
				switch (action) {
					case ACTION_READ_ID:
						Rx_Msg.ID = data;

//						serial_puts("Readed ID =");
//						int2string(data, tmp);
//						serial_puts(tmp);
//						newLine();

						switch (Rx_Msg.ID) {
							case LIN_SLAVE_WRITE_RTC_PID:
								slave_send_RTC();
								break;
							case LIN_SLAVE_READ_ACC_PID: // if it accelero ID
								action = ACTION_READ_MESSAGE;
								Rx_Msg.length = LIN_SLAVE_READ_ACC_LENGTH;
								i = 0;
							default:
								break;
						}
						break;

					case ACTION_READ_MESSAGE:

						// set reading status
						switch (Rx_Msg.ID) {
								case LIN_SLAVE_READ_ACC_PID:
									reading_acc = 1;
//									serial_puts("SET ACC READING");
									break;
									// other reading IDs
								default:
									break;
							}

						Rx_Msg.data[i] = data;

						// DEBUG
//						int2string(reading_acc, tmp);
//						serial_puts("reading_acc= ");
//						serial_puts(tmp);
//
//						int2string(i, tmp);
//						serial_puts("\t    data[");
//						serial_puts(tmp);
//						serial_puts("] = ");
//
//						int2string(data, tmp);
//						serial_puts(tmp);
//
//
//						newLine();

						i++;

						if(Rx_Msg.length == i){
							action = ACTION_NOTHING;
							serial_puts("LIN received: ");
							char converted_data[2];
							for(int k = 0; k < Rx_Msg.length; k++){
								int2string(Rx_Msg.data[k], converted_data);
								serial_puts(converted_data);
								serial_puts(",");
							}
							//serial_puts(Rx_Msg.data);
							newLine();
							// envoie CAN
							CAN_frame frame;
							frame.STDID = CAN_ACC_ID;
							for(int j = 0; j < Rx_Msg.length; j++){
								frame.data[j] = Rx_Msg.data[j];
							}
							frame.RTR = 0;
							int status = CAN_sendFrame(frame);

							// notify end of read
							switch (Rx_Msg.ID) {
								case LIN_SLAVE_READ_ACC_PID:
									reading_acc = 0;
									break;
									// other reading IDs
								default:
									break;
							}
						}
						break;

					default:
						break;
				}
			}

		}
	//}
}

int slave_response_RTC(void){
	get_time(&rtc_time);
	get_date(&rtc_date);
	Tx_Msg.data[0] = rtc_time.Hours;
	Tx_Msg.data[1] = rtc_time.Minutes;
	Tx_Msg.data[2] = rtc_time.Seconds;

	Tx_Msg.data[3] = rtc_date.Year;
	Tx_Msg.data[4] = rtc_date.Month;
	Tx_Msg.data[5] = rtc_date.Date;
	Tx_Msg.length = 6;
	SendMessage(&Tx_Msg);
}

int slave_send_RTC(void){
//	get_time(&rtc_time);
//	get_date(&rtc_date);
//	Tx_Msg.data[0] = rtc_time.Hours;
//	Tx_Msg.data[1] = rtc_time.Minutes;
//	Tx_Msg.data[2] = rtc_time.Seconds;
//
//	Tx_Msg.data[3] = rtc_date.Year;
//	Tx_Msg.data[4] = rtc_date.Month;
//	Tx_Msg.data[5] = rtc_date.Date;
//	Tx_Msg.data[6] = checksum(6, Tx_Msg.data);
//	Tx_Msg.length = 6;
//	SendMessage(&Tx_Msg);

	// TODO: create new function for this
//	allumer_phares();
	serial_puts("Sucessfully handle interrupt");
}


/*--- Send sync field and break ---*/
void LIN_SendBreak(void) {
    // Send break character (10 or more bits low
	// OKAY)
    USART3->CR1 |= USART_CR1_SBK; // Send break
    // USART3->DR = 0x1;
    while (!(USART3->SR & USART_SR_TC)); // Wait for transmission complete

}

void LIN_SendSync(void) {
    // Send Sync field (0x55)
    USART3->DR = 0x55;
    while (!(USART3->SR & USART_SR_TC)); // Wait for transmission complete
}


/*--- Transmit char ---*/
 
void UART_PutChar(uint8_t data)
{
	LINMSG msg;
	msg.ID = 0x67;
	msg.data[0] = data;
	msg.length = 8;

	//Load data to register
	USART3->DR = data;
	while(!(USART3->SR & 0x00000080));
	while(!(USART3->SR & 0x00000040));

}

/*--- Calculate lin checksum ---*/

uint8_t checksum(uint8_t length, uint8_t *data)
  {
  uint8_t ix;
  uint16_t check_sum = 0;

  for(ix = 0; ix < length-1; ix++)
    {
    check_sum += data[ix];
    if(check_sum >= 256){
      check_sum -= 255;
      }
    }

  return (uint8_t)(0xff - check_sum);
  }
