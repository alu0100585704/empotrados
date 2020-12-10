#include "ad.h"

/**
 * Variables globales.
 * Las funciones configuran estas variables, y algunas retorna los valores necesarios.
 * Aún así, se podrían configurar directamente ya que son variables globales
 */

uint16_t DirAD_; 		/** Desplazamiento de la dirección para escoger módulo 0 o 1 */
uint8_t Pin_; 			/** Pin dentro del puerto del que se va a leer */
uint8_t Resolucion_; 		/** Resolución de los datos (8 o 10 bits) */
uint8_t ciclosMuestreo_; 	/** Ciclos de muestreo de 2, 4, 8 o 16 */
uint8_t scanMode_; 		/** Está a 1 si se activa el modo scan y a 0 si no lo está */
uint8_t cantidadConversiones_;  /** Cantidad de conversiones 1,4,8 */
uint8_t interrupMode_; 		/** Está a 1 si se activa la interrupción o a 0 si funciona en modo polling */
uint8_t valoresCambiados_; 	/** Se pondrá a 1 si una secuencia contiene algún valor que difiere de la secuencia
																anterior, o sea, compara valoresRegistros_ con valoresAnterioresRegistros_ */
uint16_t valoresRegistros_[8];  /** Desde el cero al siete se almacenará el valor de los registris ADRxnH(bits 15-8) y ADRxnL (bits 7-0) */
uint16_t valoresAnterioresRegistros_[8]; /** Para comprobar si hay diferencias */

void (* punteroUserFunction_)(uint16_t *) = 0; ///Puntero hacia una función

/**
 * @brief
 * @param	ptr El parámetro será la dirección de la función del usuario. Recuerde que
 *						debe de tener un parámetro en el que recibirá el puntero al array con los valores de los registros.
 *						Una posible función prototipo sería:  "void userFunction(uint16_t * registros)" donde registros
 *						contendrá de 0 a 7 valores, dependiendo del tamaño de la secuencia seleccionada
 * @param	interactivo	está a 1 si el usuario quiere interactuar
 */
void ad_setUserFunction(void (*ptr)(uint16_t *))
{
		punteroUserFunction_ = ptr; ///asignada función definida por el usuario.
}

/**
 * @brief	Función que activa o desactiva el tratamiento de los registros de datos mediante interrupción.
 * @param	valor		si 1, se activan las interrupciones, si 0, se desactivan
 */
void ad_activarInterrupMode(uint8_t valor)
{
		interrupMode_ = valor;
}

/**
 * @brief	Manejador interrupciones del atd (conversor analógica digital
 *  	  	Este es el manejador principal, el cual leerá los registros de
 *  	  	datos y llamará a la función definida por el usuario en caso de haberse definido.
 */
void __attribute__((interrupt))vi_atd(void) {

		///ahora se leen los registros según la configuración. Además esta lectura, restablecerá el bit de secuencia terminada al tener ATDxCTL2 / AFFC a 1.
 		ad_leer_valores();

  	if (punteroUserFunction_!= 0) ///si se estableció una rutina personalizada de usuario, se la llama.
				(*punteroUserFunction_)(&valoresRegistros_[0]);
}

/**
 * @brief	Función para esperar a que se termine la conversión, modo Polling.
 * 		Además, se llamará a la función de usuario en caso de haber sido asignada.
 */
uint16_t * ad_wait_for_data()
{
		/* Esperamos a que se termine la conversión modo polling*/
		while (! (_io_ports[M6812_ATD0STAT0 + DirAD_] & M6812B_SCF));

		///ahora se leen los registros según la configuración. Además esta lectura, restablecerá el bit de secuencia terminada al tener ATDxCTL2 / AFFC a 1.
		ad_leer_valores();

	 	if (punteroUserFunction_!= 0) ///si se estableció una rutina personalizada de usuario, se la llama.
				(*punteroUserFunction_)(&valoresRegistros_[0]);

		return  &valoresRegistros_[0];
}

/**
 * @brief	Función para seleccionar el valor de los ciclos de muestreo (2,4,8,16)
 * @param	valor valor que se define por defecto, en caso de que el usuario no desee modificar nada
 * @param	interactivo está a 1 si el usuario quiere interactuar
 */
