/* 
Binary counter with interrupts using button
*/

#include <stm32f30x.h>

int buttonPressed(void);

int main(void)
{
  RCC->AHBENR |= RCC_AHBENR_GPIOEEN; //Enabling GPIOE.
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN; //Enabling GPIOA
  GPIOE->MODER |= 0x55550000; //Output mode.
  GPIOE->ODR |= 0x00000000; //Turning all the LEDs off.
  /* Setting up the button interruption */
  SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PA; //EXTI 0 configuration
  NVIC->ISER[0] |= (1<<EXTI0_IRQn); //Turning on EXTI0 because it's connected to PA0
  EXTI->IMR |= EXTI_IMR_MR0; //Interrupt Mask on line 0
  EXTI->FTSR |= EXTI_FTSR_TR0; //Falling trigger event configuration bit of line 0 (When the button is released)
  EXTI->EMR |= EXTI_EMR_MR0; //Event Mask on line 0
  while(GPIOE->ODR != 0x0000FF00);
  
  return 0;
}
