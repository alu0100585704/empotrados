#include <stdio.h>
#include <timer.h>
#include <gpio.h>
#include <pwm.h>

#include "motor.h"
#include "teclado.h"
#include "sieteseg.h"

int main()
{
  sieteSeg_init(); ///inicializamos el 7 segmentos
  serial_init(); ///inicialiamos la UART
  pwm_set_clock(RELOJ_A,7,GRANO_FINO,3);
  pwm_set_polarity(0,ALTA);
  pwm_aling(ALINEAMIENTO_IZQ,0);
  pwm_period(200,0);
  pwm_habilitacion_canal(0);
  pwm_modificar_etapas(20,0);
  serial_print("\nFuncion del Motor");
  
 char tecla='T';
 int8_t potencia = 0;
  teclado_init();

  while (1)
  {

  serial_print("\nIntroduzca potencia del motor 0-100");
  serial_print("\nUtilize '*' para confirmar o '#' para cancelar");
   	
  while (tecla != '#') ///mientras sea distinto de cancelar
  {
	tecla = teclado_getch();  
	serial_print(&tecla);	
	if (tecla == '*')
	{
		  pwm_modificar_porcentaje(potencia,0);
	}
	if (tecla != '#')
	{
	  potencia = tecla - '0';
	  sieteSeg_valor((uint16_t) potencia);
	}	
	
  }
	sieteSeg_valor(0);
	  
  }
}
