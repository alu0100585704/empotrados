#include <ad.h>

/*******************************************************************************************
 * Versión Interrupcion con rutina de usuario.
 ******************************************************************************************/

/**
 * @brief Rutina establecida por el usuario, que se ejecutará después de la lectura de valores tanto en modo polling como por interrupción.
 * @param valores Puntero al vector de valores
 */
void rutinaUsuario(uint16_t * valores)
{
		serial_print("\nRutina usuario ejecutada");
}

int main () {

    ad_pin_inicio(0,1);

		ad_modulo(0,1);

		ad_ocho_o_diez_bits(0,1);

  	ad_ciclos_muestreo(0,1);

		ad_scanMode(0,1);

		ad_cantidadConversiones(0,1);

		///Hasta aquí se han recogido todos los datos en modo interactivo, para poder hacer las pruebas más fácilmente.

		ad_activarInterrupMode(1); ///Me aseguro de desactivar las interrupciones
		ad_setUserFunction(&rutinaUsuario); ///configuro una rutina de usuario.
    ad_start(); ///se inicia el proceso

		while(1)
				__asm__ __volatile__("wai");

}
