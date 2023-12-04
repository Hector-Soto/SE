#include "MKL46Z4.h"
#include "lcd.h"
//openocd
//Indicar minutos y segundos iniciales
int Min = 1;
int Seg = 30;
//1 para TPM y 2 para LPTMR
int timer = 2;

// Enable IRCLK (Internal Reference Clock)
// see Chapter 24 in MCU doc
void irclk_ini()
{
  MCG->C1 = MCG_C1_IRCLKEN(1) | MCG_C1_IREFSTEN(1);
  MCG->C2 = MCG_C2_IRCS(0); //0 32KHZ internal reference clock; 1= 4MHz irc
}

void tpm_ini(){
	SIM->SCGC6|=SIM_SCGC6_TPM0(1); //Habilitar clock para TPM0
	SIM->SOPT2|=SIM_SOPT2_TPMSRC(3); //MCGIRCLK
	TPM0->MOD=98500;
	TPM0->SC &= ~TPM_SC_PS(0); 
	TPM0->SC |= TPM_SC_TOIE(1) | TPM_SC_CMOD(1);
	//31.3.1 PS(0): divide by 1, CMOD(1):PM counter increments on every TPM counter clock: habilitar tpm0, TOIE(1):Timer Overflow Interrupt Enable
	NVIC_EnableIRQ(TPM0_IRQn);
}

void lptmr_ini(){
	SIM->SCGC5|=SIM_SCGC5_LPTMR(1); //Habilitar clock para LPTMR0
	LPTMR0->CMR=16500; //Compare value
	LPTMR0->CSR |= LPTMR_CSR_TIE(1) | LPTMR_CSR_TEN(1) | LPTMR_CSR_TCF(1);
	//TIE: habilit interrup, TEN: iniciar LPTMR, TCF: cnr(counter value)=cmr
	NVIC_EnableIRQ(LPTMR0_IRQn);
}

void FTM0IntHandler(void){ //FTM0IntHandler
	TPM0->SC|= TPM_SC_TOF_MASK; //Clear interrupt request flag
	if(Seg>0){
		Seg--;
	}else{
		if(Min>0){
			Min--;
			Seg=59;
		}else{
			TPM0->SC |= ~TPM_SC_CMOD_MASK; //Desactivar TPM0
		}
	}
	lcd_display_time(Min, Seg);
}

void LPTMRIntHandler(void){
	LPTMR0->CSR |= LPTMR_CSR_TCF(1); //Clear interrupt request flag
	if(Seg>0){
		Seg--;
	}else{
		if(Min>0){
			Min--;
			Seg=59;
		}else{
			LPTMR0->CSR &= ~LPTMR_CSR_TEN_MASK; //Desactiva LPTMR
		}
	}
	lcd_display_time(Min, Seg);
}

int main(void)
{
  irclk_ini();
  lcd_ini();
  lcd_display_time(Min, Seg);
  if(timer==1){
  	tpm_ini();
  }else if(timer==2){
  	lptmr_ini();
  }
  
  while (1) {
  }

  return 0;
}
