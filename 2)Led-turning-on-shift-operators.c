/*
* Achieves the same result of the (1) Program (check this same repository) , but using shifts operators.
*/

#include <stm32f30x.h>

void zzz();

int main(void)
{
  RCC->AHBENR |= 0x00200000;
  GPIOE->MODER |= 0x55550000; 
  //Ripete il giro dei led 30 volte
  int h;
  for(h = 0 ; h < 30; h++){
    int j;
    //Initial value set to 0000 0000 0000 0000 1000 0000 0000 0000 I turn on the port 15 <-> bit 15, which is LD6.
    GPIOE->ODR = 0x00008000;
    zzz();
    for(j = 0; j < 7; j++){
      //I use the right shift ... starting from the 15 bit we have: 1000 0000 0000 0000 .. shifting I'll have 0100 0000 0000 000 .. ecc. I'll eventually turn on all the LEDs one by one. 
      GPIOE->ODR = (GPIOE->ODR) >> 1;
      //sleep
      zzz();
    }
  }
  return 0;
}

void zzz(){
int i;
for(i = 0; i < 500000; i++){}
}


