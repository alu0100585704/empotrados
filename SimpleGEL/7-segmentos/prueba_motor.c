#include <stdio.h>
#include <timer.h>
#include <gpio.h>
#include <pwm.h>

#include "motor.h"
#include "teclado.h"
#include "sieteseg.h"

int main()
{
	
  serial_init();
  serial_print("\nFuncion del Motor");
  
 char tecla;

  teclado_init();
  serial_print("\nPulse cualquier tecla del teclado: ");
	
  tecla = teclado_getch();
  serial_print("\nPulsada tecla : ");
  serial_print(&tecla);
  serial_print("\nPulse cualquier tecla del teclado o se recibirá T a los dos segundos: ");

  while (1)
  {



    tecla = teclado_getch_timeout(2000);
    serial_print("\nPulsada tecla : ");
     serial_print(&tecla);
	
	  
  }
}
