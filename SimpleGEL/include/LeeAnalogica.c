
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

uint16_t DirAD_; 		/** Desplazamiento de la dirección para escoger módulo 0 o 1 */
uint8_t Pin_; 			/** Pin dentro del puerto del que se va a leer */
uint8_t Resolucion_; 		/** Resolución de los datos (8 o 10 bits) */
uint8_t ciclosMuestreo_; 	/** Ciclos de muestreo */

/**
 * @brief	Función para seleccionar el valor de los ciclos de muestreo (2,4,8,16)
 * @param	valor		valor que se define por defecto, en caso de que el usuario no desee modificar nada
 * @param	interactivo	está a 1 si el usuario quiere interactuar
 */
ad_ciclos_muestreo (uint8_t valor, uint8_t interactivo)
{
	char c;
	if (interactivo)
	{ 
		serial_print("\nCiclos de Muestreo de 2, 4, 8 o 16 (0 = 2, 1 = 4, 2 = 8 y 3 = 16):");
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
ad_ocho_o_diez_bits (uint8_t valor, uint8_t interactivo)
{
	char c;
	if (interactivo)
	{ 
		serial_print("\nResolución de 8 o 10 Bits (0 para 8 bits o 1 para 10 bits):");
     		while((c = serial_recv()) < '0' && c > '1');
    		serial_send(c); /* a modo de confirmación*/
     		Resolucion_ = c - '0';    
	}
	else
		Resolucion_ = valor;	
}

/**
 * @brief	Función para seleccionar el tiempo de muestreo
 * @param	valor		valor que definirá el usuario para el modo
 * @param	interactivo	está a 1 si el usuario quiere interactuar
 */
void ad_tiempo_muestreo (uint16_t valor)
{
	/*
	Cada conversion dura (en ciclos del submodulo):
		2 ciclos de captura inicial
		4 ciclos de copia del potencial
		2, 4, 8  ́o 16 ciclos de muestreo (configurables)
		10/12 ciclos de conversion de pendiendo de la resolucion pedida (8/10 bits).
	*/
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
		serial_print("\nPin del puerto a utilizar (0 - 7)?:");
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
		serial_print("\nMódulo conversor a utilizar");
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
 * @brief	Función para completar los parámetros y dar por iniciada la conversión.
 *		En ella se hace uso de los atributos globales para usarlos como máscara
 *		para completar los registros correctamente.
 */
void ad_iniciar ()
{
	/** Se mira el tipo de resolución */
	if (Resolucion_)			
		_io_ports[M6812_ATD0CTL4 + DirAD_] |= M6812B_RES10 ;
    	else
		_io_ports[M6812_ATD0CTL4 + DirAD_] &= ~M6812B_RES10;	

	/** Se añade el pin que se va a utilizar */
	 _io_ports[M6812_ATD0CTL5 + DirAD_] |=  Pin_;
	
	/** Dependiendo del valor seleccionado, se activara unos bits u otros para el valor de ciclos de muestreo (SMP0 Y SMP1) */
	switch (ciclosMuestreo_)
	{
		case 0: 
			_io_ports[M6812_ATD0CTL4 + DirAD_] &= (~M6812B_SMP0 & ~M6812B_SMP1);
			break;
	     	case 1:
	     		_io_ports[M6812_ATD0CTL4 + DirAD_] &=  (M6812B_SMP0 | ~M6812B_SMP1); 					
			break;
		case 2:
			_io_ports[M6812_ATD0CTL4 + DirAD_] &=  (~M6812B_SMP0 | M6812B_SMP1); 
			break;
		case 3:
			_io_ports[M6812_ATD0CTL4 + DirAD_] |=  (M6812B_SMP0 | M6812B_SMP1); 
			break;
		default:
			break;
	}

	/** Se enciende el módulo correspondiente y se justifican los datos a la izquierda */
	_io_ports[M6812_ATD0CTL2 + DirAD_] = M6812B_ADPU;
	  	  
	/** Se desactiva FIFO */
	_io_ports[M6812_ATD0CTL3 + DirAD_] &= ~M6812B_FIFO;
  
}

/**
 * @brief	Función para esperar a que se termine la conversión 
 */
void ad_esperar_terminar()
{
	/* Esperamos a que se termine la conversión */
	while (! (_io_ports[M6812_ATD0STAT0 + DirAD_] & M6812B_SCF));
}

/**
 * @brief	Función para obtener los valores resultantes de la conversión y mostrarlos al usuario
 */
void ad_devolver_valores_leidos ()
{
	//CONVERSIÓN
	serial_print("\nConvirtiendo (pulsa para salir)\n");

    	char simbolo[] = "/|\\-*";
    	uint16_t itera = 0;
	#define ITERA_CAMBIO (5000)
    	while(!serial_receive_pending()) {
		itera++;
      		if (!(itera % ITERA_CAMBIO)) {
			uint8_t simAct = itera/ITERA_CAMBIO;
			if(!simbolo[simAct]) {
				itera = 0;
				simAct = 0;
			}
			serial_send(simbolo[simAct]);
			serial_send('\b');
		}

		/**
		MIRAR
		MIRAR
		MIRAR
		MIRAR
		MIRAR
		*/
		/** Esperamos a que termine la conversión */
		ad_esperar_terminar();

		/*Invertimos el led*/
		_io_ports[M6812_PORTG] ^= M6812B_PG7;

		/*Vemos si los 8 resultados son iguales */
		uint16_t resultado = _IO_PORTS_W(M6812_ADR00H + DirAD_);
		uint8_t iguales = 1;
		for(uint8_t i = 0; iguales && i < 8; i++)
			iguales = resultado == _IO_PORTS_W(M6812_ADR00H + DirAD_ + 2 * i);
		if(! iguales)
			continue;
		if (resultado == resultadoAnterior)
			continue;

		/* Los 8 resultados son iguales y distintos a lo que teníamos antes*/
		serial_print("Nuevo valor = ");
		serial_printdecword(resultado);
		serial_send('\n');
		resultadoAnterior = resultado;
	}
}

int main () {
	
	char c;
	uint16_t DirAd;
	uint8_t Pin;
	
    	ad_pin_inicio(0,1); 
    
	ad_modulo(0,1);
	
	ad_ocho_o_diez_bits(0,1); ///resio_ports[M6812_ATD0CTL5 + DirAD] = M6812B_SCAN | M6812B_S8C | Pin;olución solicitada
   
    	ad_ciclos_muestreo(0,1);
    
    	ad_iniciar();
    
	ad_devolver_valores_leidos();

	}
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
