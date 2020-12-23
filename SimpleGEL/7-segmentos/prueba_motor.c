/* ****************************************
 * Se introduce un valor en el teclado
 * y en funcion de ese valor, el motor 
 * gira a una determinada potencia
 * @author Juan Siverio Rojas
 * @author Felipe Esquivel Delgado
 * @date 18/12/2020
************************************** */

#include <stdio.h>
#include <timer.h>
#include <gpio.h>
#include <pwm.h>

#include "motor.h"
#include "teclado.h"
#include "sieteseg.h"

int main()
{ 
  serial_init(); ///inicialiamos la serial
  serial_print("\nFuncion del Motor");

  char tecla='T';
  int8_t potencia = 0;

  pwm_set_clock(RELOJ_A,7,GRANO_FINO,3);
  pwm_set_polarity(0,ALTA);
  pwm_aling(ALINEAMIENTO_IZQ,0);
  pwm_period(200,0);
  pwm_modificar_etapas(20,0);
  pwm_habilitacion_canal(0);
  pwm_modificar_porcentaje(0,0);

  teclado_init(); ///inicializamos el teclado
  sieteSeg_init(); ///inicializamos el 7 segmentos

  while (1)
  {
    serial_print("\nIntroduzca potencia del motor 0-9");
    serial_print("\nUtilize '*' para confirmar o '#' para cancelar");	

    tecla = teclado_getch();  
    serial_print("\n");	
    serial_send(tecla);

    if (tecla == '*')
    {
      serial_print("\nModificando potencia a: ");
      serial_printdecbyte(potencia);
      pwm_modificar_porcentaje(potencia,0);
    }

    if (tecla == '#')
    {
      potencia = 0;
      pwm_modificar_porcentaje(0,0);
      serial_print("\nModificando potencia a: ");
      serial_printdecbyte(potencia);
    }	

    tecla = tecla -'0';
 
    if ((tecla >= 0) && (tecla <=9))
    {
      serial_print("\nValor de  potencia a: ");
      serial_printdecbyte(tecla);
      potencia = tecla;

      sieteSeg_valor((uint16_t) potencia);
    }	  
  }
}
