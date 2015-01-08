/* Build a program that will start with all the leds turned on. 
   Every time the user button is pushed, a led will turn off.
   Once all the leds are turned off, a timer will start (use basic timers TIM6) 
   and will bring the led back to the original position (1 led turned on each second)
*/

#include <stm32f30x.h>

int buttonPressed(void);

int main(void)
{
  RCC->AHBENR |= RCC_AHBENR_GPIOEEN; //Enabling GPIOE.
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN; //Enabling GPIOA
  GPIOE->MODER |= 0x55550000; //Output mode.
  GPIOE->ODR |= 0x0000FF00; //Turning all the LEDs on.
  while(GPIOE->ODR != 0x00000000)
  {
    if(buttonPressed())
    {
      GPIOE->ODR = (GPIOE->ODR)<<1;
    }
  }
  /*Now we want to restore all the LEDs on through timers */
  RCC->APB1ENR |= RCC_APB1ENR_TIM6EN; //Enabling timer6.
  TIM6->CR1 = 0;
  TIM6->CR2 = 0;
  TIM6->PSC = 7199; //Setting the prescaler to 7199+1
  TIM6->DIER = 0; //Disabling interrupts for this piece of program.
  TIM6->ARR = 10000; //Update each second 
  TIM6->CR1 |= TIM_CR1_CEN;
  int counter = 8;
  while(GPIOE->ODR != 0x0000FF00)
  {
    if( ( TIM6->SR & 0x1 ) == 0x1)
    {
      TIM6->SR = 0; //Resetting the update event flag.
      GPIOE->ODR |= (1 << counter++);  //Explanation : We want to put to 1 all the bits from 8 to 15. So we shift 1 to the left COUNTER times. Counter is then increment at each turn.
    }
  }
  
  return 0;
}

/*
*@return 1 (true) if the button was pressed.
*/
int buttonPressed(void)
{
  if(GPIOA->IDR&0x1 == 0x1)
  {
      while(GPIOA->IDR&0x1 == 0x1){}
      return 1;
  }
  return 0;
}
