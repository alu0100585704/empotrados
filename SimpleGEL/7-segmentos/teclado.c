#include "teclado.h"

/*
 * @brief Funcion para inicializar el Decodificador y el Timer.
 */
 
void keyboard_init ()
{
	gpio_pup_enable(M6812_PORTH); ///activamos resistencia de pull up en el registro
	
	///configuramos puerto G con los pines para las filas como salida y valor 0(0 voltios)
  gpio_set_output(SET_PIN_H,0);
  gpio_set_output(SET_PIN_H,1);
  gpio_set_output(SET_PIN_H,3);
  gpio_set_output(SET_PIN_H,5);
  
  gpio_write_pin(SET_PIN_H, 0, 0);
  gpio_write_pin(SET_PIN_H, 1, 0);
  gpio_write_pin(SET_PIN_H, 3, 0);
  gpio_write_pin(SET_PIN_H, 5, 0);
  
  ///configuramos puerto G como entrada los pines de las columnas
  gpio_set_input(SET_PIN_H,2);
  gpio_set_input(SET_PIN_H,4);
  gpio_set_input(SET_PIN_H,6);
 
  
  
}
