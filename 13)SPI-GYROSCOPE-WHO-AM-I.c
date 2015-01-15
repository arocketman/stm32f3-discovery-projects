#include <stm32f30x.h>

uint8_t SPI_SendData8(uint8_t dati);
uint8_t ricevuto;

int main(void)
{ 
  //We can see from STM32 Discovery page 13 that SPI1 is connected to APB2 which has 72MHz clock. Since the Gyroscope has to go to a MAX. 10MHz We will choose the configuration of BAUD RATE 010 which is CLOCK/8 = 9MHz
  RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; //Enabling clock for SPI peripheral
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOEEN; //Enabling PA PE
  GPIOE->MODER |= 0x00000040; //Gen purpose output mode PE3 CS_I2C/SPI (Check discovery pdf)
  GPIOA->MODER |= 0x0000A800; //Setting PA5-PA6-PA7 on alternate function (Check pin description on datasheet)
  GPIOA->AFR[0] |= 0x55500000; // We select AFR[0] port GPIOA. Then we put the bits 31-20 to conf AF5 which is 0101 .
  GPIOA->OSPEEDR |= 0x00005400; //10MHz for port A
  GPIOE->BSRR |= 1 << 3; //Do not communicate with gyroscope, we must set the bit to 1 because we wish to NOT communicate during configuration phase
  
  /* Configuration phase */
  
  SPI1->CR1 |= (1 << 4); //SPI_CR1_BR_1 010 , The gyroscope goes at 10MHz. I can't have a Bode rate > 10MHz. So I select divided by 72/8 = 9MHz < 10MHz.
  SPI1->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_MSTR; // Software driven and master mode.
  SPI1->CR2 |= SPI_CR2_FRXTH; //Set the FRXTH bit. The RXFIFO threshold must be aligned to the read access size for the SPIx_DR register
  SPI1->CR2 |= SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0; //0111: 8-bit for DS
  
  SPI1->CR1 |= SPI_CR1_SPE; //Enabling
  
  GPIOE->BSRR |= 1<<19; //Reset the bit, communicate with the gyroscope
  
  SPI_SendData8(0x8F);
  ricevuto = SPI_SendData8(0x00); //Reading the data. We have to get the WHO AM I register value (Read only).
  return 1;
  
  
}

uint8_t SPI_SendData8(uint8_t dati){
  uint32_t spixbase= 0x00;                                             
  spixbase =(uint32_t)SPI1;                                             
  spixbase += 0x0C;                                                     
  while((SPI1->SR & SPI_SR_TXE) != SPI_SR_TXE){}                                 // Waiting for transmission buffer to be empty
  *(__IO uint8_t *)spixbase = dati;
  while((SPI1->SR & SPI_SR_RXNE) != SPI_SR_RXNE){}                                 // Waiting for the buffer in reception to be empty.
  return *(__IO uint8_t *)spixbase;
}
