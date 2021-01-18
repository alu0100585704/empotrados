/* ****************************************
 * Programa para mostrar la tecla pulsada
 * del teclado por consola
 * @author Juan Siverio Rojas
 * @author Felipe Esquivel Delgado
 * @date 18/12/2020
	************************************** */

#include <stdio.h>
#include <timer.h>
#include <gpio.h>
#include "teclado.h"

int main ()
{
  serial_init();
  serial_print("\nFuncion del Teclado");

  teclado_init();
  serial_print("\nPulse cualquier tecla del teclado: ");

  char tecla;
  tecla = teclado_getch();
  serial_print("\nPulsada tecla : ");
  serial_print(&tecla);
  serial_print("\nPulsar tecla del teclado o se escribirá T a los 2 segundos: ");

  while (1)
  {
    tecla = teclado_getch_timeout(2000);
    serial_print("\nPulsada tecla : ");
    serial_print(&tecla);
  }
}
