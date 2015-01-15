#include "stm32f30x.h"

uint8_t risultato = 0;
uint8_t valori[6];
float risultati[3];

uint8_t SPI_SendData8(uint8_t);

int main(void)
{
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOEEN;
  RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
  GPIOE->MODER |= 0x00000040; //PE3 in general purpose
  GPIOA->MODER |= 0x0000A800; //PA5 PA6 PA7 in alternate function.
  GPIOA->OSPEEDR |= 0x00005400; //SPEED FOR GPIOA to 10 MHz - medium speed.
  GPIOA->AFR[0] |= 0x55500000; //We select alternate function 5 which is the SPI (Check the datasheet).
  GPIOE->BSRR |= (1<<3); //Brining PE3 to 1 to select the CS of the SPI. We want to bring it to 1 so that we can go though the configuration phase.
  
  //Configuration phase.
  
  SPI1->I2SCFGR &= 0xF7FF;
  SPI1->CR1 |= SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_MSTR; //Selecting software driven and master mode.
  SPI1->CR1 |= SPI_CR1_BR_1; //Bode rate equal to 72MHz / 8 . We do this because the gyroscope can't go faster than 10MHz.
  SPI1->CR2 |= SPI_CR2_FRXTH; //RXNE event is generated ifthe FIFO level is greater than or equal to 1/4 (8-bit)
  SPI1->CR2 |= SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0; //Configuration to 8-bit  0111.
  SPI1->CR1 |= SPI_CR1_SPE;
  
  GPIOE->BSRR |= (1 << 19);
  
  //Write on registry CTRL_REG1
  SPI_SendData8(0x20);
  //Activate normal mode, Xen,Yen,Zen
  SPI_SendData8(0x0F); 
  
  GPIOE->BSRR |= 1<<3;
  GPIOE->BSRR |= 1<<19;

  SPI_SendData8(0x23); //Write on CTRL_REG2
  SPI_SendData8(0x30); //2000 DPS
  
    while(1){
    
  GPIOE->BSRR |= 1<<19;                                                                 //abbasso il chipselect
  for(int i=0;i<500;i++);                                                               //attendo qualcosa
  SPI_SendData8(0xE8);                                                                      //scelgo il registro che contiene l'asse x e gli dico di autoincrementare 
  for(int i=0;i<6;i++) valori[i] = SPI_SendData8(0x00);                                     //ricevo i dati
  GPIOE->BSRR |= 1<<3;                                                                  //alzo il chip select
  for(int i=0;i<6;i=i+2) {
    risultati[i/2]= ((float)((int16_t)((uint16_t)(valori[i+1]*256) + valori[i])))*0.07;            //suppongo sia il calcolo in gradi della velocità
    }
  }

  
  return 1;
}

uint8_t SPI_SendData8(uint8_t data)
{
  uint32_t spixbase = 0x00;
  spixbase = (uint32_t)SPI1;
  spixbase += 0x0C;
  while((SPI1->SR & SPI_SR_TXE) != SPI_SR_TXE){}
  *(__IO uint8_t *)spixbase = data;
  while((SPI1->SR & SPI_SR_RXNE) != SPI_SR_RXNE){}
  return *(__IO uint8_t *)spixbase;
}
