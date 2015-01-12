#include <stm32f30x.h>

/*
* Use of the DAC - Digital to Analog Converter.
* Takes a sample at each TIM6 overflow event, resets at 100 samples which will be stored in an array.
* We are taking a sample of a sinusoid, its period is 2pi. We want 100 samples so we have 2pi/100 = 0.0628 ~ 0.063
* Everytime the button user is pressed, we are incrementing of 10^cont , cont can't be > 2.
*/

void setupTIM6(void);
void setupDAC(void);
void setupButton(void);

int cont = 0;
int a = 0;
int prescaler = 0;
int campioni[100];
int risultati[100];


int main(void)
{
  setupButton();
  setupDAC();
  setupTIM6();
  TIM6->CR1 |= TIM_CR1_CEN;
  while(1);
  return 1;

}
  
void setupTIM6(void)
{
  RCC->APB1ENR |= RCC_APB1ENR_TIM6EN; //Timer 6 clock enable 
  NVIC->ISER[1] |= (1 << 22); //Enabling interrupts TIM6 global and DAC12 underrun interrupts
 
  TIM6->CR1 = 0; //Resetting CR1
  TIM6->CR2 = 0; //Resetting CR2
  TIM6->PSC = 3599; //Prescaler to 3600 (3599+1)
  TIM6->CNT = 10000; 
  TIM6->DIER |= TIM_DIER_UIE; //Update interrupt enable
}

void setupButton(void)
{
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN; //Enabling GPIOA for button
  NVIC->ISER[0] |= (1 << 6); //Enabling EXTI0
  SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PA; //PA0 pin
  EXTI->IMR |= EXTI_IMR_MR0;
  EXTI->EMR |= EXTI_EMR_MR0;
  EXTI->FTSR |= EXTI_FTSR_TR0;
}

void setupDAC()
{
  RCC->APB1ENR |= RCC_APB1ENR_DACEN; //Turning on the clock of the DAC activating the peripheral
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN; //Enabling port A
  GPIOA->MODER |= GPIO_MODER_MODER4 | GPIO_MODER_MODER5; //PA4 and PA5 to analog mode
  DAC->CR |= DAC_CR_EN1; //Enabling the DAC channel 1.
}
