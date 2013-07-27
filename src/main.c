/**
 * STM32F4 Template file by: Tom McLeod
 *
 * Borrowed heavily from the IO example provided by ST.
 *
 * This code will blink the 4 LED's on the STM32F4Discovery board repeatedly.
 *
 * If you end up using this code please let me know!
 *
 * That said, this isn't licensed, so feel free to do what you want :P.
 *
 * THIS CODE IS PROVIDED AS IS, WITH NO WARRANTY.
 **/ 

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include "misc.h"

GPIO_InitTypeDef GPIO_InitStructure;
EXTI_InitTypeDef EXTI_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;

void delay(float time);
void EXTI0_IRQHandler(void);

int presses = 0;

int main(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);		// Start clock on GPIOD
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);		// Same for GPIOA
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);		// Same for the SYSCFG

	// This configures the Discovery LED pins D(12-15)
	// 12 = green/left
	// 13 = orange/top
	// 14 = red/right
	// 15 = blue/bottom
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	// Enable FPU :D
	// Not really used in this case, but doesn't hurt and proves that it works to 
	// some degree.
	SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	// Connect EXTI0 to GPIOA
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);

	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	// Set up the NVIC to have a low priority
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

	while (1)
	{
		if (presses == 1) {
			GPIO_SetBits(GPIOD,GPIO_Pin_12);	// Turn on green
		}

		if (presses == 2) {
			GPIO_SetBits(GPIOD,GPIO_Pin_13);	// Turn on orange
		}

		if (presses == 3) {
			GPIO_SetBits(GPIOD,GPIO_Pin_14);	// Turn on red
		}

		if (presses == 4) {
			GPIO_SetBits(GPIOD,GPIO_Pin_15);	// Turn on blue
		}

		if (presses == 0) {
			// Turn all off
			GPIO_ResetBits(GPIOD,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
		}
	}
}

void delay(float time) 
{
	while(time--)
	{
	}
}

void EXTI0_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
		if (presses < 5)
		{
			presses++;
		} else
		{
			presses = 0;
		}
    /* Clear the EXTI line 0 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line0);
  }
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{ 
	while (1)
	{
	}
}
#endif

