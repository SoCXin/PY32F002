/**
  ******************************************************************************
  * @file    hsi.c
  * @author  MCU Application Team
  * @brief   This file provides all the HSI measurement and calibration firmware functions.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) Puya Semiconductor Co.
  * All rights reserved.</center></h2>
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "hsi.h"
#include <math.h>


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

  /* If reference frequency is not the LSE and lower than 250 Hz (The TIM14 min frequency),
  the TIMx counter prescaler should be used.
  Otherwise (the reference frequency is greater than 250 Hz),
  the TIMx counter prescaler should be set to 0

  For example, if reference frequency is 50 Hz, the counter prescaler should be 8.
  In that case the minimum frequency (if HSI is system clock source) is 16MHz/(8*65535) ~ 31 Hz

  If the reference frequency is lower than 31Hz, the counter prescaler should be
  used regarding the reference frequency in input */

  /* The reference frequency is 100 Hz (the signal in input capture is not divided) */
  /* #define REFERENCE_FREQUENCY         ((uint32_t)100) */ /* The reference frequency value in Hz */
  #define REFERENCE_FREQUENCY            ((uint32_t)1000)   /* The reference frequency value in Hz */
  /* #define HSI_TIMx_COUNTER_PRESCALER  ((uint32_t)7) */   /* TIMx counter is divided by 7 + 1 = 8 */
  #define HSI_TIMx_COUNTER_PRESCALER     ((uint32_t)0)      /* TIMx counter is divided by 0 + 1 = 1 */
  /* The signal in input capture is not divided */
  #define HSI_TIMx_IC_DIVIDER           TIM_ICPSC_DIV1

  /* Connect GPIO to TIMx Input Capture 1 */
  #define HSI_Timer_ConnectInput()      HAL_TIMEx_RemapConfig(&TimHandle, TIM_TIMx_GPIO)

  /* Number of measurements in the loop */
  /* #define HSI_NUMBER_OF_LOOPS            ((uint32_t)10) */  /* 1200MS */
  /* #define HSI_NUMBER_OF_LOOPS            ((uint32_t)5) */   /* 650ms */
  #define HSI_NUMBER_OF_LOOPS               ((uint32_t)3)      /* 430ms */


  /* HSI Rough VALUE */
  uint32_t HSI_Rough_Value ;
  
  /* HSI Fine VALUE */
  uint32_t HSI_Fine_Value ;
  

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void      HSI_TIMx_ConfigForCalibration(void);
void      HSI_RCC_AdjustCalibrationValue(uint32_t TrimmingValue);
uint32_t  HSI_FreqMeasure(void);
void HSI_MeasurementInit(uint32_t HSICLKSource_set);
uint32_t Hsi_Rough_Trimming(void);
uint32_t Hsi_Fine_Trimming(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief   ??????????????????4???,??????4????????????????????????
  * @param   ???
  * @retval  ???4????????????
  */
uint32_t Hsi_Rough_Trimming(void)
{
  uint32_t i;
  uint32_t trim_Dac;                              /* ?????????????????????4bit */
  uint32_t dac_Index;
  uint32_t dac_Array[15];	
  uint32_t first_Index=7;
  uint32_t binary_Cyc=4;                          /* ?????????????????? */
  uint32_t sysclockfrequency = 0;
  uint32_t measuredfrequency = 0;
  /* uint32_t trim_Final_Value  =0; */
  uint32_t Rough_trim_Final_freq  =0;
  uint32_t Rough_trim_Final_Dac = 0;              /* ???4??????????????? */

  /* ??????????????? */
  for(i=0;i<15;i++)  /* ??????0??? */
  {
     dac_Array[i]=i+1;
  } 
  
  dac_Index=first_Index;
  trim_Dac=dac_Array[dac_Index];

  sysclockfrequency = HAL_RCC_GetSysClockFreq();

/* ??????????????? */
  do
  {
      if (StartCalibration != 0)
      {
        /* Set the Intern Osc trimming bits to trimmingvalue */
        HSI_RCC_AdjustCalibrationValue((trim_Dac<<9) + 255);
      }
      
      /* Get actual frequency value */
      measuredfrequency = HSI_FreqMeasure(); 
      
      if(ABS_RETURN((int32_t)(measuredfrequency-sysclockfrequency))<ABS_RETURN((int32_t)(Rough_trim_Final_freq-sysclockfrequency))) /* ????????????DAC */
      {
        Rough_trim_Final_freq = measuredfrequency ;
        Rough_trim_Final_Dac  = trim_Dac ;
      }                              
      if(measuredfrequency <sysclockfrequency)            /* ????????????DAC??????????????????????????????????????????????????????DAC */
      {
         dac_Index  = dac_Index +(uint32_t)pow(2,binary_Cyc-2);
         trim_Dac  = dac_Array[dac_Index ];
      }
      else                 
      {
        dac_Index  = dac_Index -(uint32_t)pow(2,binary_Cyc-2);
        trim_Dac  = dac_Array[dac_Index];
      }
    binary_Cyc-=1;
  }while(binary_Cyc>0); 
  return Rough_trim_Final_Dac;
}

/**
  * @brief   ??????????????????9????????????9????????????????????????
  * @param   ???
  * @retval  ???9????????????
  */
uint32_t Hsi_Fine_Trimming(void)
{
  uint32_t i;
  uint32_t trim_Dac;                 /* ?????????????????????9bit */
  uint32_t dac_Index;
  uint32_t dac_Array[511];
  uint32_t first_Index=255;
  uint32_t binary_Cyc=9;             /* ?????????????????? */
  uint32_t sysclockfrequency = 0;
  uint32_t measuredfrequency = 0;
  uint32_t Fine_trim_Final_freq  =0;
  uint32_t Fine_trim_Final_Dac = 0;

  /* ??????????????? */
  for(i=0;i<511;i++)
  {
    dac_Array[i]=i+1;
  }
  
  dac_Index=first_Index;
  trim_Dac=dac_Array[dac_Index];
  
  /* Set measurement environment */
  /* HSI_MeasurementInit(); */
  /* Get system clock frequency */
  sysclockfrequency = HAL_RCC_GetSysClockFreq();

/* ??????????????? */
  do
  {
      if (StartCalibration != 0)
      {
        /* Set the Intern Osc trimming bits to trimmingvalue */
        HSI_RCC_AdjustCalibrationValue((HSI_Rough_Value<<9) + trim_Dac);
      }
      
      /* Get actual frequency value */
      measuredfrequency = HSI_FreqMeasure(); 
      
      if(ABS_RETURN((int32_t)(measuredfrequency-sysclockfrequency))<ABS_RETURN((int32_t)(Fine_trim_Final_freq-sysclockfrequency))) /* ????????????DAC */
      {
        Fine_trim_Final_freq = measuredfrequency ;
        Fine_trim_Final_Dac  = trim_Dac ;
      }                              
      if(measuredfrequency <sysclockfrequency)                                 /* ????????????DAC??????????????????????????????????????????????????????DAC */
      {
         dac_Index  = dac_Index +(uint32_t)pow(2,binary_Cyc-2);
         trim_Dac  = dac_Array[dac_Index ];
      }
      else                 
      {
        dac_Index  = dac_Index -(uint32_t)pow(2,binary_Cyc-2);
        trim_Dac  = dac_Array[dac_Index];
      }
    binary_Cyc-=1;
  }while(binary_Cyc>0);
  return Fine_trim_Final_Dac;	
}

/**
  * @brief   ??????HSI????????????
  * @param   ???
  * @retval  HSI????????????
  */
uint32_t HSI_FreqMeasure( void )
{
  uint32_t measuredfrequency;
  uint32_t loopcounter = 0;
  uint32_t timeout = ((uint32_t)0xFFFFFF);    /* ?????????????????? */


  /* Start frequency measurement for current trimming value */
  measuredfrequency = 0;
  loopcounter = 0;

  /* Start measuring Internal Oscillator frequency */
  while (loopcounter <= HSI_NUMBER_OF_LOOPS)
  {
    CaptureState = CAPTURE_START;


    /* Enable capture 1 interrupt */
    HAL_TIM_IC_Start_IT(&TimHandle, TIM_CHANNEL_y);

    /* Enable the TIMx IRQ channel */
    HAL_NVIC_EnableIRQ(TIMx_IRQn);

    /* Wait for end of capture: two consecutive captures */
    while ((CaptureState != CAPTURE_COMPLETED) && (timeout != 0))
    {
      if(--timeout == 0)
      {
        return ERROR;
      }
    }

    /* Disable IRQ channel */
    HAL_NVIC_DisableIRQ(TIMx_IRQn);

    /* Disable TIMx */
    HAL_TIM_IC_Stop_IT(&TimHandle, TIM_CHANNEL_y);

    if (loopcounter != 0)
    {
      /* Compute the frequency (the Timer prescaler isn't included) */
      measuredfrequency += (uint32_t) (REFERENCE_FREQUENCY * Capture);
    }

    /* Increment loop counter */
    loopcounter++;
  }
  /* END of Measurement */

  /* Compute the average value corresponding the current trimming value */
  measuredfrequency = (uint32_t)((__HAL_GET_TIM_PRESCALER(&TimHandle) + 1) * (measuredfrequency / HSI_NUMBER_OF_LOOPS));
  return measuredfrequency;
}

/**
  * @brief   HSI?????????????????????
  * @param   HSICLKSource_selt
  * @retval  ???
  */
void HSI_MeasurementInit(uint32_t HSICLKSource_set)
{
  SystemClock_Config(HSICLKSource_set);                                 /* ?????????????????? */
    
  /* HSI_Rough_Value default value */	  
  /* ??????????????? */
  HSI_Rough_Value  =  ( READ_REG(RCC->ICSCR) & 0x00001fff ) >>9;        /* High 4 bits  ???4??? */
  HSI_Fine_Value   =  ( READ_REG(RCC->ICSCR) & 0x000001ff );            /* Low 9 bits   ???4??? */
  
  /* Configure the GPIO ports before starting calibration process */
  GPIO_ConfigForCalibration();
 
  /* Configure TIMx before starting calibration process */
  HSI_TIMx_ConfigForCalibration();
}


/**
  * @brief   Timer?????????????????????????????????????????????HSI
  * @param   ???
  * @retval  ???
  */
void HSI_TIMx_ConfigForCalibration(void)
{
  TIM_IC_InitTypeDef      ic_config; /* Timer Input Capture Configuration Structure declaration */

  /* Enable TIMx clock */
  __TIMx_CLK_ENABLE();

  /* Set TIMx instance */
  TimHandle.Instance = TIMx;

  /* Reset TIMx registers */
  HAL_TIM_IC_DeInit(&TimHandle);

  /* Connect input signal */
  HSI_Timer_ConnectInput();                         /* ????????????????????? */

  /* Initialize TIMx peripheral as follows:
       + Period = 0xFFFF
       + Prescaler = 0
       + ClockDivision = 0
       + Counter direction = Up
  */
  TimHandle.Init.Period            = 0xFFFF;
  TimHandle.Init.Prescaler         = HSI_TIMx_COUNTER_PRESCALER;
  TimHandle.Init.ClockDivision     = 0;
  TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  if (HAL_TIM_IC_Init(&TimHandle) != HAL_OK)
  {
    /* Initialization Error */
    APP_ErrorHandler();
  }

  /*##-2- Configure the Input Capture channel ################################*/
  /* Configure the Input Capture of channel 2 */
  ic_config.ICPolarity  = TIM_ICPOLARITY_RISING;
  ic_config.ICSelection = TIM_ICSELECTION_DIRECTTI;
  ic_config.ICPrescaler = HSI_TIMx_IC_DIVIDER;
  ic_config.ICFilter    = 0;
  if (HAL_TIM_IC_ConfigChannel(&TimHandle, &ic_config, TIM_CHANNEL_y) != HAL_OK)
  {
    /* Configuration Error */
    APP_ErrorHandler();
  }

  /*##-2- Configure the NVIC for TIMx */
  HAL_NVIC_SetPriority(TIMx_IRQn, 0, 1);

  /* Disable the TIMx global Interrupt */
  HAL_NVIC_DisableIRQ(TIMx_IRQn);
}

/**
  * @brief   ??????????????????????????????????????????????????????
  * @param   TrimmingValue?????????????????????????????????
  * @retval  ???
  */
void HSI_RCC_AdjustCalibrationValue(uint32_t TrimmingValue)
{
  MODIFY_REG(RCC->ICSCR, RCC_ICSCR_HSI_TRIM, (TrimmingValue) << RCC_ICSCR_HSI_TRIM_Pos);
}

/**
  * @}
  */

/************************ (C) COPYRIGHT Puya *****END OF FILE******************/
