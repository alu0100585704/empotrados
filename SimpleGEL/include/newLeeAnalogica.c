
/* ****************************************
 * Lee una entrada analógica y muestra valor por
 * la serial.
 * Al inicio permite elegir en tiempo de ejecución el puerto
 * y el pin dentro del puerto a utilizar
 * No utiliza interrupciones sino polling

  Copyright (C) Alberto F. Hamilton Castro
  Dpto. de Ingeniería Informática de Sistemas
  Universidad de La Laguna

  Licencia: GPLv3

  *************************************** */

#include <types.h>
#include <sys/param.h>
#include <sys/interrupts.h>
#include <sys/sio.h>
#include <sys/locks.h>

//presentar proyecto ad.c ad.h test_ad_xxxx.c  makefile  ... 

/**
* La rutina de usuario, se ejecutará tanto en el modo initerrupción como en el modo polling. Justo después de leerse los registros de datos correspondientes,
* se llamará a la rutina de usuario, si así se definió.
* Prácticamente todas las funciones de la librerían disponen de un modo interactivo, para solicitar al usuario el dato necesario para 
* configuración de funcionalidad en concreto. Por supuesto, todas se pueden configurar de forma no interactiva.
* Los resultados de las lecturas de los registros de datos, se mantendrán actualizadas en un array, ya sea mediante polling o por la rutina de interrupción
* en caso de así haberlo decidido el programador.
* Por defecto los siguientes bits se configuraran de la siguiente manera:
*
* ATDxCTL2 / AFFC = 1 (Basta con acceder a registro de datos para restablecer bit ATDxSTAT0-SCF que indica secuencia terminada
* ATDxCTL2 / DJM = 0 (Justificación a izquierda)
* ATDxCTL3 / FIFO = 0 (Modo FIFO desactivado)
*
* Inicialmente, el FIFO estará siempre deshabilitado, y la justificación se trabajará siempre a izquierda. Todo lo demás si se podrá parametrizar.
*/


/**
 * Variables globales. Las funciones configuran estas variables, y algunas retorna los valores necesarios, aún así, se podrín configurar directamente ya que son variables globales
 */

uint16_t DirAD_; 		/** Desplazamiento de la dirección para escoger módulo 0 o 1 */
uint8_t Pin_; 			/** Pin dentro del puerto del que se va a leer */
uint8_t Resolucion_; 		/** Resolución de los datos (8 o 10 bits) */
uint8_t ciclosMuestreo_; 	/** Ciclos de muestreo de 2, 4, 8 o 16*/
uint8_t scanMode_; ///a uno si se activa el modo scan. 0 si no lo está
uint8_t cantidadConversiones_; /// cantidad de conversiones 1,4,8
uint8_t interrupMode_; ///a 1 si se activa la interrupción o cero si funcioamiento modo polling
uint8_t valoresCambiados_; ///se pondrá a 1 si una secuencia contiene algún valor que difiere de la secuencia anterior, o sea, compara valoresRegistros_ con valoresAnterioresRegistros_
uint16_t valoresRegistros_[8];  ///desde el cero al siete se almacenará el valor de los registris ADRxnH(bits 15-8) y ADRxnL (bits 7-0)
uint16_t valoresAnterioresRegistros_[8]; ///para comprobar si hay diferencias.

void (* punteroUserFunction_)(uint16_t *) = NULL; ///Puntero hacia una función
///
/********************************************************************************************************************************
 *	Prototipo de funciones
 *********************************************************************************************************************************/
///

/**
 * @brief	
 * @param	ptr 		El parámetro será la dirección de la función del usuario. Recuerde que debe de tener un parámetro en el que recibirá el puntero al array con los valores de los registros.
 *						Una posible función prototipo sería:  "void userFunction(uint16_t * registros)" donde registros contendrá de 0 a 7 valores, dependiendo del tamaño de la secuencia seleccionada
 * @param	interactivo	está a 1 si el usuario quiere interactuar
 */
void ad_setUserFunction(void (*ptr)(uint16_t *));
/**
 * @brief	Función que activa o desactiva el tratamiento de los registros de datos mediante interrupción.
 * @param	valor		si 1, se activan las interrupciones, si 0, se desactivan 
 */
void ad_activarInterrupMode(uint8_t valor);

