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



char teclado_getch_timeout(uint32_t milis); // espera a que se realice una
//pulsación en el teclado y devuelve código ASCII Si no hay pulsación en
//milis milisegundos devuelve el carácter 'T'.

char teclado_getch();


/**
 * @brief Funcion para inicializar el Decodificador y el Timer.
 */
void teclado_init ();

#endif
