#include <ad.h>

/*******************************************************************************************
* Versión Interrupcion sin rutina de usuario.
******************************************************************************************/

int main () {

		uint8_t * valor;

    ad_pin_inicio(0,1);

		ad_modulo(0,1);

		ad_ocho_o_diez_bits(0,1);

    ad_ciclos_muestreo(0,1);

		ad_scanMode(0,1);

		ad_cantidadConversiones(0,1);

		///Hasta aquí se han recogido todos los datos en modo interactivo, para poder hacer las pruebas más fácilmente.

		ad_activarInterrupMode(1); ///Me aseguro de desactivar las interrupciones
		ad_setUserFunction(0); ///configuro una rutina de usuario.
    ad_start(); ///se inicia el proceso

	  while(1)
	  {
		  	valor = ad_returnValoresCambiados();
				if (*(valor)!=0)
				{
						serial_print("\nCambiaron los valores diciendolo fuera de la rutina");
						mostrarValores(1); ///muestro valores solo si han cambiado desde el anterior.
				}
	  }

}
