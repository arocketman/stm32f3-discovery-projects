#include <stm32f30x.h>

/*
* Generate a tension of COUNTER volts each second and turn on COUNTER leds based on the value of the tension.
*/

void setupDAC(void);
void setupLED(void);
void generateTension(int);
void turnNLeds(const int ledNumbers);
void incrementCounterAndWait(void);
void delay();

int counter = 0;

int main(void)
{
  setupLED();
  setupDAC();
  while(1)
  {
    generateTension(counter);
    turnNLeds(counter);
    incrementCounterAndWait();
  }
  return 1;
}
 
/*
  //Generates a tension between 0 and 3.
*/

void generateTension(int tension)
{
  if(tension < 0)
    tension = tension * (-1);
  if(tension > 3)
    tension = tension % 3;
  DAC->DHR12R1 = (int)(tension * 4095 / 3.3); //We are using the formula at chapter 13.3.5 of reference manual DACOUT = VDDA  * DOR / 4095
}

void setupLED(void)
{
  RCC->AHBENR |= RCC_AHBENR_GPIOEEN;
  GPIOE->MODER |= 0x55550000;
}

void setupDAC(void)
{
  RCC->APB1ENR |= RCC_APB1ENR_DACEN;
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
  GPIOA->MODER |= 0x000000F00;
}

/*
*Turns on ledNumbers (N) LEDs
*@param ledNumber LED numbers to be turned on.
*/
void turnNLeds(const int ledNumbers)
{
  if(ledNumbers > 8 || ledNumbers <= 0) return;
  int i;
  for(i = 8; i < ledNumbers+8; i++)
    GPIOE->ODR |= (1 << i);
}

void incrementCounterAndWait(void)
{
  counter++;
  if(counter > 3){
    delay();
    //When the counter is greater than 3, we reset it to 0 and turn off all the LEDs.
    counter = 0;
    GPIOE->ODR &= 0x000000FF; //Resetting LEDs.
  }
  delay();
}

void delay()
{
  for(int i = 0; i < 7200000; i++); //Waiting.
}
