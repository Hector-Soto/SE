#include "FreeRTOS.h"
#include "MKL46Z4.h"
#include "lcd.h"
#include "queue.h"
#include "task.h"
#include "string.h"

char cadena[] = "Supercalifragilistiaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaacocuespialidoso";
QueueHandle_t xQueue;
int pos; 
int tamCadena;
int tamCola = 7; 
char envio; 
char recibo; 
int cantidad;

void irclk_ini(){
  MCG->C1 = MCG_C1_IRCLKEN(1) | MCG_C1_IREFSTEN(1);
  MCG->C2 = MCG_C2_IRCS(0); //0 32KHZ internal reference clock; 1= 4MHz irc
}

void productor(void *pvParameters){
    int start = 0;
    for (;;) {
    	if(cantidad != tamCola){
    		if(pos<tamCadena){
    			envio = cadena[pos];
    			pos++;
		        const void * envio2 = (void *)&envio;
			xQueueSendToBack(xQueue, envio2, portMAX_DELAY);
		    	
			cantidad = uxQueueMessagesWaiting(xQueue);
			lcd_display_dec(cantidad);
    		}
    	}
    	vTaskDelay(100/portTICK_RATE_MS);
    }
}

void consumidor(void *pvParameters){
    for (;;) {
    	if(xQueue != NULL){
        	xQueueReceive(xQueue, &recibo, portMAX_DELAY);
    		cantidad = uxQueueMessagesWaiting(xQueue);
		lcd_display_dec(cantidad);
    	}
    	vTaskDelay(150/portTICK_RATE_MS);
    }
}

int main(void){
  pos = 0;
  tamCadena = strlen(cadena);
  
  irclk_ini();
  lcd_ini();

  xQueue = xQueueCreate(tamCola, sizeof( char ) );
  
  
  xTaskCreate(productor, (signed char *)"productor", 
		configMINIMAL_STACK_SIZE, NULL, 1, NULL);

  xTaskCreate(consumidor, (signed char *)"consumidor", 
		configMINIMAL_STACK_SIZE, NULL, 0, NULL);
		
  /* start the scheduler */
  vTaskStartScheduler();
  
  for (;;);

  return 0;
}
