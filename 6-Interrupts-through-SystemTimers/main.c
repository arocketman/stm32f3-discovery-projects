#include <std32f30x.h>

int main(void)
{
  //Generate interruptions.
  if(SysTick_Config(SystemCoreClock/5)) { while(1);}
  //Turn on GPIOE
  RCC ->AHBENR |= RCC_AHBENR_GPIOEEN;
  //Output mode
  GPIOE ->MODER |= 0x55550000;
  while(1);
  return 0;
}
