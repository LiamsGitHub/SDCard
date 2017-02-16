// This code provides "signs of life": routes internal high speed clock to MCO pin 29, toggles GPIO PA9 on pin 30 and PA10 on pin 31
// PA9 and PA10  drive a small LED via 680 ohm resistors
// PA10 pulse is much shorter than PA9 to distinguish this project from other versions of SignOfLife
// Requires startup assembly file StartUp_simple.s, register header file STM32F100.h, link script STM32F100C8_simple.ld and a Makefile
// More complex  boot environment suitable for extension later on
// Liam Goudge Feb 2015
// Updated Aug 2016
// STM103 version Jan 2017

#include "../Foundation/STM32F103.h"
#include "../Foundation/myprintf.h"

void flash(void);

void flash (void)
  {
		int i=0;

	  GPIOC_BSRR=0x2000; // Set PC13
	  for (i=0;i<0x80000;i++);
	  GPIOC_BSRR=0x20000000; // Clear PC13
	  for (i=0;i<0x10000;i++);
      }


int main(void)

// This worked finally. Next add in OR language to set bits and while loop to wait for transmit to be done. Then add in Rx capability
// Finally transmit A-Z to Rapi


{

// Set up power to USART1 and allocate pins (Tx=pin 30 (PA9) Rx=pin 31 (PA10) )
// USART1 is on APB2
// Also enable GPIO port C for LED

  RCC_APB2ENR= 0x4015; // Start clock to USART1, GPIO port C and AFIO

// Set up GPIO for PC13 to drive LED
  GPIOC_CRH = 0x200000; 	// PC13 output push pull
  	  	  	  	  	  	  	// GPIO output 10Mhz

// Tx on PA9. Set CNF to mode ALT function, OUT, PUSH-PULL = 0b10. Set MODE to output 10MHz = 0b01
// Rx on PA10. Set CNF to mode input floating 0b01 and MODE to input 0x00
// Both on CRH

  GPIOA_CRH = 0x8b0;

  //Set the Clock config register to output High Speed Internal 8MHz clock at MCO pin 29


  // Now set up UART1. (Switch on UART1 and enable transmit / receive) (Tx is 0x2008, Rx is 0x2004)
  USART1_CR1 = 0x2000; // Just set UE

  // Baud rate 9600. With Fclk set to HSI = 8MHz, MANTISSA=52 and FRACTIONAL=1 for BR of 9604
  USART1_BRR = 0x341;
  USART1_CR1 = 0x200C; // Set TE and RE bits as well. Bit 12 set to 0 for 1 start bit, 8 data and n stop bits (stop bits in CR2)
  // USART_CR2 bits 12:13 reset values are 00 = 1 stop bit


  //char transmit = 'A';

  while (1) {
/*
  USART1_DR = transmit++;
  if (transmit>'Z') {
	  transmit = 'A';
  }

    while (!(USART1_SR & 0x20));
    myprintf("Data: ",USART1_DR & 0xFF);
*/
  flash();
  }
}




