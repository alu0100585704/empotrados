/* ****************************************
 * @author Juan Siverio Rojas
 * @author Felipe Esquivel Delgado
 * @date 18/12/2020
	************************************** */

#ifndef TECLADO
#define TECLADO

#include <stdio.h>
#include <timer.h>
#include <gpio.h>

/// ASUMIMOS QUE los puertos
/// C2 F1 C1 F4 C3 F3 F2 se conectan  así:
///
/// F2 pin 42 del conector H1, equivale al pin 0  Puerto H del microcontrolador,
/// F3 pin 43 del conector H1, equivale al pin 1  Puerto H del microcontrolador,
/// C3 pin 44 del conector H1, equivale al pin 2  Puerto H del microcontrolador,
/// F4 pin 45 del conector H1, equivale al pin 3  Puerto H del microcontrolador,
/// C1 pin 46 del conector H1, equivale al pin 4  Puerto H del microcontrolador,
/// F1 pin 47 del conector H1, equivale al pin 5  Puerto H del microcontrolador,
/// C2 pin 48 del conector H1, equivale al pin 6  Puerto H del microcontrolador,

#define C2 64
#define F1 32
#define C1 16
#define F4 8
#define C3 4
#define F3 2
#define F2 1

/**
 * @brief Funcion para obtener la tecla pulsada en el teclado y, en caso de
 * no pulsar nada, mostrar el valor T cada cierto tiempo.
 * @param milis Valor que espera para obtener pulsación en el teclado.
 */
char teclado_getch_timeout(uint32_t milis);

/**
 * @brief Funcion obtener la tecla pulsada en el teclado.
 */
char teclado_getch();

/**
 * @brief Funcion para inicializar el Teclado.
 */
void teclado_init ();

#endif