/** Manejador interrupciones del atd (conversor analógica digital  
*  Este es el manejador principal, el cual leerá los registros de datos y llamará a la función definida por el usuario en caso de haberse definido.
*/
void __attribute__((interrupt))vi_atd(void);  

/**
 * @brief	Función para esperar a que se termine la conversión, modo Polling. Además, se llamará a la función de usuario en caso de haber sido asignada.
 */
uint16_t * ad_wait_for_data(); ///el array devuelto el la variable valoresRegistros_[8];
/**
 * @brief	Función para seleccionar el valor de los ciclos de muestreo (2,4,8,16)
 * @param	valor		valor que se define por defecto, en caso de que el usuario no desee modificar nada
 * @param	interactivo	está a 1 si el usuario quiere interactuar
 */
void ad_ciclos_muestreo (uint8_t valor, uint8_t interactivo);

/**
 * @brief	Función para seleccionar el valor de la conversión (8 o 10 bits)
 * @param	valor		valor que se define por defecto, en caso de que el usuario no desee modificar nada
 * @param	interactivo	está a 1 si el usuario quiere interactuar
 */
void ad_ocho_o_diez_bits (uint8_t valor, uint8_t interactivo);
/**
 * @brief	Función para seleccionar el tiempo de muestreo
 * @param	valor		valor que definirá el usuario para el modo
 * @param	interactivo	está a 1 si el usuario quiere interactuar
 */
void ad_tiempo_muestreo (uint16_t valor);
/**
 * @brief	Función para seleccionar el pin de inicio en el microcontrolador
 * @param	valor		valor que se define por defecto, en caso de que el usuario no desee modificar nada 
 * @param	interactivo	está a 1 si el usuario quiere interactuar
 */
void ad_pin_inicio (uint8_t valor, uint8_t interactivo);
/**
 * @brief	Función para seleccionar el módulo correspondiente al conversor A/D
 * @param	valor		valor que se define por defecto, en caso de que el usuario no desee modificar nada 
 * @param	interactivo	está a 1 si el usuario quiere interactuar
 */
void ad_modulo (uint8_t valor, uint8_t interactivo);
/**
 * @brief	Función que muestra los valores de los registros dependiendo de la configuración, Se puede decidir mostrar solo si se produjeron cambios con respecto
 * 			a la secuencia anterior. Si soloCambios = 1, se mostrarán solo los cambios.
 *		
 */
void mostrarValores(uint8_t soloCambios);

/**
 * @brief	Función interna de la librería que se encarga de leer los valores de los registros, dependiendo de la cantidad de conversiones seleccionadas.
 *			Se asume que la justificación siempre es a izquierda
 */
void ad_leer_valores(void);
/**
 * @brief	Función para activar o desactivar el modo SCAN
 *
 */
void ad_scanMode(uint8_t valor, uint8_t interactivo);

/**
*	@brief Función para establecer el número de conversiones a 1, 4 u 8 por secuencia.
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









/**********************************************************************************************************************************************************
**********************************************************************************************************************************************************
*****************************************************          IMPLEMENTACIÓN CÓDIGO   				******************************************************
***********************************************************************************************************************************************************/







/**
 * @brief	
 * @param	ptr 		El parámetro será la dirección de la función del usuario. Recuerde que debe de tener un parámetro en el que recibirá el puntero al array con los valores de los registros.
 *						Una posible función prototipo sería:  "void userFunction(uint16_t * registros)" donde registros contendrá de 0 a 7 valores, dependiendo del tamaño de la secuencia seleccionada
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


/** Manejador interrupciones del atd (conversor analógica digital  
*  Este es el manejador principal, el cual leerá los registros de datos y llamará a la función definida por el usuario en caso de haberse definido.
*/
void __attribute__((interrupt))vi_atd(void) {

///ahora se leen los registros según la configuración. Además esta lectura, restablecerá el bit de secuencia terminada al tener ATDxCTL2 / AFFC a 1.
 ad_leer_valores();
  
  if (punteroUserFunction_!= NULL) ///si se estableció una rutina personalizada de usuario, se la llama.
		(*punteroUserFunction_)(&valoresRegistros_);
		

}

/**
 * @brief	Función para esperar a que se termine la conversión, modo Polling. Además, se llamará a la función de usuario en caso de haber sido asignada.
 */