void ad_ciclos_muestreo (uint8_t valor, uint8_t interactivo)
{
		char c;
		if (interactivo)
		{
				serial_print("\nCiclos de Muestreo de 2, 4, 8 o 16 Presione: (0 = 2, 1 = 4, 2 = 8 y 3 = 16):");
	     	while((c = serial_recv()) < '0' || c > '3');
	    	serial_send(c); /* a modo de confirmación*/
	     	ciclosMuestreo_ = c - '0';
		}
		else
				ciclosMuestreo_ = valor;
}

/**
 * @brief	Función para seleccionar el valor de la conversión (8 o 10 bits)
 * @param	valor valor que se define por defecto, en caso de que el usuario no desee modificar nada
 * @param	interactivo está a 1 si el usuario quiere interactuar
 */
void ad_ocho_o_diez_bits (uint8_t valor, uint8_t interactivo)
{
		char c;
		if (interactivo)
		{
				serial_print("\nResolución de 8 o 10 Bits Presione: (0 para 8 bits o 1 para 10 bits):");
	     	while((c = serial_recv()) < '0' || c > '1');
	    	serial_send(c); /* a modo de confirmación*/
	     	Resolucion_ = c - '0';
		}
		else
				Resolucion_ = valor;
}

/**
 * @brief	Función para mostrar por pantalla el tiempo de muestreo segun los
 *		parámetros configurados anteriormente.
		Cada conversion dura (en ciclos del submodulo):
			2 ciclos de captura inicial
			4 ciclos de copia del potencial
			2, 4, 8 o 16 ciclos de muestreo (configurables)
			10/12 ciclos de conversion dependiendo de la resolución pedida (8 o 10 bits configurables).
 */
void ad_tiempo_muestreo ()
{
		uint8_t sum = 0;

		uint8_t aux = Resolucion_ + '0';
		if (aux == 0)
				sum += 10;
		else
				sum += 12;

		aux = ciclosMuestreo_ + '0';
		switch (aux)
		{
				case 0:
						sum += 2;
						break;
				case 1:
						sum += 4;
						break;
				case 2:
						sum += 8;
						break;
				case 3:
						sum += 16;
						break;
				default:
						break;
		}
		sum += sum + 6;

		serial_print("\nTiempo de muestreo en ciclos según la configuración realizada: ");
		serial_send(sum);
}

/**
 * @brief	Función para seleccionar el pin de inicio en el microcontrolador
 * @param	valor valor que se define por defecto, en caso de que el usuario no desee modificar nada
 * @param	interactivo está a 1 si el usuario quiere interactuar
 */
void ad_pin_inicio (uint8_t valor, uint8_t interactivo)
{
		char c;
		if (interactivo)
		{
				serial_print("\nPin del puerto a utilizar Presione: (0 - 7)?:");
	     	while((c = serial_recv()) < '0' || c > '7');
	    	serial_send(c); /** a modo de confirmación */
	    	Pin_ = c - '0';
		}
		else
				Pin_ = valor;
}

/**
 * @brief	Función para seleccionar el módulo correspondiente al conversor A/D
 * @param	valor valor que se define por defecto, en caso de que el usuario no desee modificar nada
 * @param	interactivo está a 1 si el usuario quiere interactuar
 */
void ad_modulo (uint8_t valor, uint8_t interactivo)
{
		char c;
		if (interactivo)
		{
				serial_print("\nMódulo conversor a utilizar Presione (0 = Módulo 0, 1 = Módulo 1)");
				while((c = serial_recv()) != '0' && c != '1');
				serial_send(c); /** a modo de confirmación*/
				DirAD_ = (c == '0')?0:(M6812_ATD1CTL0 - M6812_ATD0CTL0);
		}
		else
		{
				if (valor)
						DirAD_ = (M6812_ATD1CTL0 - M6812_ATD0CTL0);
				else
						DirAD_ = 0;
		}
}

/**
 * @brief	Función que muestra los valores de los registros dependiendo de la configuración.
 * 		Se puede decidir mostrar solo si se produjeron cambios con respecto a
 * 		la secuencia anterior.
 * @param	soloCambios Si soloCambios = 1, se mostrarán solo los cambios.
 */
