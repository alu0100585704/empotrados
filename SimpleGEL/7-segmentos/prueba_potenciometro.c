/* ****************************************
 * Lee el valor proporcionado por el conversor
 * AD y muestra la conversión en el decodificador
 * de siete segmentos.
 * @author Juan Siverio Rojas
 * @author Felipe Esquivel Delgado
 * @date 18/12/2020
	************************************** */

#include <stdio.h>
#include <timer.h>
#include <gpio.h>
#include <ad.h>
#include "sieteseg.h"

/*
 * @brief Función de rutina de usuario.
 * @param valores Puntero a los valores recibidos de la conversión del ADC.
 */
void myUserFunction (uint16_t * valores)
{
  ///Se envía el valor obtenido solo si ha cambiado
  sieteSeg_valor(valores[0]);
}

int main ()
{
  serial_init();
  serial_print("\nFuncion del decodificador de Siete Segmentos utilizando ADC");
  sieteSeg_init();

  ad_pin_inicio(3,0); ///Se utiliza el pin 3 para obtener los valores
  ad_modulo(0,0); ///Se selecciona módulo cero
  ad_ocho_o_diez_bits(1,0); ///Resolución de 10 bits
  ad_ciclos_muestreo(3,0); ///16 ciclos por muestreo
  ad_scanMode(1,0); ///Se activa modo scan
  ad_cantidadConversiones(1,0); ///Solo se activa una conversión
  ad_activarInterrupMode(1); ///Se habilita el modo interrupción
  ad_setUserFunction(&myUserFunction); ///Se configura una rutina de usuario.

  ad_start(); ///Se inicia el proceso

  while (1);
}
