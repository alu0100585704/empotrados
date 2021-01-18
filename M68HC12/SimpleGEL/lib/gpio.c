/**
 * Librería E/S
 * \file gpio.c
 * \author Daniel Paz Marcos
 * \author David Martín Martín
 * \date 02/12/20
 * Universidad de La Laguna
 **/

#include "gpio.h"

/**
 * Pone la flag de output a un pin, indicado en los define
 * */
void gpio_set_output(int reg, int port)
{
  _io_ports[reg] |= 1UL << port;
}

/**
 * Pone la flag de output a todos los pines de un tipo
 * */
void gpio_set_output_all_reg(int reg)
{
  _io_ports[reg] |= 0xFF;
}

/**
 * Pone el flag de input a un pin, indicado en los define
 * */
void gpio_set_input(int reg, int port)
{
  _io_ports[reg] &= ~(1UL << port);
}


/**
 * Pone el flag de input a todos los pines de un tipo
 * */
void gpio_set_input_all_reg(int reg)
{
  _io_ports[reg] &= 0x00;
}


/**
 * Vector de interrupciones.
 * */
void (*interrupt[])(void) = {SR_G, SR_H};


/**
 * Función que activa un pin de interrupción
 * */
void gpio_set_interrupt_pin(int reg, int port)
{
  _io_ports[reg] |= 1UL << port;
}

/**
 * Función que desactiva un pin de interrupción
 * */
void gpio_reset_interrupt_pin(int reg, int port)
{
  _io_ports[reg] &= ~(1UL << port);
}

/**
 * Función que desactiva todos los pines de interrupción de un registro
 * */
void gpio_reset_all_pins(int reg)
{
  _io_ports[reg] &= 0x00;
}

/**
 * Función que activa todos los pines de interrupción de un registro
 * */
void gpio_set_interrupt_all_pins(int reg)
{
  _io_ports[reg] |= 0xFF;
}


/**
 * Función que Maneja las interrupciones.
 * Es llamada al enviar una señal de interrupción por uno de los pines
 * */
void __attribute__((interrupt)) vi_kwgh(void)
{

  int reg;
  int port_g;
  int port;

  if (_io_ports[M6812_KWIFG] != 0x00) {

    port_g = 0;
    reg = _io_ports[M6812_KWIFG];

  }

  if (_io_ports[M6812_KWIFH] != 0x00) {

    port_g = 1;
    reg = _io_ports[M6812_KWIFH];

  }

  switch (reg) {

    case (0x01):
      port = 0;
      _io_ports[reg] &= ~(1UL << port);
      break;
    case (0x02):
      port = 1;
      _io_ports[reg] &= ~(1UL << port);
      break;
    case (0x04):
      port = 2;
      _io_ports[reg] &= ~(1UL << port);
      break;
    case (0x08):
      port = 3;
      _io_ports[reg] &= ~(1UL << port);
      break;
    case (0x10):
      port = 4;
      _io_ports[reg] &= ~(1UL << port);
      break;
    case (0x20):
      port = 5;
      _io_ports[reg] &= ~(1UL << port);
      break;
    case (0x40):
      port = 6;
      _io_ports[reg] &= ~(1UL << port);
      break;
    case (0x80):
      port = 7;
      _io_ports[reg] &= ~(1UL << port);
      break;

    default:
      port = -1;
      break;
  }

  if (port_g == 0) {

    interrupt[SR_G]();
  }
  else {
    interrupt[SR_H]();
  }

}


/**
 * Function to enable pullup on a register
 * */
