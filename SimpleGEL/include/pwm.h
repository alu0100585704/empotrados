/**
 * @file pwm.h
 * @brief Fichero contenedor de los nombres de funciones
 * @author Diego Cruz Rodríguez
 * @author Carlos Socas García
 * @date 03/12/2020
*/


#ifndef _PWM_H_
#define _PWM_H_

#include <sys/sio.h>
#include <sys/param.h>
#include <sys/ports.h>
#include <sys/interrupts.h>
#include <types.h>
#include <sys/sio.h>
#include <sys/locks.h>

typedef enum { false, true } bool;

#define ALTA 1
#define BAJA 0
#define RELOJ_A 1
#define RELOJ_B 0
#define RETURN 0
#define GRANO_FINO 1
#define GRANO_GRUESO 0
#define ALINEAMIENTO_IZQ  1
#define ALINEAMIENTO_CENT  0

/*! @defgroup pwm Funciones de manejo del módulo PWM
 */

/*@{*/


/**  Configurar el reloj base para un determinado canal.
 *
 *   La configuracion del reloj del pwm se basa en la division
 *   del reloj base de la placa (25Mhz) apartir de ellos
 *   se puede usar una  presicion de grano fino o de grano grueso.
 *   En la precion de grano grueso, se emplea la siguiente formula
 *   Reloj_base / 2^N = Reloj_PWM.  Si emplemaos la presicion de
 *   grano fino apartir de reloj generado en el grano grueso se
 *   aplica la siguiente formula. Reloj_PWM /2 *(valor_contador + 1)
 *   = Reloj_PWM_Grano_fino.
 *
 *  @param Reloj_A Define el reloj que emplea cada canal,
 *  el reloj A alimenta los canales 0 y 1, y el reloj B los canales
 *  2 y 3
 *  @param factor_N Define el valor de N
 *  @param grano_fino Define si se emplea el precion de grano
 *  fino o grueso
 *  @param valor_contador   Define el valor del contador del grano
 *  fino
 */
void pwm_set_clock(bool Reloj_A,
				   int8_t factor_N,
				   bool grano_fino,
				   int8_t valor_contador);

/**  Configurar la polaridad para un determinado canal
 *
 *   La configuración de la polaridad establece el modo de funcionamiento
 *   de los canales, alta o baja
 *
 *  @param puerto_seleccionado Establece el puerto
 *  @param Alta Establece el tipo de polaridad.
 */
void pwm_set_polarity(int8_t puerto_seleccionado, bool Alta);

/**  Configurar el alineamiento para un determinado canal
 *
 *   La función nos permite establecer el tipo de alineado para poder
 *   modificar los pulsos de reloj
 *
 *  @param alineamiento Define el alineamiento
 *  @param canal_seleccionado Define el canal
 */
void pwm_aling(bool alineamiento, int8_t canal_seleccionado);

/**  Configurar el números de etapas del periodo para un determinado canal
 *
 *  Permite establecer el número de etapas por periodo
 *
 *  @param etapas_ Establece el número de etapas que se añade
 *  @param canal_ Determina el canal donde afecta la función
 */
void pwm_period( int8_t etapas_, int8_t canal);


/**  Habilitación de canal especificado
 *
 *
 *  @param canal_ Determina el canal donde afecta la función
 */
void pwm_habilitacion_canal(int8_t canal);

/**  Deshabilitación de canal especificado
 *
 *
 *  @param canal_ Determina el canal donde afecta la función
 */
void pwm_deshabilitacion_canal(int8_t canal);


/**  Modificar ciclo de trabajo en etapas
 *
 * 	 Modifica el numero de ciclos  segun las etapas de trabajo
 *
 *  @param canal Determina el canal donde afecta la función
 *  @param etapas_alta
 *  @param etapas_baja
 *  @param alineamiento_izq
 */
void pwm_modificar_etapas(int8_t etapas, int8_t canal);

/**  Modificar ciclo de trabajo en etapas
 *
 * 	 Modifica el numero de ciclos segun un porcentaje
 *
 *  @param porcentaje Determina el canal donde afecta la función
 *  @param canal
 *  @param alineacion_izquierda
 */
void pwm_modificar_porcentaje(int8_t porcentaje, int8_t canal);

/*@}*/

#endif //_PWM_H_

