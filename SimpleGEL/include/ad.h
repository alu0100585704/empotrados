#ifndef _AD_H_
#define _AD_H_

#include <types.h>
#include <sys/param.h>
#include <sys/interrupts.h>
#include <sys/sio.h>
#include <sys/locks.h>


/*! @defgroup ad Funciones de manejo del conversor A/D
 * La rutina de usuario, se ejecutará tanto en el modo initerrupción como en el modo polling.
 * Justo después de leerse los registros de datos correspondientes, se llamará a la rutina de usuario, si así se definió.
 * Prácticamente todas las funciones de la librerían disponen de un modo interactivo, para solicitar
 * al usuario el dato necesario para la configuración de una funcionalidad en concreto. Por supuesto,
 * todas se pueden configurar de forma no interactiva.
 * Los resultados de las lecturas de los registros de datos, se mantendrán actualizadas en un array,
 * ya sea mediante polling o por la rutina de interrupción en caso de así haberlo decidido el programador.
 *
 * Por defecto los siguientes bits se configuraran de la siguiente manera:
 *
 * ATDxCTL2 / AFFC = 1 (Basta con acceder a registro de datos para restablecer bit ATDxSTAT0-SCF que indica secuencia terminada
 * ATDxCTL2 / DJM = 0 (Justificación a izquierda)
 * ATDxCTL3 / FIFO = 0 (Modo FIFO desactivado)
 *
 * Inicialmente, el FIFO estará siempre deshabilitado, y la justificación se trabajará siempre a izquierda.
 * Todo lo demás si se podrá parametrizar.
 */

/*@{*/


/**
 * @brief	Una posible función prototipo sería:  "void userFunction(uint16_t * registros)" donde
 * 		registros contendrá de 0 a 7 valores, dependiendo del tamaño de la secuencia seleccionada
 * @param	ptr El parámetro será la dirección de la función del usuario.
 * 		Recuerde que debe de tener un parámetro en el que recibirá el puntero al array con los valores de los registros.
 */
void ad_setUserFunction(void (*ptr)(uint16_t *));

/**
 * @brief	Función que activa o desactiva el tratamiento de los registros de datos mediante interrupción.
 * @param	valor	Si está a 1, se activan las interrupciones, si está a 0, se desactivan
 */
void ad_activarInterrupMode(uint8_t valor);

/**
 * @brief	Función para esperar a que se termine la conversión, modo Polling.
 * 		Además, se llamará a la función de usuario en caso de haber sido asignada.
 */
uint16_t * ad_wait_for_data(); ///el array devuelto el la variable valoresRegistros_[8];

/**
 * @brief	Función para seleccionar el valor de los ciclos de muestreo (2,4,8,16)
 * @param	valor Valor que se define por defecto, en caso de que el usuario no desee modificar nada
 * @param	interactivo Está a 1 si el usuario quiere interactuar
 */
void ad_ciclos_muestreo (uint8_t valor, uint8_t interactivo);

/**
 * @brief	Función para seleccionar el valor de la conversión (8 o 10 bits)
 * @param	valor Valor que se define por defecto, en caso de que el usuario no desee modificar nada
 * @param	interactivo Está a 1 si el usuario quiere interactuar
 */
void ad_ocho_o_diez_bits (uint8_t valor, uint8_t interactivo);

/**
 * @brief	Función para mostrar el tiempo de muestreo
 */
void ad_tiempo_muestreo ();

/**
 * @brief	Función para seleccionar el pin de inicio en el microcontrolador
 * @param	valor Valor que se define por defecto, en caso de que el usuario no desee modificar nada
 * @param	interactivo Está a 1 si el usuario quiere interactuar
 */
void ad_pin_inicio (uint8_t valor, uint8_t interactivo);

/**
 * @brief	Función para seleccionar el módulo correspondiente al conversor A/D
 * @param	valor Valor que se define por defecto, en caso de que el usuario no desee modificar nada
 * @param	interactivo Está a 1 si el usuario quiere interactuar
 */
void ad_modulo (uint8_t valor, uint8_t interactivo);

/**
 * @brief	Función que muestra los valores de los registros dependiendo de la configuración.
 * 		Se puede decidir mostrar solo si se produjeron cambios con respecto a la secuencia anterior.
 * @param	soloCambios Si soloCambios está a 1, se mostrarán solo los cambios.
 */
void ad_mostrarValores(uint8_t soloCambios);

/**
 * @brief	Función interna de la librería que se encarga de leer los valores de los registros,
 * 		dependiendo de la cantidad de conversiones seleccionadas.
 *		Se asume que la justificación siempre es a izquierda
 */
void ad_leer_valores(void);

/**
 * @brief	Función para activar o desactivar el modo SCAN
 * @param	valor Valor que se define por defecto, en caso de que el usuario no desee modificar nada
 * 		En este caso, se activará el modo SCAN
 * @param	interactivo Está a 1 si el usuario quiere interactuar
 */
void ad_scanMode(uint8_t valor, uint8_t interactivo);

/**
 * @brief	Función para establecer el número de conversiones a 1, 4 u 8 por secuencia
 * @param	valor Valor que se define por defecto, en caso de que el usuario no desee modificar nada
 * @param	interactivo Está a 1 si el usuario quiere interactuar
 */
void ad_cantidadConversiones(uint8_t valor, uint8_t interactivo);

/**
 * @brief	Función que detiene el funcionamiento del módulo analógico digital
 */
void ad_stop();

/**
 * @brief	Función para completar los parámetros y dar por iniciada la conversión.
 *		En ella se hace uso de los atributos globales para usarlos como máscara
 *		para completar los registros correctamente.
 */
void ad_start();

/**
 * @brief	Funcion para retornar la posición del array de valores de los registros
 */
uint16_t * ad_returnValores(void);

/**
 * @brief	Funcion para retornar la posicion del array de los valores que han cambiado
 */
uint8_t * ad_returnValoresCambiados(void);

/*@}*/
#endif