uint16_t * ad_wait_for_data()
{
	/* Esperamos a que se termine la conversión modo polling*/
	while (! (_io_ports[M6812_ATD0STAT0 + DirAD_] & M6812B_SCF));
	
	///ahora se leen los registros según la configuración. Además esta lectura, restablecerá el bit de secuencia terminada al tener ATDxCTL2 / AFFC a 1.
	ad_leer_valores();
  
	
	 if (punteroUserFunction_!= NULL) ///si se estableció una rutina personalizada de usuario, se la llama.
		(*punteroUserFunction_)(&valoresRegistros_);
		
	return  &valoresRegistros_;
}


/**
 * @brief	Función para seleccionar el valor de los ciclos de muestreo (2,4,8,16)
 * @param	valor		valor que se define por defecto, en caso de que el usuario no desee modificar nada
 * @param	interactivo	está a 1 si el usuario quiere interactuar
 */
void ad_ciclos_muestreo (uint8_t valor, uint8_t interactivo)
{
	char c;
	if (interactivo)
	{ 
		serial_print("\nCiclos de Muestreo de 2, 4, 8 o 16 Presione: (0 = 2, 1 = 4, 2 = 8 y 3 = 16):");
     		while((c = serial_recv()) < '0' && c > '3');
    		serial_send(c); /* a modo de confirmación*/     
     		ciclosMuestreo_ = c - '0';    
	}
	else
		ciclosMuestreo_ = valor;	
}

/**
 * @brief	Función para seleccionar el valor de la conversión (8 o 10 bits)
 * @param	valor		valor que se define por defecto, en caso de que el usuario no desee modificar nada
 * @param	interactivo	está a 1 si el usuario quiere interactuar
 */
