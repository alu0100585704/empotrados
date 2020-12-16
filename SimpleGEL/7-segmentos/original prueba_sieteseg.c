/* ****************************************
 * Se muestra un valor en el decodificador de
 * siete segmentos que se va aumentando si se
 * pulsa la tecla espaciadora.
 * @author Juan Siverio Rojas
 * @author Felipe Esquivel Delgado
 * @date 18/12/2020
	************************************** */

#include <stdio.h>
#include <timer.h>
#include <gpio.h>
#include "sieteseg.h"

int main ()
{
  serial_init();
  serial_print("\nFuncion del decodificador de Siete Segmentos");

  sieteSeg_init();

  uint8_t valor7Seg[] = {1, 2, 3, 4};
  sieteSeg_digitos(valor7Seg);

  char c;
  int number = 0;
  while (1)
  {
    while((c = serial_recv()) != ' '); 
    number ++;
    sieteSeg_valor(number);
  }
}
