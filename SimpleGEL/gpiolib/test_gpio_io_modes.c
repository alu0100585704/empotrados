
/**
 * Ejemplos de manejo de interrupciones para la librería E/S 
 * \file test_gpio_io_modes.c 
 * \author Daniel Paz Marcos
 * \author David Martín Martín
 * \date 02/12/20
 * Universidad de La Laguna
 **/

#include <gpio.h>
#include <sys/param.h>
#include <sys/ports.h>
#include <sys/interrupts.h>
#include <types.h>

/**
 * Programa de ejemplo
 * */
int main()
{

  serial_printbinbyte(_io_ports[SET_PIN_G]);
  serial_print("\n");
  serial_printbinbyte(_io_ports[SET_PIN_S]);
  serial_print("\n");

  gpio_set_output(SET_PIN_G, 1);
  gpio_set_output_all_reg(SET_PIN_S);

  serial_printbinbyte(_io_ports[SET_PIN_G]);
  serial_print("\n");
  serial_printbinbyte(_io_ports[SET_PIN_S]);
  serial_print("\n");

  return 0;
}
