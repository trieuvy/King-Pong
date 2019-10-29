// UART.c
// Runs on TM4C123 or LM4F120
// Lab 11 involves switching this from UART1 to UART0.
//                 switching from PC5,PC4 to PA1,PA0
// Daniel Valvano
// December 29, 2014

/* This example accompanies the book
   "Embedded Systems: Introduction to ARM Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2014

 Copyright 2015 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

// this connection occurs in the USB debugging cable
// U0Rx (PA0) connected to serial port on PC
// U0Tx (PA1) connected to serial port on PC
// Ground connected ground in the USB cable

#include "tm4c123gh6pm.h"
#include "UART.h"

//------------UART_Init------------
// Initialize the UART for 57600 baud rate (assuming 80 MHz UART clock),
// 8 bit word length, no parity bits, one stop bit, FIFOs enabled
// Input: none
// Output: none

void UART_Init(void){
// as part of Lab 11, modify this program to use UART0 instead of UART1
//                 switching from PC5,PC4 to PA1,PA0
  SYSCTL_RCGC1_R |= SYSCTL_RCGC1_UART1; // activate UART1
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB; // activate port B
  UART1_CTL_R &= ~UART_CTL_UARTEN;      // disable UART
  UART1_IBRD_R = 86;                  	// IBRD = int(80,000,000 / (16 * 57600)) = int(86.80555556)
  UART1_FBRD_R = 51;                  	// FBRD = round(0.80555556 * 64 + 0.05) = 52
	//UART1_IBRD_R = 130;                   // IBRD = int(80,000,000 / (16 * 9600)) = int(520.8333333)
  //UART1_FBRD_R = 13;                    // FBRD = round(0.8333333 * 64 + 0.05) = 53
																				// 8 bit word length (one stop bit)
  UART1_LCRH_R = (UART_LCRH_WLEN_8);		
  UART1_CTL_R |= UART_CTL_UARTEN;       // enable UART
  GPIO_PORTB_AFSEL_R |= 0x03;           // enable alt funct on PB1,PB0
  GPIO_PORTB_DEN_R |= 0x03;             // enable digital I/O on PB1,PB0
  GPIO_PORTB_PCTL_R &= ~0x000000FF; 		// configure PB0 and PB1 as U1Rx and U1Tx
	GPIO_PORTB_PCTL_R |= 0x00000011;
  GPIO_PORTB_AMSEL_R &= ~0x03;          // disable analog functionality on PB1,PB0
}

/*
unsigned char [] UART_InString(void){
	unsigned char [] data;
	unsigned int i = 0;
	while(str[i] !=0x0){
		data += UART_InChar();
		i++;
	}
	return data;
}


unsigned int UART_InInt(void){
	while((UART1_FR_R&UART_FR_RXFE) != 0);
	return((unsigned int)(UART1_DR_R&0xFF));
}
		*/

//------------UART_InChar------------
// Wait for new serial port input
// Input: none
// Output: ASCII code for key typed
unsigned char UART_InChar(void){
  while((UART1_FR_R&UART_FR_RXFE) != 0);
  return((unsigned char)(UART1_DR_R&0xFF));
}


//------------UART_OutChar------------
// Output 8-bit to serial port
// Input: letter is an 8-bit ASCII character to be transferred
// Output: none
void UART_OutChar(unsigned char data){
// as part of Lab 11, modify this program to use UART0 instead of UART1
  while((UART1_FR_R&UART_FR_TXFF) != 0);
  UART1_DR_R = data;
}

//------------UART_OutString------------
// Output String (NULL termination)
// Input: pointer to a NULL-terminated string to be transferred
// Output: none
void UART_OutString(unsigned char str[]){
	unsigned int i = 0;
	while(str[i] != 0x0){
		UART_OutChar(str[i]);
		i++;
	}
}
