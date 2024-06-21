/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "can.h"
#include "phares.h"
#include "commodos.h"
#include "UART_LIN.h"
#include "usart2.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
USART_HandleTypeDef husart2;

osThreadId linSenderTaskHandle;
osThreadId canReceiverTaskHandle;
osThreadId myTask04Handle;
osThreadId canTrackerTaskHandle;
osMessageQId linCanQueueHandle;
osMutexId myMutex01Handle;
/* USER CODE BEGIN PV */

unsigned char DisplayData[];
LINMSG Tx_Msg;
LINMSG Rx_Msg;
int new_request;
int i = 0;
int actual_data;
int should_read_id = 0;
int should_read_data = 0;
int should_checksum = 0;
int should_synch = 1;
int deactivate_interrupt = 0;
int id_readed = 0;
int sync_readed = 0;
//extern int action;
int reading_acc = 0;

// CAN integ
volatile CAN_frame CAN_RxMessage;
volatile CAN_frame CAN_RxMessages[10];
int i_can;
volatile int read_index;
volatile uint8_t emergency_data_read = 0;
volatile uint8_t can_read_data;
extern buffer_has_more_cycle;
extern int total_received;
volatile int lin_readed_index = 0;
extern int phare_state;
CAN_frame my_frame;
volatile CAN_frame received_frame;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_Init(void);
void StartLinSenderTask(void const * argument);
void StartCanReceiverTask(void const * argument);
void StartTask04(void const * argument);
void StartCanTrackerTask(void const * argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int _write(int file, char *ptr, int len){
	HAL_USART_Transmit(&husart2, (uint8_t *)ptr, len, 10);
	return len;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_Init();
  /* USER CODE BEGIN 2 */
  UART_Init();
  CAN_config();
  CAN_config_filter(0, 0, 0x0, 0x10520312, 0x10035110);
  /* USER CODE END 2 */

  /* Create the mutex(es) */
  /* definition and creation of myMutex01 */
  osMutexDef(myMutex01);
  myMutex01Handle = osMutexCreate(osMutex(myMutex01));

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* definition and creation of linCanQueue */
  osMessageQDef(linCanQueue, 16, uint16_t);
  linCanQueueHandle = osMessageCreate(osMessageQ(linCanQueue), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of linSenderTask */
  osThreadDef(linSenderTask, StartLinSenderTask, osPriorityHigh, 0, 128);
  linSenderTaskHandle = osThreadCreate(osThread(linSenderTask), NULL);

  /* definition and creation of canReceiverTask */
  osThreadDef(canReceiverTask, StartCanReceiverTask, osPriorityNormal, 0, 128);
  canReceiverTaskHandle = osThreadCreate(osThread(canReceiverTask), NULL);

  /* definition and creation of myTask04 */
  osThreadDef(myTask04, StartTask04, osPriorityIdle, 0, 128);
  myTask04Handle = osThreadCreate(osThread(myTask04), NULL);

  /* definition and creation of canTrackerTask */
  osThreadDef(canTrackerTask, StartCanTrackerTask, osPriorityIdle, 0, 128);
  canTrackerTaskHandle = osThreadCreate(osThread(canTrackerTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  husart2.Instance = USART2;
  husart2.Init.BaudRate = 19200;
  husart2.Init.WordLength = USART_WORDLENGTH_8B;
  husart2.Init.StopBits = USART_STOPBITS_1;
  husart2.Init.Parity = USART_PARITY_NONE;
  husart2.Init.Mode = USART_MODE_TX_RX;
  husart2.Init.CLKPolarity = USART_POLARITY_LOW;
  husart2.Init.CLKPhase = USART_PHASE_1EDGE;
  husart2.Init.CLKLastBit = USART_LASTBIT_DISABLE;
  if (HAL_USART_Init(&husart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartLinSenderTask */
/**
  * @brief  Function implementing the linSenderTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartLinSenderTask */
void StartLinSenderTask(void const * argument)
{
  /* USER CODE BEGIN 5 */
	int8_t lin_received_cnt = 0;
	uint8_t can_send_lin = 0;
  /* Infinite loop */
  for(;;)
  {
	  osSignalWait(1, osWaitForever);
	  lin_received_cnt++;
	  can_send_lin = 1;
	  int data = (unsigned int) CAN_RxMessage.data[0];

	  Tx_Msg.ID = LIN_SLAVE_WRITE_RTC_PID;
	  Tx_Msg.length = LIN_SLAVE_WRITE_RTC_LENGTH;

	  switch (CAN_RxMessage.STDID) {

		case COMMODO_CLIGNOTANT_RETURN_BASE:
				switch (data) {
					case COMMODO_CLIGNOTANT_REPOS_RETOURN_VALUE:
						Tx_Msg.ID = LIN_CLIGNO_STOP;
						printf("CLIGNO STOP \n\r");
						break;

					case COMMODO_CLIGNOTANT_DROIT_RETOURN_VALUE:
						Tx_Msg.ID = LIN_CLIGNO_DROIT;
						printf("CLIGNO DROIT \n\r");
						break;

					case COMMODO_CLIGNOTANT_GAUCHE_RETOURN_VALUE:
						Tx_Msg.ID = LIN_CLIGNO_GAUCHE;
						printf("CLIGNO GAUCHE \n\r");
						break;
					default:
						can_send_lin = 0;
						break;
				}
			break;

		case COMMODO_PHARE_RETURN_BASE:
				switch (data) {
					case COMMODO_PHARE_ALLUMER_RETOURN_VALUE:
						Tx_Msg.ID = LIN_PHARE_AVANT;
						printf("PHARES AVANT \n\r");
						break;

					case COMMODO_PHARE_VEILLEUSE_RETOURN_VALUE:
						Tx_Msg.ID = LIN_PHARE_ARRIERE;
						printf("PHARES ARRIERES \n\r");
						break;

					case COMMODO_PHARE_AUTO_RETOURN_VALUE:
						Tx_Msg.ID = LIN_PHARE_STOP;
						printf("PHARES STOP \n\r");
						break;
					default:
						can_send_lin = 0;
						break;
				}
			break;
		default:
			can_send_lin = 0;
			break;
	}

//	  Rx_Msg.ID = LIN_SLAVE_READ_RTC_PID;
//	  Rx_Msg.length = LIN_SLAVE_READ_RTC_LENGTH;
	  if(can_send_lin){

		  SendRequest(&Tx_Msg);
		  HAL_Delay(500);
	  }

	  printf("\n\r=================================\n\r");

	  osDelay(1000);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartCanReceiverTask */
/**
* @brief Function implementing the canReceiverTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartCanReceiverTask */
void StartCanReceiverTask(void const * argument)
{
  /* USER CODE BEGIN StartCanReceiverTask */
	int8_t can_received_cnt = 0;

	osEvent canReadedTracker;
	int8_t executed_once = 0;

	CAN_frame auto_frame;
	  auto_frame.STDID = 0x10530310;
	  auto_frame.DLC = 1;
	  auto_frame.RTR = 0;
  /* Infinite loop */
  for(;;)
  {
//	  if (!executed_once){
//		  executed_once = 1;
//	  }else{
//
//	  }
//	  printf("Blablalba\n\r");
//	  auto_frame.data[0] = 1;
//	  	  CAN_sendFrame(auto_frame);
//
//	  	  HAL_Delay(2000);
//
//	  	  auto_frame.data[0] = (8 & ~(1));
//	  	  CAN_sendFrame(auto_frame);
//	  	  HAL_Delay(2000);

	  	  // REAL CODE
	  canReadedTracker = osMessageGet(linCanQueueHandle, 1000);
	  if(canReadedTracker.value.v == 0x1){
//		  printf("Can Readed - Let's send LIN request \n \r");
		can_received_cnt++;
//		printf("CAN Received n°%d :: (ID = %d, Value= %d) \n\r", can_received_cnt, CAN_RxMessage.STDID, CAN_RxMessage.data[0]);
		osSignalSet(linSenderTaskHandle, 1);
		osDelay(1000);
	  }
  }
//    osSignalWait(1, osWaitForever);
  /* USER CODE END StartCanReceiverTask */
}

/* USER CODE BEGIN Header_StartTask04 */
/**
* @brief Function implementing the myTask04 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask04 */
void StartTask04(void const * argument)
{
  /* USER CODE BEGIN StartTask04 */
	int cnt = 0;
  /* Infinite loop */
  for(;;)
  {
    osSignalWait(1, osWaitForever);
    cnt++;
    printf("Donnée Can recue %d \n \r", cnt);
    HAL_Delay(500);

	osDelay(1000);
  }
  /* USER CODE END StartTask04 */
}

/* USER CODE BEGIN Header_StartCanTrackerTask */
/**
* @brief Function implementing the canTrackerTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartCanTrackerTask */
void StartCanTrackerTask(void const * argument)
{
  /* USER CODE BEGIN StartCanTrackerTask */
	  int8_t cnt = 0;
  /* Infinite loop */
  for(;;)
  {
	printf("Waiting for CAN \n \r");
	osSignalWait(1, osWaitForever);
	cnt++;
	CAN_readFrame();
	printf("Readed CAN n° %d \n \r", cnt);
	HAL_Delay(100);

//	CAN_readFrame();
//	HAL_Delay(100);

//	printf("CAN Readed \n \r");
//	HAL_Delay(100);
	//	HAL_Delay(100);
	if(CAN_RxMessage.STDID == COMMODO_PHARE_RETURN_ID){
		if(CAN_RxMessage.data[0] == COMMODO_PHARE_RETURN_VALUE){
			uint8_t toto = 0;
		}else{
			uint8_t toto = 1;
		}
	}

	osDelay(1000);
//	osSignalSet(canTrackerTaskHandle, 1);

  }
  /* USER CODE END StartCanTrackerTask */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
