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
#include <stdbool.h>
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
/* Definitions for TaskBlinky */
osThreadId_t TaskBlinkyHandle;
const osThreadAttr_t TaskBlinky_attributes = {
  .name = "TaskBlinky",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for TaskTrace */
osThreadId_t TaskTraceHandle;
const osThreadAttr_t TaskTrace_attributes = {
  .name = "TaskTrace",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for TaskDispecer */
osThreadId_t TaskDispecerHandle;
const osThreadAttr_t TaskDispecer_attributes = {
  .name = "TaskDispecer",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for TaskRadioComms */
osThreadId_t TaskRadioCommsHandle;
const osThreadAttr_t TaskRadioComms_attributes = {
  .name = "TaskRadioComms",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for TaskWaitForTrigg */
osThreadId_t TaskWaitForTriggHandle;
const osThreadAttr_t TaskWaitForTrigg_attributes = {
  .name = "TaskWaitForTrigg",
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
/* Definitions for TaskHwi2c */
osThreadId_t TaskHwi2cHandle;
const osThreadAttr_t TaskHwi2c_attributes = {
  .name = "TaskHwi2c",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for TaskHwSpi */
osThreadId_t TaskHwSpiHandle;
const osThreadAttr_t TaskHwSpi_attributes = {
  .name = "TaskHwSpi",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for TaskDigitalIn */
osThreadId_t TaskDigitalInHandle;
const osThreadAttr_t TaskDigitalIn_attributes = {
  .name = "TaskDigitalIn",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for TaskAnalogIn */
osThreadId_t TaskAnalogInHandle;
const osThreadAttr_t TaskAnalogIn_attributes = {
  .name = "TaskAnalogIn",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for EvtMeasurementStart */
osEventFlagsId_t EvtMeasurementStartHandle;
const osEventFlagsAttr_t EvtMeasurementStart_attributes = {
  .name = "EvtMeasurementStart"
};
/* Definitions for EvtMeasurementDone */
osEventFlagsId_t EvtMeasurementDoneHandle;
const osEventFlagsAttr_t EvtMeasurementDone_attributes = {
  .name = "EvtMeasurementDone"
};
/* Definitions for EvtSpare1 */
osEventFlagsId_t EvtSpare1Handle;
const osEventFlagsAttr_t EvtSpare1_attributes = {
  .name = "EvtSpare1"
};
/* Definitions for EvtSpare2 */
osEventFlagsId_t EvtSpare2Handle;
const osEventFlagsAttr_t EvtSpare2_attributes = {
  .name = "EvtSpare2"
};
/* USER CODE BEGIN PV */

// maksimalni flegovi
uint32_t flg_MAX = 0x7FFFFFFFU;
// ovi se salju dispeceru
#define flg_DIGITAL_IRQ					0x0000'0000'00000001			// EXTI callback: digitalni ulaz se promenio
#define flg_DIGITAL_REQUEST				0x0000'0000'00000010			// neki task zahteva ocitavanje digitalnih ulaza
#define flg_DIGITAL_PROCESSING_DONE		0x0000'0000'00000100			// svi digitalni pinovi procitani i spremni za slanje
#define flg_DIGITAL_TRIGGERED			0x0000'0000'00001000			// svi digitalni pinovi procitani i spremni za slanje

#define flg_ADC_CONV_CPLT_IRQ			0x0000'0000'00010010			// ADC IRQ callback: zavrsena adc konverzija u pozadini i DMA je obavio svoje
#define flg_ADC_REQUEST					0x0000'0000'00100000			// neki task zahteva adc konverziju
#define flg_ANALOG_PROCESSING_DONE		0x0000'0000'01000000			// zavrsena analogna merenja ali ni jedan threshold nije prekoracen
#define flg_ANALOG_TRIGGERED			0x0000'0000'10000000			// zavrsena analogna merenja i neka vrednost je prekoracena
#define evtflg_SPARE1_TRIGGER			0x
#define evtflg_SPARE2_TRIGGER			0x
#define evtflg_SPARE3_TRIGGER			0x
#define evtflg_SPARE4_TRIGGER			0x


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
void StartTaskBlinky(void *argument);
void StartTaskTrace(void *argument);
void StartTaskDispecer(void *argument);
void StartTaskRadioComms(void *argument);
void StartTaskWaitForTrigg(void *argument);
void d3start(void *argument);
void d4start(void *argument);
void d5s(void *argument);
void d6start(void *argument);
void StartTaskHwi2c(void *argument);
void StartTaskHwSpi(void *argument);
void StartTaskDigitalIn(void *argument);
void StartTaskAnalogIn(void *argument);

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

  /* creation of TaskBlinky */
  TaskBlinkyHandle = osThreadNew(StartTaskBlinky, NULL, &TaskBlinky_attributes);

  /* creation of TaskTrace */
  TaskTraceHandle = osThreadNew(StartTaskTrace, NULL, &TaskTrace_attributes);

  /* creation of TaskDispecer */
  TaskDispecerHandle = osThreadNew(StartTaskDispecer, NULL, &TaskDispecer_attributes);

  /* creation of TaskRadioComms */
  TaskRadioCommsHandle = osThreadNew(StartTaskRadioComms, NULL, &TaskRadioComms_attributes);

  /* creation of TaskWaitForTrigg */
  TaskWaitForTriggHandle = osThreadNew(StartTaskWaitForTrigg, NULL, &TaskWaitForTrigg_attributes);

  /* creation of d3 */
  d3Handle = osThreadNew(d3start, NULL, &d3_attributes);

  /* creation of d4 */
  d4Handle = osThreadNew(d4start, NULL, &d4_attributes);

  /* creation of d5 */
  d5Handle = osThreadNew(d5s, NULL, &d5_attributes);

  /* creation of d6 */
  d6Handle = osThreadNew(d6start, NULL, &d6_attributes);

  /* creation of TaskHwi2c */
  TaskHwi2cHandle = osThreadNew(StartTaskHwi2c, NULL, &TaskHwi2c_attributes);

  /* creation of TaskHwSpi */
  TaskHwSpiHandle = osThreadNew(StartTaskHwSpi, NULL, &TaskHwSpi_attributes);

  /* creation of TaskDigitalIn */
  TaskDigitalInHandle = osThreadNew(StartTaskDigitalIn, NULL, &TaskDigitalIn_attributes);

  /* creation of TaskAnalogIn */
  TaskAnalogInHandle = osThreadNew(StartTaskAnalogIn, NULL, &TaskAnalogIn_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Create the event(s) */
  /* creation of EvtMeasurementStart */
  EvtMeasurementStartHandle = osEventFlagsNew(&EvtMeasurementStart_attributes);

  /* creation of EvtMeasurementDone */
  EvtMeasurementDoneHandle = osEventFlagsNew(&EvtMeasurementDone_attributes);

  /* creation of EvtSpare1 */
  EvtSpare1Handle = osEventFlagsNew(&EvtSpare1_attributes);

  /* creation of EvtSpare2 */
  EvtSpare2Handle = osEventFlagsNew(&EvtSpare2_attributes);

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
  HAL_NVIC_SetPriority(EXTI4_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	// uint32_t fl = (uint32_t) GPIO_Pin;
	osThreadFlagsSet(TaskDispecerHandle, flg_DIGITAL_IRQ);				// desio se EXTI
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
	osThreadFlagsSet(TaskDispecerHandle, flg_ADC_CONV_CPLT_IRQ);		// ADC merenje zavrseno u pozadini
}


/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument) {
	/* USER CODE BEGIN 5 */
	/* Infinite loop */
	for (;;) {
		osDelay(100);
	}
	/* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartTaskBlinky */
/**
* @brief Function implementing the TaskBlinky thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskBlinky */
void StartTaskBlinky(void *argument)
{
  /* USER CODE BEGIN StartTaskBlinky */
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
  /* USER CODE END StartTaskBlinky */
}

/* USER CODE BEGIN Header_StartTaskTrace */
/**
* @brief Function implementing the TaskTrace thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskTrace */
void StartTaskTrace(void *argument)
{
  /* USER CODE BEGIN StartTaskTrace */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartTaskTrace */
}

/* USER CODE BEGIN Header_StartTaskDispecer */
/**
* @brief Function implementing the TaskDispecer thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskDispecer */
void StartTaskDispecer(void *argument)
{
  /* USER CODE BEGIN StartTaskDispecer */
  /* Infinite loop */
  for(;;){
  	uint32_t rez = osThreadFlagsWait(flg_MAX, osFlagsWaitAny, osWaitForever);
  	switch (rez) {
  		case osFlagsErrorTimeout:
  			// za sada nista
  			break;

  		default:
  			osThreadFlagsSet(TaskDigitalInHandle, flg_DIGITAL_REQUEST);
  			osThreadFlagsSet(TaskAnalogInHandle, flg_ADC_REQUEST);
  			break;
  	}
  	osDelay(1);
  	osThreadYield();

  }
  /* USER CODE END StartTaskDispecer */
}

/* USER CODE BEGIN Header_StartTaskRadioComms */
/**
* @brief Function implementing the TaskRadioComms thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskRadioComms */
void StartTaskRadioComms(void *argument)
{
  /* USER CODE BEGIN StartTaskRadioComms */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartTaskRadioComms */
}

/* USER CODE BEGIN Header_StartTaskWaitForTrigg */
/**
* @brief Function implementing the TaskWaitForTrigg thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskWaitForTrigg */
void StartTaskWaitForTrigg(void *argument) {
	/* USER CODE BEGIN StartTaskWaitForTrigg */
	/* Infinite loop */
	for (;;) {
		// rezult = osEventFlagsWait(EvtMeasurementDoneHandle, 0x7fffFFFF, osFlagsWaitAny, osWaitForever);
		// if (rezult & 0x80000000U) {
		// 	// Handle Error (e.g., osFlagsErrorTimeout) TODO
		//
		// } else {
		// 	// Use the result safely
		// 	uint32_t cleanFlags = rezult & 0x7FFFFFFFU;
		// }
		osDelay(10);
	}
	/* USER CODE END StartTaskWaitForTrigg */
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

/* USER CODE BEGIN Header_StartTaskHwi2c */
/**
* @brief Function implementing the TaskHwi2c thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskHwi2c */
void StartTaskHwi2c(void *argument)
{
  /* USER CODE BEGIN StartTaskHwi2c */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartTaskHwi2c */
}

/* USER CODE BEGIN Header_StartTaskHwSpi */
/**
* @brief Function implementing the TaskHwSpi thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskHwSpi */
void StartTaskHwSpi(void *argument)
{
  /* USER CODE BEGIN StartTaskHwSpi */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartTaskHwSpi */
}

/* USER CODE BEGIN Header_StartTaskDigitalIn */
/**
* @brief Function implementing the TaskDigitalIn thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskDigitalIn */
void StartTaskDigitalIn(void *argument)
{
  /* USER CODE BEGIN StartTaskDigitalIn */
	// ovde se dolazi iz HAL_GPIO_EXTI_Callback
	// irq callback salje broj pina kao thread flag
	uint32_t extPinovi;
  /* Infinite loop */
	for (;;) {
		uint32_t rez = osThreadFlagsWait(flg_MAX, osFlagsWaitAny, osWaitForever);
		uint32_t prethodni;
		switch (rez) {
			case osFlagsErrorTimeout:
				// ni jedan digitalni ulaz se nije promenio. ne radim nista
				break;

				default:
				// bilo koji flag -> primio sam EXTI sa nekog digitalnog pina ili je trazeno merenje zbog promene na ANALOGNOM delu
				// procitam vrednosti i posaljem u measurement set
				extPinovi = HAL_GPIO_ReadPin(Exti_d1_GPIO_Port, Exti_d1_Pin);
				extPinovi &= HAL_GPIO_ReadPin(Exti_d2_GPIO_Port, Exti_d2_Pin);
				extPinovi &= HAL_GPIO_ReadPin(Exti_d3_GPIO_Port, Exti_d3_Pin);
				prethodni = getDigitalResult();
				if (prethodni == extPinovi) {
					osEventFlagsClear(EvtMeasurementDoneHandle, flg_DIGITAL_TRIGGERED);
				} else {
					setDigitalResult(extPinovi);
					osEventFlagsSet(EvtMeasurementDoneHandle, flg_DIGITAL_TRIGGERED);
				}
				osEventFlagsSet(EvtMeasurementDoneHandle, flg_DIGITAL_PROCESSING_DONE);				// signaliziram zavrsetak
				break;
		}
		osDelay(1);
		osThreadYield();
	}
  /* USER CODE END StartTaskDigitalIn */
}

/* USER CODE BEGIN Header_StartTaskAnalogIn */
/**
* @brief Function implementing the TaskAnalogIn thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskAnalogIn */
void StartTaskAnalogIn(void *argument)
{
  /* USER CODE BEGIN StartTaskAnalogIn */
	uint32_t AD_REZULT[hadc1.Init.NbrOfConversion];
  /* Infinite loop */
	for (;;) {

		uint32_t flg = osThreadFlagsWait(flg_MAX, osFlagsWaitAny, ADC_REPETITION_PERIOD_mS);
		// analogno merenje svakih ADC_REPETITION_PERIOD_mS, ili ako pre vremena dobijem fleg zbog nekog drugog trigera
		switch (flg) {
			case osFlagsErrorTimeout:
			case flg_ADC_REQUEST:
				// ako istekne ADC_REPETITION_PERIOD_mS ili neki drugi task zatrazi merenje
				HAL_ADC_Start_DMA(&hadc1, AD_REZULT, hadc1.Init.NbrOfConversion);	// posle ovoga cekam DMA da zavrsi
				break;

			case flg_ADC_CONV_CPLT_IRQ:
				// ADC_DMA transfer je zavrsen
				bool desioSeTrig;
				desioSeTrig = false;		// FREEZE: ovde setujem varijablu a tek na kraju SAMO JEDNOM SETUJEM thread flag
				for (int i = 0; i < hadc1.Init.NbrOfConversion; i++) {
					setAnalogResult(i, AD_REZULT[i]);

					// THRESHOLD > 0: alarmiram prekoracenje IZNAD
					if (AD_THRESHOLD__DEBUG >= 0) {
						if (AD_REZULT[i] > (AD_THRESHOLD__DEBUG)) {
							desioSeTrig = true;		// preko thresholda
						} else if (AD_REZULT[i] <= (AD_THRESHOLD__DEBUG - AD_HYSTERSIS__DEBUG)) {
							desioSeTrig = false;	// ispod threshold-hyst
						}
					}

					// THRESHOLD < 0: alarmiram prekoracenje ISPOD
					if (AD_THRESHOLD__DEBUG < 0) {
						if (AD_REZULT[i] < (AD_THRESHOLD__DEBUG)) {
							desioSeTrig = true;		// preko thresholda
						} else if (AD_REZULT[i] >= (AD_THRESHOLD__DEBUG + AD_HYSTERSIS__DEBUG)) {
							desioSeTrig = false;	// ispod threshold-hyst
						}
					}
				}


				if (desioSeTrig) {
					// signaliziraj trigger
					osEventFlagsSet(EvtMeasurementDoneHandle, flg_ANALOG_TRIGGERED);
				} else {
					// nije bio trigger
					osEventFlagsClear(EvtMeasurementDoneHandle, flg_ANALOG_TRIGGERED);
				}
				// konacno signaliziraj zavrseno merenje
				osEventFlagsSet(EvtMeasurementDoneHandle, flg_ANALOG_PROCESSING_DONE);
				break;

			default:
				// TODO sta ovde? nista?
				break;
		}
		osDelay(1);
		osThreadYield();
	}
  /* USER CODE END StartTaskAnalogIn */
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
