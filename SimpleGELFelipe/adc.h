/**
 * Funcion para seleccionar muestra de 8 o 10 bits
 *
 */
void ad_ocho_o_diez_bits(bool ocho)
{
	if (ocho)
	{
		//resolucion de 8bits, se deja el bit numero 7 a cero
		//con 16 ciclos de muestreo
		_io_ports[M6812_ATD0CTL4] = M6812B_SMP1 | M6812B_SMP0;
	}
	else 
	{
		//resolucion de 10bits, se pone el bit numero 7 a uno
		//con 16 ciclos de muestreo
		_io_ports[M6812_ATD0CTL4] = M6812B_RES10 | M6812B_SMP1 | M6812B_SMP0;
	}
}

void ad_tiempo_muestreo(uint16_t valor)
{
}

/**
 * Funcion para seleccionar el pin inicial
 *
 */
void ad_pin_inicio(uint16_t pin)
{
	//si escojo un pin, como se lo indico a la variable 
	//_io_ports
	//por que el profe desactiva el fifo ? algo que ver con esto?
	//bien??
	_io_ports[M6812_ATD0CTL5] = pin | M6812B_SCAN | M6812B_S8C; 
}

/**
 * Funcion para activar el módulo AD
 *
 */
void ad_iniciar()
{
	//NO SE SI ESTÁ BIEN
	//se activa el bit numero 7 y activa el módulo 0,
	//además, justifica a derecha activando el bit 4, es decir
	//del 9 al 0 para 10 bits 
	//y del 7 al 0 para 8 bits
	_io_ports[M6812_ATD0CTL2] = M6812B_ADPU | M6812B_DJM;
}

void ad_esperar_terminar()
{
}

void ad_devolver_valore_leidos()
{
}

	
