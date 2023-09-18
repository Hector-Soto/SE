#include "MKL46Z4.h"
//se usa mas defines q variables, porq var usan mucha mem y los define solo sustituyen
// LED (RG)
// LED_GREEN = PTD5  porto D pin 5
// LED_RED = PTE29

void delay(void)
{
  volatile int i;
//deicr cmpilador q no looptimice, q no lo salte
  for (i = 0; i < 1000000; i++);
}

// LED_GREEN = PTD5
void led_green_init()
{
  SIM->COPC = 0;
  SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
  PORTD->PCR[5] = PORT_PCR_MUX(1); //mux, alternativa 1 y activa dicha funcion
  GPIOD->PDDR |= (1 << 5); //pin 5 decimos q es de salida
  GPIOD->PSOR = (1 << 5);  //psor de configurar
}
//gated signal: q estan apagadas, la señal
//abajo loestamos encendiendo lo q qremos usar
void led_green_toggle()
{
  GPIOD->PTOR = (1 << 5);
}

// LED_RED = PTE29
void led_red_init()
{//reg y se tratan como enderezo memoria
  SIM->COPC = 0; //sim: system integration mobile: COPC->watchdog, lo desabilia, pone los 32 bits a 0, el num 3y2 es pa desabilit o no
  SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK; //activa porD, bit 12 es porD, hay q ponerle1.         & bit a bit, && no es pa bits     es un define a lo q iguala,u es sinsigno, unsigned: bit 12 a 1 y resto a 0, 16 en total
  PORTE->PCR[29] = PORT_PCR_MUX(1);
  GPIOE->PDDR |= (1 << 29);
  GPIOE->PSOR = (1 << 29);
}

void led_red_toggle(void)
{
  GPIOE->PTOR = (1 << 29);
}

int main(void)
{
  SIM->COPC = 0;
  led_green_init();
  led_red_init();
  while (1) {
    led_green_toggle();
    delay();
    led_green_toggle();
    led_red_toggle();
    delay();
    led_red_toggle();
  }

  return 0;
}
