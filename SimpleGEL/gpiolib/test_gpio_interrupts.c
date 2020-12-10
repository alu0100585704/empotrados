/**
 * Ejemplos de Modos de IO para la librería E/S 
 * \file test_gpio_interrupts.c 
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

  serial_printbinbyte(_io_ports[INTERRUPT_PIN_G]);
  serial_print("\n");

  gpio_set_interrupt_pin(INTERRUPT_PIN_G, 1);

  serial_printbinbyte(_io_ports[INTERRUPT_PIN_G]);
  serial_print("\n");

  serial_printbinbyte(_io_ports[INTERRUPT_PIN_H]);
  serial_print("\n");

  gpio_set_interrupt_all_pins(INTERRUPT_PIN_H);

  serial_printbinbyte(_io_ports[INTERRUPT_PIN_H]);
  serial_print("\n");


  return 0;
}
