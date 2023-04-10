/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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

//const uint8_t zg[]= {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x0};//знакогенератор 0123456789пробел
const uint8_t zgnr[]= {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x0};//знакогенератор 0123456789пробел
uint8_t statusIntLed = 0;// состояние встроенного светодиода
uint16_t statusChangeCounter = 1000; //счетчик изменений встроенного светодиода
uint8_t countInd = 0; //счетчик динамической индикации
uint8_t digs[4] = {10,10,10,10};// 10 - "пусто" на индикаторе
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);

void Delay( void )
{
    uint32_t i,j;
    for( i = 0; i <= 50000; i++ ) //50000
        for( j = 0; j <= 5; j++ );
}
void setSegment(uint8_t segm) // вывод символа в текущий сегмент
{
segm = zgnr[segm];
//segm = 6;
//декодирование seg

if (segm & 1) {HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);} else {HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);}; 
if (segm & 2) {HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);} else {HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);}; 
if (segm & 4) {HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);} else {HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);}; 
if (segm & 8) {HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);} else {HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);}; 
if (segm & 16) {HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);} else {HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);}; 
if (segm & 32) {HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);} else {HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);}; 
if (segm & 64) {HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);} else {HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);}; 
}

void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
 HAL_IncTick();
   if (!statusChangeCounter)  { //счетчие =0, обработка мигания встроенным светодиодом
                                 statusChangeCounter = 1000;
//  HAL_GPIO_WritePin (GPIOA,GPIO_PIN_7, GPIO_PIN_SET); //                                 
                                 if (statusIntLed) {HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
                                                    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);}
                                                    else  {HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
                                                    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);        
                                                                   };
                                 statusIntLed =!statusIntLed; 
    }
    statusChangeCounter--;
    // динамическая индикация
    // all digits off	
HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET); 
HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET); 
HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET); 
HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET); 			
        
			setSegment(digs[countInd]); // вывод символа в текущий сегмент
            if (countInd==0 ) {//digit 1 on
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);};
            if (countInd==1 ) {//digit 2 on
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);};
            if (countInd==2 ) {//digit 3 on
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);};
            if (countInd==3 ) {//digit 4 on
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);};
            countInd++;
			if (countInd>3) {countInd = 0;};    
      
// USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}


/*
uint8_t statusIntLed = 0;// состояние встроенного светодиода
uint16_t statusChangeCounter = 1000; //счетчик изменений встроенного светодиода
uint8_t count = 0; //счетчик динамической индикации
uint8_t digs[4] = {10,10,10,10};// 10 - "пусто" на индикаторе
*/
/*void SysTick_Handler (void) // прерывание от системного таймера
{
    if (!statusChangeCounter)  { //счетчие =0, обработка мигания встроенным светодиодом
                                 statusChangeCounter = 1000;
//  HAL_GPIO_WritePin (GPIOA,GPIO_PIN_7, GPIO_PIN_SET); //                                 
                                 if (statusIntLed) {HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
                                                    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);}
                                                    else  {HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
                                                    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);        
                                                                   };
                                 statusIntLed =!statusIntLed; 
    }
    statusChangeCounter--;
    // динамическая индикация
    // all digits off	
HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET); 
HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET); 
HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET); 
HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET); 			
        
			setSegment(digs[countInd]); // вывод символа в текущий сегмент
            if (countInd==0 ) {//digit 1 on
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);};
            if (countInd==1 ) {//digit 2 on
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);};
            if (countInd==2 ) {//digit 3 on
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);};
            if (countInd==3 ) {//digit 4 on
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);};
            countInd++;
			if (countInd>3) {countInd = 0;};    
    
}
*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
            uint16_t value,tempValue;
    uint8_t keyRrattling;
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
    SysTick_Config(SystemCoreClock/1000); //интервал 1мкс
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
      value = 0;  
            // выделение  тысяч
    digs[0] = value / 1000;
    tempValue = value - digs[0] * 1000;
    // выделение сотен
    digs[1] =  tempValue / 100;
    tempValue -=digs[1] * 100;    
    // выделение десятков
    digs[2] = tempValue / 10;
    tempValue -=digs[2] * 10;    
    // выделение единиц
    digs[3] = (uint8_t) tempValue;

  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */


//if (HAL_GPIO_ReadPin (BUTN_GPIO_Port, BUTN_Pin) == 0) // 
/*HAL_GPIO_WritePin (GPIOA,GPIO_PIN_7, GPIO_PIN_SET); //
HAL_Delay(100);     
HAL_GPIO_WritePin (GPIOA,GPIO_PIN_7, GPIO_PIN_RESET); //
HAL_Delay(100);*/
while ((USART1->SR & USART_SR_TXE) == 0) {}
USART1->DR = 'A';

 digs[0] = value / 1000;
    tempValue = value - digs[0] * 1000;
    // выделение сотен
    digs[1] =  tempValue / 100;
    tempValue -=digs[1] * 100;    
    // выделение десятков
    digs[2] = tempValue / 10;
    tempValue -=digs[2] * 10;    
    // выделение единиц
    digs[3] = (uint8_t) tempValue;
  // убирание нулей справа
    if (digs[0]==0) {digs[0]=10;};//х1000
    if ((digs[1]==0) && (digs[0]==10) ){digs[1]=10;};//х100
    if ((digs[2]==0) && (digs[1]==10) ){digs[2]=10;};//х10
   
    // !!!!! переделать, не держать обработку в опросе, возможно разнести на 2 части
    //ожидание устойчивой 1
    wait1:keyRrattling = 100;
    while (keyRrattling>0) 
    { //{if (HAL_GPIO_ReadPin (BUTN_GPIO_Port, BUTN_Pin) == 0) 
    if (HAL_GPIO_ReadPin (GPIOB, GPIO_PIN_6) !=1) {goto wait1;}
    keyRrattling--;
    }
    //ожидание устойчивоuj 0
    wait0:keyRrattling = 100;
    while (keyRrattling>0) 
    {
    if (HAL_GPIO_ReadPin (GPIOB, GPIO_PIN_6) ==1) {goto wait0;}
    keyRrattling--;
    }
    if (value <9999) {value++;};
   
    
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(inLed_GPIO_Port, inLed_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, digA_Pin|digB_Pin|digC_Pin|digD_Pin
                          |digE_Pin|digF_Pin|digG_Pin|modeLed_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, addr1_Pin|addr2_Pin|addr3_Pin|addr4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : inLed_Pin */
  GPIO_InitStruct.Pin = inLed_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(inLed_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : digA_Pin digB_Pin digC_Pin digD_Pin
                           digE_Pin digF_Pin digG_Pin modeLed_Pin */
  GPIO_InitStruct.Pin = digA_Pin|digB_Pin|digC_Pin|digD_Pin
                          |digE_Pin|digF_Pin|digG_Pin|modeLed_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : addr1_Pin addr2_Pin addr3_Pin addr4_Pin */
  GPIO_InitStruct.Pin = addr1_Pin|addr2_Pin|addr3_Pin|addr4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : SW1_Pin */
  GPIO_InitStruct.Pin = SW1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(SW1_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

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
