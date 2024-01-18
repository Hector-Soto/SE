/*
 * The Clear BSD License
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 *  that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"

#include "pin_mux.h"
#include <stdbool.h>
bool derb=false, izqb=false;
int valor=0, izq, der;
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
void led_green_init()
{
  SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
  PORTD->PCR[5] = PORT_PCR_MUX(1); 
  GPIOD->PDDR |= (1 << 5); 
  GPIOD->PSOR = (1 << 5);  
}

void led_green_toggle()
{
  GPIOD->PTOR = (1 << 5);
}

void led_red_init()
{
  SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
  PORTE->PCR[29] = PORT_PCR_MUX(1);
  GPIOE->PDDR |= (1 << 29);
  GPIOE->PSOR = (1 << 29);
}

void led_red_toggle(void)
{
  GPIOE->PTOR = (1 << 29);
}

void sw1_init(){
  SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK; //0x800U, bit nº11
  PORTC->PCR[3] |= PORT_PCR_MUX(1) | PORT_PCR_PE(1) | PORT_PCR_PS(1) | PORT_PCR_IRQC(10);//IRQC 10 (1010) interrupcion falling
  GPIOC->PDDR &= ~(1 << 3); //Pin entrada debe estar a 0
}

void sw2_init(){
  SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
  PORTC->PCR[12] |= PORT_PCR_MUX(1) | PORT_PCR_PE(1) | PORT_PCR_PS(1) | PORT_PCR_IRQC(10);
  GPIOC->PDDR &= ~(1 << 12);
}

void PORTD_Int_Handler(void){ //PORTC/PORTD handler
    der = !(PORTC->ISFR & (1<<12)); //Comprobar que botón activo la interrupción
    izq = !(PORTC->ISFR & (1<<3));
    if(izq==1){
    	valor = valor+1;
    	izqb = true;
    }else if(der==1){
    	valor = valor-2;
    	derb = true;
    }
    
  if(izqb){
	if(valor%4==0){ //Ambos apagados
  		led_green_toggle(); 	
  		led_red_toggle();	
  	}else if(valor%4==1){ //Enciende rojo y apaga verde
  		led_red_toggle();
  	}else if(valor%4==2){ //Enciende verde apaga rojo
  		led_green_toggle();
  		led_red_toggle();
  	}else if(valor%4==3){ //Ambos encendidos
  		led_red_toggle();
  	}  
  }else if(derb){
  	led_green_toggle();
  	led_red_toggle();
  	if(valor%4==0){ //Ambos apagados
  		valor=3;	
  	}else if(valor%4==1){ //Enciende rojo y apaga verde
  		valor=2;
  	}else if(valor%4==2){ //Enciende verde apaga rojo
  		valor=1;
  	}else if(valor%4==3){ //Ambos encendidos
  		valor=0;
  	}  
  }
  

  derb = false;
  izqb = false;
  PORTC->PCR[3] |= PORT_PCR_ISF(1);
  PORTC->PCR[12] |= PORT_PCR_ISF(1);
}
int main(void)
{
  char ch;

  /* Init board hardware. */
  BOARD_InitPins();
  BOARD_BootClockRUN();
  BOARD_InitDebugConsole();
  
  SIM->COPC = 0; 
  sw1_init();
  sw2_init();
  led_green_init();
  led_red_init();
  NVIC_EnableIRQ(PORTC_PORTD_IRQn);

  PRINTF("\r\nReinicio!\r\n");

  while (1)
    {
      ch = GETCHAR();
      PUTCHAR(ch);
    }
}
