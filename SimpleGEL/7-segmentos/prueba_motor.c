#include <stdio.h>
#include <timer.h>
#include <gpio.h>
#include <pwm.h>

#include "motor.h"
#include "teclado.h"
#include "sieteseg.h"

int main()
{
  
  serial_init(); ///inicialiamos la UART
  serial_print("\nFuncion del Motor");

 char tecla='T';
 int8_t potencia = 0;

  
  pwm_set_clock(RELOJ_A,7,GRANO_FINO,3);
  pwm_set_polarity(0,ALTA);
  pwm_aling(ALINEAMIENTO_IZQ,0);
  pwm_period(200,0);
  pwm_modificar_etapas(20,0);


 //pwm_habilitacion_canal(0);
 pwm_modificar_porcentaje(5,0);


 teclado_init();
  sieteSeg_init(); ///inicializamos el 7 segmentos
 tecla = teclado_getch();
serial_print("\npasado siete segmentos");

  while (1)
  {

  serial_print("\nIntroduzca potencia del motor 0-9");
  serial_print("\nUtilize '*' para confirmar o '#' para cancelar");
   	
  while (tecla != '#') ///mientras sea distinto de cancelar
  {
	tecla = teclado_getch();  
	serial_print("\n");	
	serial_print(&tecla);
	

	if (tecla == '*')
	{
		  pwm_modificar_porcentaje(potencia,0);
	}
else if (tecla != '#')
	{
	  potencia = tecla - '0';
	  sieteSeg_valor((uint16_t) potencia);
	}	
	
  }
	sieteSeg_valor(0);
	tecla='T';
	  
  }
}
