#include "MKL46Z4.h"
#include <stdbool.h>
// LED (RG)
// LED_GREEN = PTD5  porto D pin 5
// LED_RED = PTE29

bool x=false, y=false;
int a, b, v=0, w=0;
  
void delay(void)
{
  volatile int i;
  for (i = 0; i < 1000000; i++);
}

// LED_GREEN = PTD5
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

// LED_RED = PTE29
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

void PORTDIntHandler(void){ //PORTC/PORTD handler
    a = !(PORTC->ISFR & (1<<3)); //Comprobar que botón activo la interrupción
    b = !(PORTC->ISFR & (1<<12));
    
    if(a==1){
    	x=!x;
    }
    if(b==1){
    	y=!y;
    }
    
    if(x==false && y==false && ( (v==1 && w==0) || (v==0 && w==1) ) ){//00
    	v=0;
    	w=0;
    	led_green_toggle();
    	led_red_toggle();
    }else if( x==false && y==true){ //01
    	if(v==0 && w == 0){
    		led_green_toggle();
    		led_red_toggle();
    	}
    	v=0;
    	w=1;
    }else if( x==true && y==false ){ //10
    	if(v==0 && w == 0){
    		led_green_toggle();
    		led_red_toggle();
    	}
    	v=1;
    	w=0;
    }else if( x==true && y==true ){ //11
    	v=1;
    	w=1;
    }
    
    PORTC->PCR[3] |= PORT_PCR_ISF(1);
    PORTC->PCR[12] |= PORT_PCR_ISF(1);
}

int main(void)
{
  SIM->COPC = 0; 
  sw1_init();
  sw2_init();
  led_green_init();
  led_red_init();
  led_green_toggle();
  NVIC_EnableIRQ(PORTC_PORTD_IRQn);//Habilitar IRQ en portC
  while (1) {   
  }

  return 0;
}