void ad_mostrarValores(uint8_t soloCambios)
{
		uint8_t i;
		if (soloCambios==1)
		{
			 	if (valoresCambiados_) ///solo muestro el valor de los registros si hubo algún cambio de valor con respecto a la lectura anterior
			 	{
						for (i = 0; i < cantidadConversiones_; i++)
						{
								serial_print("\nValor Registro = ");
								serial_printdecword(valoresRegistros_[i]);
								serial_send('\n');
						}
			 	}
		}
		else
		{
				for (i = 0; i < cantidadConversiones_; i++)  ///muestro el valor de todos los registros aunque sean iguales a la lectura anterior
				{
						serial_print("\nValor Registro = ");
						serial_printdecword(valoresRegistros_[i]);
						serial_send('\n');
				}
		}
}

/**
 * @brief	Función interna de la librería que se encarga de leer los valores
 * 		de los registros, dependiendo de la cantidad de conversiones seleccionadas.
 *		Se asume que la justificación siempre es a izquierda
 */
void ad_leer_valores(void)
{
		uint8_t i = 0;
		uint16_t borrador = 0;
		for (i = 0; i < 8; i++)
				valoresAnterioresRegistros_[i] = valoresRegistros_[i];  ///primero se copia el estado de los registros actualmente, bueno, de su valor en memoria de los últimos leídos

		valoresCambiados_ = 0; ///por defecto, parte de la base de que los valores no han cambiado.

		///Siempre se comienza leyendo desde el registro cero, ya que el modo FIFO no está contemplado en esta librería.
		///Además se da por hecho que la justificación es a izquierda.

		for(i = 0; i < cantidadConversiones_; i++)
		{
				borrador = _IO_PORTS_W(M6812_ADR00H + DirAD_ + 2 * i);
				///coloco los bits hacia la derecha, para que sea más legible al humano. Doy por hecho que la justificación viene a izquierda.
				if (Resolucion_) /// si son 10 bits
						borrador = borrador >> 6;
				else
						borrador = borrador >> 8;

				valoresRegistros_[i] = borrador;

				if (valoresAnterioresRegistros_[i] != borrador)
						valoresCambiados_ = 1;
		}

 		///Se entiende que al leer el primer registor de datos, se restablece el bit de secuencia terminada ATDxSTAT0-SCF tal y como
		///se configura por defecto ponieno a 1 el bit ATDxCTL2 - AFFC
}

/**
 * @brief	Función para activar o desactivar el modo SCAN
 * @param 	valor valor que se define por defecto, en caso de que el usuario no quiera modificar nada
 * @param	interactivo Está a 1 si el usuario desea interactuar
 */
void ad_scanMode(uint8_t valor, uint8_t interactivo)
{
		char c;
		if (interactivo)
		{
				serial_print("\nActivar modo SCAN Presione: (S/N");
				while((c = serial_recv()) != 'S' && c != 'N');
				serial_send(c); /* a modo de confirmación*/
				scanMode_ = (c == 'S')?1:0;
		}
		else
				scanMode_ = valor;
}

/**
 * @brief Función para establecer el número de conversiones a 1, 4 u 8 por secuencia.
 * @param       valor valor que se define por defecto, en caso de que el usuario no quiera modificar nada
 * @param       interactivo Está a 1 si el usuario desea interactuar
*/
void ad_cantidadConversiones(uint8_t valor, uint8_t interactivo)
{
		char c;
		if (interactivo)
		{
				serial_print("\nCantidad de conversiones 1, 4, 8 Presione: (1 = una, 4 = cuatro y 8 = ocho):");
				while((c = serial_recv()) != '1' && c != '4' && c != '8');
				serial_send(c); /* a modo de confirmación*/
				cantidadConversiones_ = c - '0';
		}
		else
				cantidadConversiones_ = valor;

}

/**
 * @brief	Función que detiene el funcionamiento del módulo analógico digital
 */
void ad_stop()
{
		/** Se apaga el  módulo correspondiente, aún así, no se detiene la secuencia actual*/
		_io_ports[M6812_ATD0CTL2 + DirAD_] = 0;

		_io_ports[M6812_ATD0CTL0 + DirAD_] = 0; ///No contiene información, pero la escritura de este registro, aborta la secuencia actual.
}

/**
 * @brief	Función para completar los parámetros y dar por iniciada la conversión.
 *		En ella se hace uso de los atributos globales para usarlos como máscara
 *		para completar los registros correctamente.
 */