int gpio_pup_enable(int reg) {
  int special_port = ((reg == M6812_PORTP) ||
      (reg == M6812_PORTS) ||
      (reg == M6812_PORTT) ||
      (reg == M6812_PORTCAN));


  if (!special_port) {
    //Enable pullup for ports A, B, E, G, H
    _io_ports[M6812_PUCR] |= reg;

  } else if (reg == M6812_PORTP){
    //Enable pup for port P
    _io_ports[M6812_PWCTL] |= 0x01 << 1;

  } else if (reg == M6812_PORTS){
    //Enable pup for port S
    unsigned char bits = _io_ports[M6812_PURDS];
    bits |= 0x07;
    _io_ports[M6812_PURDS] = bits;

  } else if (reg == M6812_PORTT){
    //Enable pup for port T
    _io_ports[M6812_TMSK2] |= 0x01 << 5;

  } else if (reg == M6812_PORTCAN){
    _io_ports[M6812_PCTLCAN] |= 0x02;
  } else {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}


/**
 * Function to disable pullup on a register
 * */
int gpio_pup_disable_(int reg) {
  int special_port = ((reg == M6812_PORTP) ||
      (reg == M6812_PORTS) ||
      (reg == M6812_PORTT) ||
      (reg == M6812_PORTCAN));


  if (!special_port) {
    //Enable pullup for ports A, B, E, G, H
    _io_ports[M6812_PUCR] &= ~reg;

  } else if (reg == M6812_PORTP){
    //Enable pup for port P
    _io_ports[M6812_PWCTL] &= ~(0x01 << 1);

  } else if (reg == M6812_PORTS){
    //Enable pup for port S
    unsigned char bits = _io_ports[M6812_PURDS];
    bits &= ~0x07;
    _io_ports[M6812_PURDS] = bits;

  } else if (reg == M6812_PORTT){
    //Enable pup for port T
    _io_ports[M6812_TMSK2] &= ~(0x01 << 5);

  } else if (reg == M6812_PORTCAN){
    _io_ports[M6812_PCTLCAN] &= ~0x02;
  } else {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}


/**
 * Función para escribir un pin
 * */
void gpio_write_pin(int port, unsigned char pin, unsigned char data) {
  unsigned char direction = 0;
  int direction_port = 0;
  unsigned short is_read_only = (port == M6812_PORTAD0) ||
    (port == M6812_PORTAD1);

  //If pin not in range return empy optional.
  if ((pin > 8)) {
    //serial_print("\nPin fuera de rango");
    return;
  }

  //If port is read only avoid further checks.
  if (is_read_only) {
    //serial_print("\nPuerto de solo lectura");
    return;
  }

  switch (port) {
    case M6812_PORTA:
      direction_port = M6812_DDRA;
      break;
    case M6812_PORTB:
      direction_port = M6812_DDRB;
      break;
    case M6812_PORTE:
      direction_port = M6812_DDRE;
      break;
    case M6812_PORTG:
      direction_port = M6812_DDRG;
      break;
    case M6812_PORTH:
      direction_port = M6812_DDRH;
      break;
    case M6812_PORTP:
      direction_port = M6812_DDRP;
      break;
    case M6812_PORTS:
      direction_port = M6812_DDRS;
      break;
    case M6812_PORTT:
      direction_port = M6812_DDRT;
      break;
    case M6812_PORTCAN:
      direction_port = M6812_DDRCAN;
      break;
  }

  direction = _io_ports[direction_port] & (0x01 << pin);
  if (!direction) {
    //serial_print("\nPin es de entrada");
    return;
  } else {
    char register_cpy = _io_ports[port];
    register_cpy = (register_cpy & (~pin));
    register_cpy = register_cpy | ((data & 1) << pin);
    //serial_print("\nregister_cpy:");
    //serial_printbinbyte(register_cpy);
    _io_ports[port] = register_cpy;
  }
}


/**
 * Función para leer un pin
 * */
Optional gpio_read_pin(int port, unsigned char pin) {
  Optional data;
  data.is_valid = 0;
  unsigned char direction = 0;
  int direction_port = 0;
  unsigned short is_read_only = (port == M6812_PORTAD0) ||
    (port == M6812_PORTAD1);

  //If pin not in range return empy optional.
  if ((pin >= 8)) {
	 serial_print("return\n");
    return data;
  }

  //If port is read only avoid further checks.
  if (is_read_only) {
	 serial_print("return2\n");
    data.is_valid = 0;
    data.data = _io_ports[port] & pin;
    return data;
  }

  switch (port) {
    case M6812_PORTA:
      direction_port = M6812_DDRA;
      break;
    case M6812_PORTB:
      direction_port = M6812_DDRB;
      break;
    case M6812_PORTE:
      direction_port = M6812_DDRE;
      break;
    case M6812_PORTG:
      direction_port = M6812_DDRG;
      break;
    case M6812_PORTH:
      direction_port = M6812_DDRH;
      break;
    case M6812_PORTP:
      direction_port = M6812_DDRP;
      break;
    case M6812_PORTS:
      direction_port = M6812_DDRS;
      break;
    case M6812_PORTT:
      direction_port = M6812_DDRT;
      break;
    case M6812_PORTCAN:
      direction_port = M6812_DDRCAN;
      break;
    default:
      return data;
  }

  direction = _io_ports[direction_port] & (0x01 << pin);
  if (direction != 0) {
	serial_print("El pin esta en modo escritura");
    return data;
  }

  data.data = !!(_io_ports[port] & (1 << pin));
  data.is_valid = 1;
  return data;
}


/**
 * Función para leer un puerto
 * */
Optional gpio_read_port(int port) {
  Optional data;
  if (port == M6812_PORTCAN) {

    data.is_valid = 0;
    data.data = 0;
    return data;
  }

  unsigned short read_mode_enabled;

  switch (port) {
    case M6812_PORTA:
      read_mode_enabled = ~(_io_ports[M6812_DDRA] | READ_ENABLED_PORT);
      break;
    case M6812_PORTB:
      read_mode_enabled = ~(_io_ports[M6812_DDRB] | READ_ENABLED_PORT);
      break;
    case M6812_PORTG:
      read_mode_enabled = ~(_io_ports[M6812_DDRG] | READ_ENABLED_PORT);
      break;
    case M6812_PORTH:
      read_mode_enabled = ~(_io_ports[M6812_DDRH] | READ_ENABLED_PORT);
      break;
    case M6812_PORTP:
      read_mode_enabled = ~(_io_ports[M6812_DDRP] | READ_ENABLED_PORT);
      break;
    case M6812_PORTS:
      read_mode_enabled = ~(_io_ports[M6812_DDRS] | READ_ENABLED_PORT);
      break;
    case M6812_PORTT:
      read_mode_enabled = ~(_io_ports[M6812_DDRT] | READ_ENABLED_PORT);
      break;
    case M6812_PORTAD0:
      read_mode_enabled = 1;
      break;
    case M6812_PORTAD1:
      read_mode_enabled = 1;
      break;
    case M6812_PORTE:
      read_mode_enabled = ~(_io_ports[M6812_DDRE] | READ_ENABLED_PORT);
      break;
    default:
      read_mode_enabled = 0;
      break;
  }

  if (read_mode_enabled) {
    data.is_valid = 1;
    data.data = _io_ports[port];
  } else {
    data.is_valid = 0;
    data.data = 0;
  }

  return data;
}


/**
 * Función para escribir un puerto
 * */
char gpio_write_port(int port, char data) {
  //Only allow to write to ports with all pins able to read
  int is_read_only = ((port == M6812_PORTAD0) ||
      (port == M6812_PORTAD1) ||
      (port == M6812_PORTE) ||
      (port == M6812_PORTCAN));

  if (is_read_only) {
    return EXIT_FAILURE;
  }

  //Check that the port is in write mode, if no return EXIT_FAILURE
  int is_write_enabled = 0;
  switch (port) {
    case M6812_PORTA:
      if (_io_ports[M6812_DDRA] & WRITE_ENABLED_PORT ) {
        is_write_enabled = 1;
      } else {
      }
      break;

    case M6812_PORTG:
      if (_io_ports[M6812_DDRG] & WRITE_ENABLED_PORT ) {
        is_write_enabled = 1;
      } else {
      }
      break;

    case M6812_PORTP:
      if (_io_ports[M6812_DDRP] & WRITE_ENABLED_PORT ) {
        is_write_enabled = 1;
      } else {
      }
      break;

    case M6812_PORTH:
      if (_io_ports[M6812_DDRH] & WRITE_ENABLED_PORT ) {
        is_write_enabled = 1;
      } else {
      }
      break;

    case M6812_PORTS:
      if (_io_ports[M6812_DDRS] & WRITE_ENABLED_PORT ) {
        is_write_enabled = 1;
      } else {
      }
      break;

    case M6812_PORTT:
      if (_io_ports[M6812_DDRT] & WRITE_ENABLED_PORT ) {
        is_write_enabled = 1;
      } else {
      }
      break;

    default:
      return EXIT_FAILURE;
  }

  if (!is_write_enabled) {
    return EXIT_FAILURE;
  }
  _io_ports[port] = data;
  return EXIT_SUCCESS;
}
