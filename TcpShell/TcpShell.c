/**
  ******************************************************************************
  * @file    FreeRTOS/FreeRTOS_ThreadCreation/Src/main.c
  * @author  MCD Application Team
  * @version V1.2.2
  * @date    25-May-2015
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stm32f7xx_hal.h>
#include <../CMSIS_RTOS/cmsis_os.h>
#include <stdbool.h>
#include "tcpshell.h"

extern ETH_HandleTypeDef EthHandle;

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	/* STM32F4xx HAL library initialization:
	     - Configure the Flash prefetch, instruction and Data caches
	     - Configure the Systick to generate an interrupt each 1 msec
	     - Set NVIC Group Priority to 4
	     - Global MSP (MCU Support Package) initialization
	*/
	dprintf("TcpShell: Init code. Port=%u, maxConns=%u\n", SERVER_PORT, MAX_CONNECTIONS);
	HAL_Init();
	LedInit();
	TcpInit(SERVER_PORT, MAX_CONNECTIONS);
  
	/* Start scheduler */
	dprintf("TcpShell: about to call osKernelStart()\n");	
	LedThinkingOff();
	osKernelStart();
	
	/* We should never get here as control is now taken by the scheduler */
	dprintf("TcpShell: Broke out of osKernelStart()\n");
	LedError(ErrorCodeBrokeOutOfOsKernelStart);
	for (;;) ;
}

void SysTick_Handler(void)
{
	HAL_IncTick();
	osSystickHandler();
}

void ETH_IRQHandler(void)
{
	HAL_ETH_IRQHandler(&EthHandle);
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
	dprintf("Assert failed: file %s on line %lu\r\n", file, line);
	LedError(ErrorApplicationAssertFailure);
	asm("bkpt 255");
	for (;;) ;
}

#endif

#if(  configCHECK_FOR_STACK_OVERFLOW > 0 )

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
	dprintf("Stack overflow in task %s\r\n", pcTaskName);
	LedError(ErrorApplicationStackOverflow);
	asm("bkpt 255");
	for (;;) ;
}

#endif

void vApplicationMallocFailedHook(void)
{
	dprintf("malloc failed\r\n");
	LedError(ErrorApplicationOutOfMemory);
	asm("bkpt 255");
	for (;;) ;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