void ad_start()
{

		/** Se enciende el módulo correspondiente y se justifican los datos a la izquierda  */
		_io_ports[M6812_ATD0CTL2 + DirAD_] = M6812B_ADPU;

		/**También se establece  ATDxCTL2 - AFFC a 1 para que baste con acceder al registro de datos para restablecer el indicador de secuencia terminada */
		_io_ports[M6812_ATD0CTL2 + DirAD_] |=  M6812B_AFFC;

		/** Se mira el tipo de resolución */
		if (Resolucion_)
				_io_ports[M6812_ATD0CTL4 + DirAD_] |= M6812B_RES10 ;
    else
				_io_ports[M6812_ATD0CTL4 + DirAD_] &= ~M6812B_RES10;

		/** Se añade el pin que se va a utilizar */
	 	_io_ports[M6812_ATD0CTL5 + DirAD_] |=  Pin_;

		/** Dependiendo del valor seleccionado, se activara unos bits u otros para el valor de ciclos de muestreo (SMP0 Y SMP1)
		Ciclos de 2, 4, 8 o 16. */
		switch (ciclosMuestreo_)
		{
				case 0:  ///2 ciclos
						_io_ports[M6812_ATD0CTL4 + DirAD_] &= (~M6812B_SMP0 & ~M6812B_SMP1);
						break;
	     	case 1:  ///4 ciclos
	     			_io_ports[M6812_ATD0CTL4 + DirAD_] &=  (M6812B_SMP0 | ~M6812B_SMP1);
						break;
				case 2: ///8 ciclos
						_io_ports[M6812_ATD0CTL4 + DirAD_] &=  (~M6812B_SMP0 | M6812B_SMP1);
						break;
				case 3: ///16 ciclos
						_io_ports[M6812_ATD0CTL4 + DirAD_] |=  (M6812B_SMP0 | M6812B_SMP1);
						break;
				default:
						break;
		}

		/**Activo número de conversiones especificado*/
		switch (cantidadConversiones_)
		{
				case 1:
						_io_ports[M6812_ATD0CTL5 + DirAD_] &=  ~M6812B_S8C; ///SC8 = 0 y SC1=1 para 1 conversión
						_io_ports[M6812_ATD0CTL3 + DirAD_] |=   M6812B_S1C;
						break;
			case 4:
						_io_ports[M6812_ATD0CTL5 + DirAD_] &=  ~M6812B_S8C; ///SC8 = 0 y SC1=0 para 4 conversiones
						_io_ports[M6812_ATD0CTL3 + DirAD_] &=  ~ M6812B_S1C;
						break;
			case 8:
						_io_ports[M6812_ATD0CTL5 + DirAD_] |=  M6812B_S8C; ///SC8 = 1 y SC1=x para 8 conversiones
						break;
			default:
						break;
		}

		/** Activo o desactivo las interrupciones según valor */
		if (interrupMode_) ///si interrupciones habilitadas
		{
				///permitimos las interrupciones
				unlock();
				_io_ports[M6812_ATD0CTL2 + DirAD_] |= M6812B_ASCIE;
		}
    else
		{
		  	/// Deshabilitamos interrupciones
				lock ();
				_io_ports[M6812_ATD0CTL2 + DirAD_] &= ~M6812B_ASCIE;
		}

		/** Se desactiva FIFO */
		_io_ports[M6812_ATD0CTL3 + DirAD_] &= ~M6812B_FIFO;

		/**Este es el último registro que modifico, ya que el ATDxCTL5, al escribir en él, se aborta la secuencia de conversión actual y comienza una nueva. */
		/** Modo scan activado o desactivado */
		if (scanMode_)
				_io_ports[M6812_ATD0CTL5 + DirAD_] |= M6812B_SCAN;
	  else
				_io_ports[M6812_ATD0CTL5 + DirAD_] &= ~M6812B_SCAN;
}

/**
 * @brief Funcion para retornar la posicion del array de valores de los registros
 */
uint16_t * ad_returnValores(void)
{
		return  &valoresRegistros_[0];
}

/**
 * @brief Funcion para retornar la posicion del array de valores cambiados
 */
uint8_t * ad_returnValoresCambiados(void)
{
		return  &valoresCambiados_;
}
