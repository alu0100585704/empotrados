/* ****************************************
 * @author Juan Siverio Rojas
 * @author Felipe Esquivel Delgado
 * @date 18/12/2020
	************************************** */

#ifndef SIETESEG
#define SIETESEG

#include <stdio.h>
#include <timer.h>
#include <gpio.h>
#include <ad.h>

/*
 * @brief Función para convertir el valor recibido y mostrarlo por los display
 * correspondientes.
 * @param valor Valor en decimal que se desea mostrar por el decodificador que
 * está en el rango de 0-9999.
 */
void sieteSeg_valor (uint16_t valor);

/*
 * @brief Funcion para poder pasarle los valores directamente y poder
 * imprimirlos.
 * @param valor Puntero al vector de valores para asignar al vector que se
 * imprimirá en los distintos display
 */
void sieteSeg_digitos (uint8_t * valor);

/*
 * @brief Funcion para inicializar el Decodificador y el Timer.
 */
void sieteSeg_init ();

/*
 * @brief Funcion de refresco que escribe el valor en el display seleccionado
 * y va comprobando los siguientes cada cierto tiempo.
 */
void refresh ();

#endif
