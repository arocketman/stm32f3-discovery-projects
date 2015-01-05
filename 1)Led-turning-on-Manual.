#include <stm32f30x.h>

void waitMe();
void setAndWait(uint32_t);

int main(void)
{
  //I want to set the bit 21. The LEDs are activated through GPIOE which is enabled through bit 21. Pag. 116 ref. Manual
  RCC->AHBENR |= 0x00200000;
  //We want to set the GPIOE peripheral to output mode, which is 10.
  //The LEDs are on port 8-15
  //PORTS 8-15 -> BIT 16-31
  GPIOE->MODER |= 0x55550000;
  int j;
  for(j = 0; j < 10 ; j++){
    setAndWait(0x0000FFFF); //Ports from 8-15 to 1 (With ODR Bits = ports)
    setAndWait(0x00000000); //all off
    setAndWait(0x00008000); //Port 15
    setAndWait(0x00004000); //Port 14
    setAndWait(0x00002000); //Port 13
    setAndWait(0x00001000); //Port 12
    setAndWait(0x00000800); //Port 11
    setAndWait(0x00000400); //Port 10 
    setAndWait(0x00000200); //Port 9 
    setAndWait(0x00000100); //Port 8
  }
  //Turn them all off.
  setAndWait(0x00000000);
  return 0;
}

/**
*Typical 'sleep' function.
*/
void waitMe(){
  int i = 0;
  for(i = 0; i < 500000; i++){}
}

/**
*Sets the GPIOE ODR register to a certain value. 
*@param regValue hex Value for the registry.
*/
void setAndWait(uint32_t regValue){
  GPIOE->ODR = regValue;
  waitMe();
}
