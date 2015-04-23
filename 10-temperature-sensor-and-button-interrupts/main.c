/*
* Program that reads the temperature each time the user-button is pressed.
*/

#include <stm32f30x.h>

void configureInterruptsForUserButton(void);
void setupADC1(void);

int main(void)
{
    configureInterruptsForUserButton();
    setupADC1();
    while(1);   
    return 1;
}

/*
*Enables the stm32 to work with interrupts on the user button which is connected through EXTI0 at PA0
*For explanation on the commands, look up the previous code on this same repository.
*/
void configureInterruptsForUserButton(void)
{
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PA;
    NVIC->ISER[0] |= (1 << 6);
    EXTI->IMR |= EXTI_IMR_MR0;
    EXTI->EMR |= EXTI_EMR_MR0;
    EXTI->FTSR |= EXTI_FTSR_TR0;
}

void setupADC1()
{
  
  RCC->AHBENR |= RCC_AHBENR_ADC12EN;
  ADC1_2->CCR |= (1<<16);
  ADC1->CR &= 0xCFFFFFFF; //ADVREGEN TO 00
  ADC1->CR |= 0x10000000; //ADVREGEN TO 01
  for(int i = 0; i < 1000; i++);
  ADC1->CR &= 0xFFFFFFFE; //Ensuring that ADEN = 0
  ADC1->CR &= 0xBFFFFFFF; //Single-ended-input
  ADC1->CR |= ADC_CR_ADCAL;
  while((ADC1->CR & ADC_CR_ADCAL) == ADC_CR_ADCAL); //Waiting for the calibration to finish
  
}
