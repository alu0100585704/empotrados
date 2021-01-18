#include <ad.h>

/*******************************************************************************************
 * Versión Polling con rutina de usuario.
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

		uint16_t * valores;

  	ad_pin_inicio(0,1);

		ad_modulo(0,1);

		ad_ocho_o_diez_bits(0,1);

  	ad_ciclos_muestreo(0,1);

		ad_scanMode(0,1);

		ad_cantidadConversiones(0,1);

		///Hasta aquí se han recogido todos los datos en modo interactivo, para poder hacer las pruebas más fácilmente.

		ad_activarInterrupMode(0); ///Me aseguro de desactivar las interrupciones
		ad_setUserFunction(&rutinaUsuario); ///configuro una rutina de usuario.
	  ad_start(); ///se inicia el proceso

		while (1)
		{
				valores = ad_wait_for_data(); ///espero a una lectura total de la secuencia. También se ejecutaría la rutina de usuario en caso de definirla.
				///ahora podría leer los valores mediante el puntero valores o accediendo directamente  al array  global valoresRegistros_[8];
				mostrarValores(1); ///muestro valores solo si han cambiado desde el anterior.
		}

}