void ad_ocho_o_diez_bits (uint8_t valor, uint8_t interactivo)
{
	char c;
	if (interactivo)
	{ 
		serial_print("\nResolución de 8 o 10 Bits Presione: (0 para 8 bits o 1 para 10 bits):");
     		while((c = serial_recv()) < '0' && c > '1');
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
 * @param	valor		valor que se define por defecto, en caso de que el usuario no desee modificar nada 
 * @param	interactivo	está a 1 si el usuario quiere interactuar
 */
void ad_pin_inicio (uint8_t valor, uint8_t interactivo)
{
	char c;
	if (interactivo)
	{
		serial_print("\nPin del puerto a utilizar Presione: (0 - 7)?:");
     		while((c = serial_recv()) < '0' && c > '7');
    		serial_send(c); /** a modo de confirmación */
    		Pin_ = c - '0'; 
	}
	else
		Pin_ = valor;	
}

/**
 * @brief	Función para seleccionar el módulo correspondiente al conversor A/D
 * @param	valor		valor que se define por defecto, en caso de que el usuario no desee modificar nada 
 * @param	interactivo	está a 1 si el usuario quiere interactuar
 */
void ad_modulo (uint8_t valor, uint8_t interactivo)
{
	char c;
	if (interactivo)
	{
		serial_print("\nMódulo conversor a utilizar Presione (0 = Módulo 0, 1 = Módulo 1)");
		while((c = serial_recv()) != '0' && c != '1');
		serial_send(c); /* a modo de confirmación*/
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
 * @brief	Función que muestra los valores de los registros dependiendo de la configuración, Se puede decidir mostrar solo si se produjeron cambios con respecto
 * 			a la secuencia anterior. Si soloCambios = 1, se mostrarán solo los cambios.
 *		
 */
void mostrarValores(uint8_t soloCambios)
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
 * @brief	Función interna de la librería que se encarga de leer los valores de los registros, dependiendo de la cantidad de conversiones seleccionadas.
 *			Se asume que la justificación siempre es a izquierda
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
 *
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
*	@brief Función para establecer el número de conversiones a 1, 4 u 8 por secuencia.
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
	
	///Primero de todo encendemos Módulo.
	/** Se enciende el módulo correspondiente y se justifican los datos a la izquierda  */
				
	_io_ports[M6812_ATD0CTL2 + DirAD_] = M6812B_ADPU;
	
	///También se establece  ATDxCTL2 - AFFC a 1 para que baste con acceder al registro de datos para restablecer el indicador de secuencia terminada
	_io_ports[M6812_ATD0CTL2 + DirAD_] |=  M6812B_AFFC;
	  	  
    
	/** Se mira el tipo de resolución */
	if (Resolucion_)			
		_io_ports[M6812_ATD0CTL4 + DirAD_] |= M6812B_RES10 ;
    	else
		_io_ports[M6812_ATD0CTL4 + DirAD_] &= ~M6812B_RES10;	

	/** Se añade el pin que se va a utilizar */
	 _io_ports[M6812_ATD0CTL5 + DirAD_] |=  Pin_;
	
	/** Dependiendo del valor seleccionado, se activara unos bits u otros para el valor de ciclos de muestreo (SMP0 Y SMP1) 
		Ciclos de 2, 4, 8 o 16.
	*/
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


/*Activo número de conversiones especificado*/

		switch (cantidadConversiones_)
		{
			case 1:
				_io_ports[M6812_ATD0CTL5 + DirAD_] &=  ~M6812B_S8C; ///SC8 = 0 y SC1=1 para 1 conversión
				_io_ports[M6812_ATD0CTL3 + DirAD_] |=  | M6812B_S1C;
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

///Activo o desactivo las interrupciones según valor		
	if (interrupMode_) ///si interrupciones habilitadas
		{
			 //permitimos las interrupciones
			unlock();
			_io_ports[M6812_ATD0CTL2 + DirAD_] |= M6812B_ASCIE;
		}
    else
		{	
		  /* Deshabilitamos interrupciones */
				lock ();
			_io_ports[M6812_ATD0CTL2 + DirAD_] &= ~M6812B_ASCIE;
		}
			
	/** Se desactiva FIFO */
	_io_ports[M6812_ATD0CTL3 + DirAD_] &= ~M6812B_FIFO;
	
	
	
			///Este es el último registro que modifico, ya que el ATDxCTL5, al escribir en él, se aborta la secuencia de conversión actual y comienza una nueva.

			/* Modo scan activado o desactivado*/	
		
		if (scanMode_)		
			_io_ports[M6812_ATD0CTL5 + DirAD_] |= M6812B_SCAN;
		
	     else 		
			_io_ports[M6812_ATD0CTL5 + DirAD_] &= ~M6812B_SCAN;
		 

}


/**
 * Rutina establecida por el usuario, que se ejecutará después de la lectura de valores tanto en modo polling como por interrupción.
 */
void rutinaUsuario(uint16_t * valores)
{
	serial_print("\nRutina usuario ejecutada");
	if (valoresCambiados_)
		serial_print("\nCambiaron los valores diciendolo desde rutina");
}

int main () {
	
	char c;
	uint16_t DirAd;
	uint8_t Pin;
	uint16_t * valores;
	
    	ad_pin_inicio(0,1); 
    
		ad_modulo(0,1);
	
		ad_ocho_o_diez_bits(0,1); 
   
    	ad_ciclos_muestreo(0,1);
		
		ad_scanMode(0,1);
		
		ad_cantidadConversiones(0,1);
		
		///ad_tiempo_muestreo(); Pendiente de revisar
		
		///Hasta aquí se han recogido todos los datos en modo interactivo, para poder hacer las pruebas más fácilmente.
		
    	ad_start(); ///se inicia el proceso
    
/*******************************************************************************************
 * Versión Polling sin rutina de usuario.
 ******************************************************************************************/
		ad_activarInterrupMode(0); ///Me aseguro de desactivar las interrupciones
	
	while (1) 
	{	
		valores = ad_wait_for_data(); ///espero a una lectura total de la secuencia. También se ejecutaría la rutina de usuario en caso de definirla.
		///ahora podría leer los valores mediante el puntero valores o accediendo directamente  al array  global valoresRegistros_[8];
		mostrarValores(1); ///muestro valores solo si han cambiado desde el anterior.
	}

/*******************************************************************************************
 * Versión Polling con rutina de usuario.
 ******************************************************************************************/
		ad_activarInterrupMode(0); ///Me aseguro de desactivar las interrupciones
		ad_setUserFunction(&rutinaUsuario); ///configuro una rutina de usuario.
		
	while (1) 
	{	
		valores = ad_wait_for_data(); ///espero a una lectura total de la secuencia. También se ejecutaría la rutina de usuario en caso de definirla.
		///ahora podría leer los valores mediante el puntero valores o accediendo directamente  al array  global valoresRegistros_[8];
		mostrarValores(1); ///muestro valores solo si han cambiado desde el anterior.
	}

/*******************************************************************************************
 * Versión Interrupcion sin rutina de usuario.
 ******************************************************************************************/
		ad_activarInterrupMode(1); ///Me aseguro de desactivar las interrupciones
		ad_setUserFunction(NULL); ///configuro una rutina de usuario.
		
		  while(1)
		  {
		  	if (valoresCambiados_)
				serial_print("\nCambiaron los valores diciendolo fuera de la rutina");
		  }


/*******************************************************************************************
 * Versión Interrupcion con rutina de usuario.
 ******************************************************************************************/
		ad_activarInterrupMode(1); ///Me aseguro de desactivar las interrupciones
		ad_setUserFunction(&rutinaUsuario); ///configuro una rutina de usuario.
		
		  while(1)		  
			__asm__ __volatile__("wai");


	
}

///int main () {
  //char c;
  ///*Diferencial a usar en los direccionamientos para distinguir puerto 0 y 1 */
  //uint16_t DirAD;
  //uint8_t Pin; /*Pin dentro del puerto del que se va a leer */

  //uint16_t resultadoAnterior = 0;M6812_ATD0CTL0

  ///* Deshabilitamos interrupciones */
  //lock ();

  ///*Encendemos led*/
  //_io_ports[M6812_DDRG] |= M6812B_PG7;
  //_io_ports[M6812_PORTG] |= M6812B_PG7;


  //serial_init();
  //serial_print("\nLeeAnalogica.c ===========\n");

  //while(1) {
    //// Quitamos posible pulsación pendiente
    //if (serial_receive_pending()) serial_recv();
    ///* Elección del puerto */
    //serial_print("\nPuerto conversor a utilizar (0 o 1)!);
    //while((c = serial_recv()) != '0' && c != '1');
    //serial_send(c); /* a modo de confirmación*/
    //DirAD = (c == '0')?0:(M6812_ATD1CTL0 - M6812_ATD0CTL0);

    //serial_print("\nUsando desplazamiento ");
    //serial_printdecword(DirAD);

    ///* Elección del pin dentro del puerto */
    //serial_print("\nPin del puerto a utilizar (0 - 7)?:");
    //while((c = serial_recv()) < '0' && c > '7');
    //serial_send(c); /* a modo de confirmación*/
    //Pin = c - '0';

    ///*Pasamos a configurar AD correspondiente*/
    //_io_ports[M6812_ATD0CTL2 + DirAD] = M6812B_ADPU; /*Encendemos, justf. izda*/
    //_io_ports[M6812_ATD0CTL3 + DirAD] = 0; /*Sin fifo*/

    ///* resolución de 10 bits y 16 ciclos de muestreo */
    //_io_ports[M6812_ATD0CTL4 + DirAD] = M6812B_RES10 | M6812B_SMP0 | M6812B_SMP1;

    ///* Modo scan con 8 resultados sobre el pin seleccionado */
    //_io_ports[M68 //serial_print("\nConvirtiendo (pulsa para salir)\n");

    //char simbolo[] = "/|\\-*";
    //uint16_t itera = 0;
//#define ITERA_CAMBIO (5000)
    //while(!serial_receive_pending()) {

      //itera++;
      //if (!(itera % ITERA_CAMBIO)) {
        //uint8_t simAct = itera/ITERA_CAMBIO;
        //if(!simbolo[simAct]) {
          //itera = 0;
          //simAct = 0;
        //}
        //serial_send(simbolo[simAct]);
        //serial_send('\b');
      //}

      ///* Esperamos a que se termine la conversión */
      //while(! (_io_ports[M6812_ATD0STAT0 + DirAD] & M6812B_SCF));

      ///*Invertimos el led*/
      //_io_ports[M6812_PORTG] ^= M6812B_PG7;

      ///*Vemos si los 8 resultados son iguales */
      //uint16_t resultado = _IO_PORTS_W(M6812_ADR00H + DirAD);
      //uint8_t iguales = 1;
      //for(uint8_t i = 0; iguales && i < 8; i++)
        //iguales = resultado == _IO_PORTS_W(M6812_ADR00H + DirAD + 2 * i);
      //if(! iguales)
        //continue;
      //if (resultado == resultadoAnterior)
        //continue;

      ///* Los 8 resultados son iguales y distintos a lo que teníamos antes*/
      //serial_print("Nuevo valor = ");
      //serial_printdecword(resultado);
      //serial_send('\n');
      //resultadoAnterior = resultado;
    //}
//}

