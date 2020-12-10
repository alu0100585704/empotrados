/**
 * Cabecera de librería E/S
 * \file gpio.h
 * \author Daniel Paz Marcos
 * \author David Martín Martín
 * \date 02/12/20
 * Universidad de La Laguna
 **/

/*! @defgroup gpio Librería Input Output
 *
 * Conjunto de funciones que facilitan la E/S del microprocesador.
 */

/*@{*/

#include <stdio.h>
#include <types.h>
#include <sys/param.h>
#include <sys/interrupts.h>
#include <sys/sio.h>
#include <sys/locks.h>
#include "../include/sys/ports_def.h"


/**
 * Pines permitidos para modos de IO
 **/
#define SET_PIN_A M6812_DDRA
#define SET_PIN_B M6812_DDRB
#define SET_PIN_E M6812_DDRE
#define SET_PIN_G M6812_DDRG
#define SET_PIN_H M6812_DDRH
#define SET_PIN_P M6812_DDRP
#define SET_PIN_S M6812_DDRS
#define SET_PIN_T M6812_DDRT


/**
 * puertos de interrupción de la librería
 * */
#define INTERRUPT_PIN_G M6812_KWIEG
#define INTERRUPT_PIN_H M6812_KWIEH

/**
 * Subrutinas de interrupción para los pines G y H
 * */
#define SR_G 0xFFCE
#define SR_H 0xFFCF


/**
 * Puertos para el PullUp
 * */
#define PIN_A1 M6812_DDRA << 3 | 0x00
#define PIN_A2 M6812_DDRA << 3 | 0x01
#define PIN_A3 M6812_DDRA << 3 | 0x02
#define PIN_A4 M6812_DDRA << 3 | 0x03
#define PIN_A5 M6812_DDRA << 3 | 0x04
#define PIN_A6 M6812_DDRA << 3 | 0x05
#define PIN_A7 M6812_DDRA << 3 | 0x06
#define PIN_A8 M6812_DDRA << 3 | 0x07

#define PIN 0x07
#define REGISTRO ~0x00 << 3

//gpio_set_output(PIN_A1);
//gpio_set_output(M6812_DDRA, M6812B_DDA0);

//pin
//_io_ports[pin & REGISTRO >> 3]


#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1


/**
 * Definiciones para lectura/escritura de puertos
 * */
#define WRITE_ENABLED_PORT 0XFF
#define READ_ENABLED_PORT 0X00


/**
 * Estructura para lectura/escritura
 * */
typedef struct Optional {
	char is_valid;
	unsigned char data;
} Optional;

/**
 * Estructura para lectura/escritura
 * */
typedef struct IOError {
	char code;
	char string[50];
} IOError;


void gpio_set_output(int reg, int port);

void gpio_set_output_all_reg(int reg);

void gpio_set_input(int reg, int port);

void gpio_set_input_all_reg(int reg);

void gpio_set_interrupt_pin(int reg, int port);

void gpio_reset_interrupt_pin(int reg, int port);

void gpio_reset_all_pins(int reg);

void gpio_set_interrupt_all_pins(int reg);

int gpio_pup_enable(int reg);

int gpio_pup_disable_(int reg);

void gpio_write_pin(int port, unsigned char pin, unsigned char data);

Optional gpio_read_pin(int port, unsigned char pin);

Optional gpio_read_port(int port);

char gpio_write_port(int port, char data);

/*@}*/
