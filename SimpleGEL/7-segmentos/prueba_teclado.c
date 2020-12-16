#include <stdio.h>
#include <timer.h>
#include <gpio.h>
#include "teclado.h"

int main ()
{
  serial_init();
  serial_print("\nFuncion del Teclado");

Optional data;

  keyboard_init();

  
  while (1)
  {
	  data = pio_read_pin(SET_PIN_H, 2);	  	  
	  serial_print("\nPin 2 = Columna 3");
	  serial_printdecbyte(data.data);

  	  data = pio_read_pin(SET_PIN_H, 4);	  	  
	  serial_print("\nPin 2 = Columna 1");
	  serial_printdecbyte(data.data);
	  
  	  data = pio_read_pin(SET_PIN_H, 6);	  	  
	  serial_print("\nPin 2 = Columna 2");
	  serial_printdecbyte(data.data);


	  
  }
}
