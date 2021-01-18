#include "timer.h"

#define NULL 0
volatile uint32_t overflow = 0;
void (*call_once)(void) = 0;
volatile uint32_t call_once_overflows = 0;
void (*call_period)(void) = 0;
volatile uint32_t call_period_overflows = 0;
volatile uint32_t call_period_max_overflows = 0;
volatile uint16_t period_num_ciclos = 0;

volatile uint32_t period = 0;


void timer_init(uint8_t prescale) {
	_io_ports[M6812_TMSK2] &= ~7;
	_io_ports[M6812_TMSK2] |= (prescale & 7);
	_io_ports[M6812_TMSK2] |= M6812B_TOI;	//Habilitamos la interrupcion
	// Habilitamos el temporizador
	_io_ports[M6812_TSCR] |= M6812B_TEN;
}

void timer_schedule_call(uint32_t useg, void (*f)(void))
{
	call_once = f;

	/* Vemos velocidad del temporizador*/
	uint8_t factorT = _io_ports[M6812_TMSK2] & 0x07;	/*Factor de escalado actual*/
	uint32_t frec = M6812_CPU_E_CLOCK / (1 << factorT); /* Frecuencia del temporizador*/

	uint8_t numDivisiones = 6;
	uint32_t frecD = frec;
	while ((numDivisiones > 0) && !(frecD % 10))
	{
		frecD /= 10;
		numDivisiones--;
	}
	uint32_t usegD = useg;
	uint32_t cota = 0xFFFFFFFF / frecD;
	while ((numDivisiones > 0) && (usegD > cota))
	{
		usegD /= 10;
		numDivisiones--;
	}
	uint32_t numCiclosL = frecD * usegD;

	while (numDivisiones > 0)
	{
		numCiclosL /= 10;
		numDivisiones--;
	}

	call_once_overflows = numCiclosL >> 16;
	uint16_t numCiclos = numCiclosL & 0xffff;

	/* Por si escalado muy grande y useg pequeño */
	if ((numCiclos == 0) && (call_once_overflows == 0))
		numCiclos = 1;

  //serial_print("\ncall_once_overflows: ");
  //serial_printdeclong(call_once_overflows);
  //serial_print("\nnumCiclosOnce: ");
  //serial_printdecword(numCiclos);

	/* Desconectamos para no afectar al pin */
	_io_ports[M6812_TCTL2] &= ~(M6812B_OM3 | M6812B_OL3);

	/*Preparamos disparo*/
	_IO_PORTS_W(M6812_TC3) = _IO_PORTS_W(M6812_TCNT) + numCiclos;
	_io_ports[M6812_TIOS] |= M6812B_IOS3; /*configuramos canal como comparador salida*/
	_io_ports[M6812_TFLG1] = M6812B_C3F;  /*Bajamos el banderín  */
	_io_ports[M6812_TMSK1] |= M6812B_IOS3;
}

