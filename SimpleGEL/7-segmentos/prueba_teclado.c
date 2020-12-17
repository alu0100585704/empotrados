#include <stdio.h>
#include <timer.h>
#include <gpio.h>
#include "teclado.h"


int main ()
{
  serial_init();
  serial_print("\nFuncion del Teclado");

Optional data;

  teclado_init();

  
  while (1)
  {
///    serial_print("\n\nPulsa para nueva captura");
   // serial_recv();

 char tecla;
    tecla = teclado_getch();	 
    serial_print("\n\nPulsada tecla : ");
     serial_print(&tecla);

	  
  }
}
