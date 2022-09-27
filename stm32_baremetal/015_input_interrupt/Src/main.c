#include <stm32f407xx.h>
#include <stdio.h>
#include <stdint.h>
#include "uart.h"
#include "systick.h"
#include "tim.h"
#include "exti.h"


int main(void) {
	uart1_rx_tx_init();
	printf("%s\n",__FUNCTION__);
	pa0_exti_init();
	pe4_exti_init();
	pe3_exti_init();
	GPIOA->MODER |= GPIO_MODER_MODER6_0;
	GPIOA->MODER |= GPIO_MODER_MODER7_0;


	while (1);
}


void EXTI0_IRQHandler(void)
{
	printf("%s\n",__FUNCTION__);
	if (!(EXTI->PR & EXTI_PR_PR0)){
		return;
	}

	EXTI->PR |= EXTI_PR_PR0;
	printf("WK_UP: Button Pressed\n");
}

void EXTI3_IRQHandler(void)
{
	printf("%s\n",__FUNCTION__);
	if (!(EXTI->PR & EXTI_PR_PR3)){
		return;
	}

	EXTI->PR |= EXTI_PR_PR3;
	printf("K1: Button Pressed\n");
}

void EXTI4_IRQHandler() {
	printf("%s\n",__FUNCTION__);
	if (!(EXTI->PR & EXTI_PR_PR4)) { //Asegurarnos de que la flag esta puesta
		return;
	}

	GPIOA->ODR ^= GPIO_ODR_OD7;
	EXTI->PR |= EXTI_PR_PR4; //Limpia flag pendiente
	printf("K0: Button Pressed\n");
}

/*
int main(void)
{
	uart1_rx_tx_init();
	printf("%s\n",__FUNCTION__);
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;  //LED
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN; //BOTON

    GPIOA->MODER |= GPIO_MODER_MODER7_0; //PIN7 AS OUTPUT
    GPIOE->PUPDR |= GPIO_PUPDR_PUPDR4_0; //PULLUP

    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; //Activamos SYSCFG System configuration controller clock
    SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI4_PE; //Configura la interupcion para el PIN 3 del PORTE



    EXTI->FTSR |= EXTI_FTSR_TR4;  // activar falling
    EXTI->IMR |= EXTI_IMR_MR4;  // habilitar interrupcion (unmask)
    EXTI->PR |= EXTI_PR_PR4; // Limpiamos el registro de pendiente

    //NVIC->ISER[0] |= 1 << EXTI3_IRQn; //Activamos interrupcion en NVIC, IRQ 9 por lo que esta en [0]
	NVIC_EnableIRQ(EXTI4_IRQn);
    while (1);
}
*/

