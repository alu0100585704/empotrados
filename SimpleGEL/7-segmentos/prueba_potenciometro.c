#include <stdio.h>
#include <timer.h>
#include <gpio.h>
#include <ad.h>
#include "sieteseg.h"

void myUserFunction(uint16_t * valores)
{
	
			sieteSeg_valor(valores[0]);  ///envío el valor obtenido solo si ha cambiado
}

int main ()
{

	serial_init();
	serial_print("\nFuncion del decodificador de Siete Segmentos utilizando ADC");
	
	sieteSeg_init();

	ad_pin_inicio(3,0); ///utilizaremos el pin 3 para la recogida de valores

	ad_modulo(0,0); ///módulo cero

	ad_ocho_o_diez_bits(1,0); ///resolución de 10 bits

	ad_ciclos_muestreo(3,0); ///16 ciclos por muestreo

	ad_scanMode(1,0); ///activo modo scan

	ad_cantidadConversiones(1,0); ///una conversión

	ad_activarInterrupMode(1); ///habilito el modo interrupción
	ad_setUserFunction(&myUserFunction); ///configuro una rutina de usuario.
	ad_start(); ///se inicia el proceso

	while (1);

}
