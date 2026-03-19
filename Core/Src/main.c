/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include <sys/stat.h>
#include "Merenja.h"
#include "DAQ_Config.h"

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
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

CRC_HandleTypeDef hcrc;

I2C_HandleTypeDef hi2c1;

RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi2;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for taskBlinky */
osThreadId_t taskBlinkyHandle;
const osThreadAttr_t taskBlinky_attributes = {
  .name = "taskBlinky",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for taskTrace */
osThreadId_t taskTraceHandle;
const osThreadAttr_t taskTrace_attributes = {
  .name = "taskTrace",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for taskDispecer */
osThreadId_t taskDispecerHandle;
const osThreadAttr_t taskDispecer_attributes = {
  .name = "taskDispecer",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for taskRadioComms */
osThreadId_t taskRadioCommsHandle;
const osThreadAttr_t taskRadioComms_attributes = {
  .name = "taskRadioComms",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for d2 */
osThreadId_t d2Handle;
const osThreadAttr_t d2_attributes = {
  .name = "d2",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for d3 */
osThreadId_t d3Handle;
const osThreadAttr_t d3_attributes = {
  .name = "d3",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for d4 */
osThreadId_t d4Handle;
const osThreadAttr_t d4_attributes = {
  .name = "d4",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for d5 */
osThreadId_t d5Handle;
const osThreadAttr_t d5_attributes = {
  .name = "d5",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for d6 */
osThreadId_t d6Handle;
const osThreadAttr_t d6_attributes = {
  .name = "d6",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for taskHwI2c */
osThreadId_t taskHwI2cHandle;
const osThreadAttr_t taskHwI2c_attributes = {
  .name = "taskHwI2c",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for taskHwSpi */
osThreadId_t taskHwSpiHandle;
const osThreadAttr_t taskHwSpi_attributes = {
  .name = "taskHwSpi",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for taskDigitalIn */
osThreadId_t taskDigitalInHandle;
const osThreadAttr_t taskDigitalIn_attributes = {
  .name = "taskDigitalIn",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for taskAnalogIn */
osThreadId_t taskAnalogInHandle;
const osThreadAttr_t taskAnalogIn_attributes = {
  .name = "taskAnalogIn",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* USER CODE BEGIN PV */

#define flg_ADC_DONE 0x1000

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_CRC_Init(void);
static void MX_I2C1_Init(void);
static void MX_RTC_Init(void);
static void MX_SPI2_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USB_OTG_FS_USB_Init(void);
void StartDefaultTask(void *argument);
void Blinky_start(void *argument);
void TraceTask_start(void *argument);
void Dispecer_start(void *argument);
void radioComms_start(void *argument);
void d2start(void *argument);
void d3start(void *argument);
void d4start(void *argument);
void d5s(void *argument);
void d6start(void *argument);
void HwI2C_start(void *argument);
void HwSpi_start(void *argument);
void DigitalIn_start(void *argument);
void AnalogIn_start(void *argument);

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
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_CRC_Init();
  MX_I2C1_Init();
  MX_RTC_Init();
  MX_SPI2_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USB_OTG_FS_USB_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of taskBlinky */
  taskBlinkyHandle = osThreadNew(Blinky_start, NULL, &taskBlinky_attributes);

  /* creation of taskTrace */
  taskTraceHandle = osThreadNew(TraceTask_start, NULL, &taskTrace_attributes);

  /* creation of taskDispecer */
  taskDispecerHandle = osThreadNew(Dispecer_start, NULL, &taskDispecer_attributes);

  /* creation of taskRadioComms */
  taskRadioCommsHandle = osThreadNew(radioComms_start, NULL, &taskRadioComms_attributes);

  /* creation of d2 */
  d2Handle = osThreadNew(d2start, NULL, &d2_attributes);

  /* creation of d3 */
  d3Handle = osThreadNew(d3start, NULL, &d3_attributes);

  /* creation of d4 */
  d4Handle = osThreadNew(d4start, NULL, &d4_attributes);

  /* creation of d5 */
  d5Handle = osThreadNew(d5s, NULL, &d5_attributes);

  /* creation of d6 */
  d6Handle = osThreadNew(d6start, NULL, &d6_attributes);

  /* creation of taskHwI2c */
  taskHwI2cHandle = osThreadNew(HwI2C_start, NULL, &taskHwI2c_attributes);

  /* creation of taskHwSpi */
  taskHwSpiHandle = osThreadNew(HwSpi_start, NULL, &taskHwSpi_attributes);

  /* creation of taskDigitalIn */
  taskDigitalInHandle = osThreadNew(DigitalIn_start, NULL, &taskDigitalIn_attributes);

  /* creation of taskAnalogIn */
  taskAnalogInHandle = osThreadNew(AnalogIn_start, NULL, &taskAnalogIn_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
	/* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV8;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enables the Clock Security System
  */
  HAL_RCC_EnableCSS();
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV8;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 6;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_5;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_6;
  sConfig.Rank = 2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_7;
  sConfig.Rank = 3;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_8;
  sConfig.Rank = 4;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_9;
  sConfig.Rank = 5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
  sConfig.Rank = 6;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief CRC Initialization Function
  * @param None
  * @retval None
  */
static void MX_CRC_Init(void)
{

  /* USER CODE BEGIN CRC_Init 0 */

  /* USER CODE END CRC_Init 0 */

  /* USER CODE BEGIN CRC_Init 1 */

  /* USER CODE END CRC_Init 1 */
  hcrc.Instance = CRC;
  if (HAL_CRC_Init(&hcrc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CRC_Init 2 */

  /* USER CODE END CRC_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0x0;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 0x1;
  sDate.Year = 0x0;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

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
  huart1.Init.BaudRate = 115200;
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
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_HalfDuplex_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USB_OTG_FS Initialization Function
  * @param None
  * @retval None
  */
static void MX_USB_OTG_FS_USB_Init(void)
{

  /* USER CODE BEGIN USB_OTG_FS_Init 0 */

  /* USER CODE END USB_OTG_FS_Init 0 */

  /* USER CODE BEGIN USB_OTG_FS_Init 1 */

  /* USER CODE END USB_OTG_FS_Init 1 */
  /* USER CODE BEGIN USB_OTG_FS_Init 2 */

  /* USER CODE END USB_OTG_FS_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

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
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(BOARD_LED0_GPIO_Port, BOARD_LED0_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, KBDsel_L_Pin|KBDsel_M_Pin|KBDsel_H_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : BOARD_LED0_Pin */
  GPIO_InitStruct.Pin = BOARD_LED0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(BOARD_LED0_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : KBDsel_L_Pin KBDsel_M_Pin KBDsel_H_Pin */
  GPIO_InitStruct.Pin = KBDsel_L_Pin|KBDsel_M_Pin|KBDsel_H_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : Exti_d1_Pin */
  GPIO_InitStruct.Pin = Exti_d1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(Exti_d1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : KBD2_Pin KBD3_Pin KBD0_Pin KBD1_Pin */
  GPIO_InitStruct.Pin = KBD2_Pin|KBD3_Pin|KBD0_Pin|KBD1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : Exti_d2_Pin Exti_d3_Pin */
  GPIO_InitStruct.Pin = Exti_d2_Pin|Exti_d3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA10 PA11 PA12 */
  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	uint32_t fl = (uint32_t) GPIO_Pin;
	osThreadFlagsSet(taskDigitalInHandle, fl); // prosledim broj pina kao da je flag, pa neka tamo dalje odlucuju
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
	osThreadFlagsSet(taskAnalogInHandle, 0xf000);		// TODO daj neki smisleni fleg
}


/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN 5 */
	/* Infinite loop */
	for (;;) {
		osDelay(1);
	}
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_Blinky_start */
/**
* @brief Function implementing the blinky thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Blinky_start */
void Blinky_start(void *argument)
{
  /* USER CODE BEGIN Blinky_start */
	/* Infinite loop */
	for (;;) {
		uint32_t flg = osThreadFlagsWait(UINT32_MAX, osFlagsWaitAny, cfg_GetHw_HbPer());
		if (flg == osFlagsErrorTimeout) {
			// TODO blink heartbeat
		} else {
			// TODO blink nesto drugo
		}
		osDelay(1);
	}
  /* USER CODE END Blinky_start */
}

/* USER CODE BEGIN Header_TraceTask_start */
/**
* @brief Function implementing the traceTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_TraceTask_start */
void TraceTask_start(void *argument)
{
  /* USER CODE BEGIN TraceTask_start */
	/* Infinite loop */
	for (;;) {
		osDelay(1);
	}
  /* USER CODE END TraceTask_start */
}

/* USER CODE BEGIN Header_Dispecer_start */
/**
* @brief Function implementing the dispecer thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Dispecer_start */
void Dispecer_start(void *argument)
{
  /* USER CODE BEGIN Dispecer_start */
	/* Infinite loop */
	for (;;) {
		osDelay(1);
	}
  /* USER CODE END Dispecer_start */
}

/* USER CODE BEGIN Header_radioComms_start */
/**
* @brief Function implementing the radioComms thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_radioComms_start */
void radioComms_start(void *argument)
{
  /* USER CODE BEGIN radioComms_start */
	/* Infinite loop */
	for (;;) {
		osDelay(1);
	}
  /* USER CODE END radioComms_start */
}

/* USER CODE BEGIN Header_d2start */
/**
* @brief Function implementing the d2 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_d2start */
void d2start(void *argument)
{
  /* USER CODE BEGIN d2start */
	/* Infinite loop */
	for (;;) {
		osDelay(1);
	}
  /* USER CODE END d2start */
}

/* USER CODE BEGIN Header_d3start */
/**
* @brief Function implementing the d3 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_d3start */
void d3start(void *argument)
{
  /* USER CODE BEGIN d3start */
	/* Infinite loop */
	for (;;) {
		osDelay(1);
	}
  /* USER CODE END d3start */
}

/* USER CODE BEGIN Header_d4start */
/**
* @brief Function implementing the d4 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_d4start */
void d4start(void *argument)
{
  /* USER CODE BEGIN d4start */
	/* Infinite loop */
	for (;;) {
		osDelay(1);
	}
  /* USER CODE END d4start */
}

/* USER CODE BEGIN Header_d5s */
/**
* @brief Function implementing the d5 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_d5s */
void d5s(void *argument)
{
  /* USER CODE BEGIN d5s */
	/* Infinite loop */
	for (;;) {
		osDelay(1);
	}
  /* USER CODE END d5s */
}

/* USER CODE BEGIN Header_d6start */
/**
* @brief Function implementing the d6 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_d6start */
void d6start(void *argument)
{
  /* USER CODE BEGIN d6start */
	/* Infinite loop */
	for (;;) {
		osDelay(1);
	}
  /* USER CODE END d6start */
}

/* USER CODE BEGIN Header_HwI2C_start */
/**
* @brief Function implementing the hwi2c thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_HwI2C_start */
void HwI2C_start(void *argument)
{
  /* USER CODE BEGIN HwI2C_start */
	/* Infinite loop */
	for (;;) {
		osDelay(1);
	}
  /* USER CODE END HwI2C_start */
}

/* USER CODE BEGIN Header_HwSpi_start */
/**
* @brief Function implementing the hwspi thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_HwSpi_start */
void HwSpi_start(void *argument)
{
  /* USER CODE BEGIN HwSpi_start */
	/* Infinite loop */
	for (;;) {
		osDelay(1);
	}
  /* USER CODE END HwSpi_start */
}

/* USER CODE BEGIN Header_DigitalIn_start */
/**
* @brief Function implementing the taskDigitalIn thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_DigitalIn_start */
void DigitalIn_start(void *argument)
{
  /* USER CODE BEGIN DigitalIn_start */
		uint32_t extPinovi;
	/* Infinite loop */
	for (;;) {
		uint32_t rez = osThreadFlagsWait(UINT32_MAX, osFlagsWaitAny, osWaitForever);
		switch (rez) {
			case osFlagsErrorTimeout:
				// ni jedan digitalni ulaz se nije promenio. ne radim nista
				break;

			default:
				extPinovi = HAL_GPIO_ReadPin(Exti_d1_GPIO_Port, Exti_d1_Pin);
				extPinovi &= HAL_GPIO_ReadPin(Exti_d2_GPIO_Port, Exti_d2_Pin);
				extPinovi &= HAL_GPIO_ReadPin(Exti_d3_GPIO_Port, Exti_d3_Pin);
				setDigital(extPinovi);
				break;
		}
		osDelay(1);
		osThreadYield();
	}
  /* USER CODE END DigitalIn_start */
}

/* USER CODE BEGIN Header_AnalogIn_start */
/**
* @brief Function implementing the taskAnalogIn thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_AnalogIn_start */
void AnalogIn_start(void *argument)
{
  /* USER CODE BEGIN AnalogIn_start */
	uint32_t AD_REZULT[hadc1.Init.NbrOfConversion];
for (;;) {

	uint32_t flg = osThreadFlagsWait(UINT32_MAX, osFlagsWaitAny, ADC_REPETITION_PERIOD_mS);
	// kad istekne ADC_REPETITION_PERIOD_mS ili ako pre vremena dobijem fleg, startujem merenje
	switch (flg) {
		case osFlagsErrorTimeout:
			// startujem merenje jer je istekao ADC_REPETITION_PERIOD_mS
			HAL_ADC_Start_DMA(&hadc1, AD_REZULT, hadc1.Init.NbrOfConversion);	// posle ovoga cekam DMA da zavrsi
			break;

		case flg_ADC_DONE:
			// ADC_DMA transfer je zavrsen
			for (int i = 0; i < hadc1.Init.NbrOfConversion; i++) {
				setAnalog(i, AD_REZULT[i]);
			}
			break;

		default:
			// TODO sta ovde? nista?
			break;
	}
	osDelay(1);
	osThreadYield();
}
/* Infinite loop */
  /* USER CODE END AnalogIn_start */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM11 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM11)
  {
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
	while (1) {
	}
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
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