void timer_repeat_call(uint32_t useg, void (*f)(void))
{
	call_period = f;
	period = USG_2_TICKS(useg);
	_IO_PORTS_W(M6812_TC2) = _IO_PORTS_W(M6812_TC2) + period;
	_io_ports[M6812_TCTL2] &= ~M6812B_OM2;
	_io_ports[M6812_TCTL2] |= M6812B_OL2;
	_io_ports[M6812_TIOS] |= M6812B_IOS2;
	_io_ports[M6812_TFLG1] = M6812B_IOS2;
	_io_ports[M6812_TMSK1] |= M6812B_IOS2;

	call_period = f;

	/* Vemos velocidad del temporizador*/
	uint8_t factorT = _io_ports[M6812_TMSK2] & 0x07;	/*Factor de escalado actual*/
	uint32_t frec = M6812_CPU_E_CLOCK / (1 << factorT); /* Frecuencia del temporizador*/

	uint8_t numDivisiones = 6;
	uint32_t frecD = frec;
	while ((numDivisiones > 0) && !(frecD % 10))
	{
		frecD /= 10;
		numDivisiones--;
	}
	uint32_t usegD = useg;
	uint32_t cota = 0xFFFFFFFF / frecD;
	while ((numDivisiones > 0) && (usegD > cota))
	{
		usegD /= 10;
		numDivisiones--;
	}
	uint32_t numCiclosL = frecD * usegD;

	while (numDivisiones > 0)
	{
		numCiclosL /= 10;
		numDivisiones--;
	}

	call_period_overflows = numCiclosL >> 16;
	call_period_max_overflows = call_period_overflows;
	uint16_t numCiclos = numCiclosL & 0xffff;

	/* Por si escalado muy grande y useg pequeño */
	if ((numCiclos == 0) && (call_period_overflows == 0))
		numCiclos = 1;

  //serial_print("\ncall_period_overflows: ");
  //serial_printdeclong(call_period_overflows);
  //serial_print("\ncall_period_max_overflows: ");
  //serial_printdeclong(call_period_max_overflows);
  //serial_print("\nperiod_num_ciclos: ");
  //serial_printdecword(period_num_ciclos);

	/* Desconectamos para no afectar al pin */
	_io_ports[M6812_TCTL2] &= ~(M6812B_OM2 | M6812B_OL2);

	/*Preparamos disparo*/
	_IO_PORTS_W(M6812_TC2) = _IO_PORTS_W(M6812_TCNT) + numCiclos;
	period_num_ciclos = numCiclos;
	_io_ports[M6812_TIOS] |= M6812B_IOS2; /*configuramos canal como comparador salida*/
	_io_ports[M6812_TFLG1] = M6812B_C2F;  /*Bajamos el banderín  */
	_io_ports[M6812_TMSK1] |= M6812B_IOS2;
}

uint32_t timer_get_cycle(void)
{
  uint32_t ciclos = (overflow << 16) | _IO_PORTS_W(M6812_TCNT);
  return ciclos;
}

uint32_t timer_get_micro(void)
{
	uint8_t factorT = _io_ports[M6812_TMSK2] & 0x07; // Prescalate factor 12
	uint8_t shift;
	uint32_t micros;

	if (factorT >= 3)
	{
		shift = factorT - 3;
		micros = timer_get_cycle() << shift;
	}
	else
	{
		shift = 3 - factorT;
		micros = timer_get_cycle() >> shift;
	}

	return micros;
}

uint32_t timer_get_mili(void)
{
	return timer_get_micro() / 1000;
}

void timer_sleep(uint32_t useg)
{
	uint32_t final = timer_get_micro() + useg;
	while (timer_get_micro() < final)	{}
}

void __attribute__((interrupt)) vi_tov(void)
{
	_io_ports[M6812_TFLG2] = M6812B_TOF; /*Bajamos el banderín de desbordamiento */
	overflow++;
}

void __attribute__((interrupt)) vi_ioc3(void)
{
	/*Bajamos el banderín de OC2 */
	_io_ports[M6812_TFLG1] = M6812B_IOS3;

	if (call_once_overflows > 0)
	{
		call_once_overflows--;
		return;
	}

	if (call_once != NULL)
	{
		(*call_once)();
		call_once = NULL;
		_io_ports[M6812_TIOS] &= ~M6812B_IOS3;
		_io_ports[M6812_TMSK1] &= ~M6812B_IOS3;
	}
}

void __attribute__((interrupt)) vi_ioc2(void)
{
	/*Bajamos el banderín de OC2 */
	_io_ports[M6812_TFLG1] = M6812B_IOS2;

	if (call_period_overflows > 0)
	{
		call_period_overflows--;
		return;
	}

	call_period_overflows = call_period_max_overflows;
	_IO_PORTS_W(M6812_TC2) = _IO_PORTS_W(M6812_TCNT) + period_num_ciclos;

	if (call_period != NULL)
	{
		(*call_period)();
	}
}
