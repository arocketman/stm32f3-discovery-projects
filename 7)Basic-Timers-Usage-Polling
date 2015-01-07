/*
* Using basic timers (TIM6) with polling
*/

#include <stm32f30x.h>

int main(void)
{
  RCC->AHBENR |= RCC_AHBENR_GPIOEEN; //Enabling port E.
  RCC->APB1ENR |= RCC_APB1ENR_TIM6EN; //Enabling timer 6.
  
  TIM6->CR1 = 0; //Counter initially off.
  TIM6->CR2 = 0;
  TIM6->DIER = 0; //Update interrupt Disabled 
  TIM6->PSC = 7199; //Value of the prescaler to 7200 (71999 + 1)
  TIM6->ARR = 10000; //10000 counts before update event is generated.
  TIM6->CR1 |= 1; //Enabling counter.
  
  GPIOE->MODER |= 0x55550000; //Output mode for GPIOE.
  int counter = 0;
  while(1){
    if((TIM6->SR & TIM_SR_UIF)==TIM_SR_UIF)
    {
      TIM6->SR = 0; //Reset the update event.
      GPIOE->ODR |= (counter++)<<8;
    }
  }
  return 0; //Unreachable point. That's why the polling is not worth it, better to work with interrupts.
}
