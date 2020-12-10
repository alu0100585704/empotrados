#include "pwm.h"

void pwm_set_clock(bool Reloj_A,
				   int8_t factor_N,
				   bool grano_fino,
				   int8_t valor_contador){
    //int32_t clk = 25000000;
	int8_t factor_N_segurizado = factor_N;
	int8_t seguro = 7;
	factor_N_segurizado &= seguro;
	if (Reloj_A){
		_io_ports[M6812_PWCLK] &= ~(7 <<3);
		_io_ports[M6812_PWCLK] |= (factor_N_segurizado<<3);
		if(!grano_fino){
			_io_ports[M6812_PWCLK] &= ~M6812B_CON01;
			_io_ports[M6812_PWPOL] &= ~M6812B_PCLK0;
			_io_ports[M6812_PWPOL] &= ~M6812B_PCLK1;
		} else {
			_io_ports[M6812_PWCLK] &= ~M6812B_CON01;
			_io_ports[M6812_PWPOL] |= M6812B_PCLK0;
			_io_ports[M6812_PWPOL] |= M6812B_PCLK1;
			_io_ports[M6812_PWSCAL0] =valor_contador;
		}
	} else {
		_io_ports[M6812_PWCLK] &= ~(7);
		_io_ports[M6812_PWCLK] |= (factor_N_segurizado);
		if(!grano_fino){
			_io_ports[M6812_PWCLK] &= ~M6812B_CON23;
	    	_io_ports[M6812_PWPOL] &= ~M6812B_PCLK2;
			_io_ports[M6812_PWPOL] &= ~M6812B_PCLK3;
		} else {
			_io_ports[M6812_PWCLK] &= ~M6812B_CON23;
			_io_ports[M6812_PWPOL] |= M6812B_PCLK2;
			_io_ports[M6812_PWPOL] |= M6812B_PCLK3;
			_io_ports[M6812_PWSCAL1] =valor_contador;
		}
	}
}

void pwm_set_polarity(int8_t puerto_seleccionado, bool Alta)
{
	switch(puerto_seleccionado)
	{
		case 0:
			if ( Alta == true )
				_io_ports[M6812_PWPOL] |= M6812B_PPOL0; //Polaridad 0 / 1 trabaja a alta
			else
				_io_ports[M6812_PWPOL] &= ~M6812B_PPOL0;
			break;

		case 1:
			if ( Alta == true )
				_io_ports[M6812_PWPOL] |= M6812B_PPOL1;
			else
				_io_ports[M6812_PWPOL] &= ~M6812B_PPOL1;
			break;

		case 2:
			if ( Alta == true )
				_io_ports[M6812_PWPOL] |= M6812B_PPOL2;
			else
				_io_ports[M6812_PWPOL] &= ~M6812B_PPOL2;
			break;

		case 3:
			if ( Alta == true )
				_io_ports[M6812_PWPOL] |= M6812B_PPOL2;
			else
				_io_ports[M6812_PWPOL] &= ~M6812B_PPOL2;
			break;
	}
}



void pwm_aling(bool alineamiento_izq, int8_t canal_seleccionado)
{
	// Alineamiento izquierda

	// Contador PWCNTn
	if( alineamiento_izq == true )
	{
		switch (canal_seleccionado)
		{
			case 0:
				_io_ports[M6812_PWCNT0] |= M6812B_PCLK0;

				if ( _io_ports[M6812_PWCNT0] <= M6812_PWDTY0)
					serial_print("Nivel indicado por polaridad");
				else if ( _io_ports[M6812_PWCNT0] > M6812_PWDTY0 )
					serial_print("Polaridad negativa");

				if ( _io_ports[M6812_PWCNT0] == M6812_PWPER0 )
					_io_ports[M6812_PWCNT0] = RETURN;

				break;

			case 1:
				_io_ports[M6812_PWCNT1] |= M6812B_PCLK1;

				if ( M6812_PWCNT1 <= M6812_PWDTY1)
					serial_print("Nivel indicado por polaridad");
				else if ( M6812_PWCNT1 > M6812_PWDTY1 )
					serial_print("Polaridad negativa");

				if ( _io_ports[M6812_PWCNT1] == M6812_PWPER1 )
					_io_ports[M6812_PWCNT1] = RETURN;
				break;

			case 2:
				_io_ports[M6812_PWCNT2] |= M6812B_PCLK2;

				if ( M6812_PWCNT2 <= M6812_PWDTY2 )
					serial_print("Nivel indicado por polaridad");
				else if ( M6812_PWCNT2 > M6812_PWDTY2 )
					serial_print("Polaridad negativa");

				if ( _io_ports[M6812_PWCNT2] == M6812_PWPER2 )
					_io_ports[M6812_PWCNT2] = RETURN;
				break;

			case 3:
				_io_ports[M6812_PWCNT3] |= M6812B_PCLK3;

				if ( M6812_PWCNT3 <= M6812_PWDTY3 )
					serial_print("Nivel indicado por polaridad");
				if ( M6812_PWCNT3 > M6812_PWDTY3 )
					serial_print("Polaridad negativa");

				if ( _io_ports[M6812_PWCNT3] == M6812_PWPER3 )
					_io_ports[M6812_PWCNT3] = RETURN;
				break;
		}
	}
	//Alineamiento centrado
	else
	{
		switch (canal_seleccionado)
		{
			case 0:

				if ( _io_ports[M6812_PWCNT0] == M6812_PWPER0 )
					_io_ports[M6812_PWCNT0] &= ~M6812B_PCLK0;
				if ( _io_ports[M6812_PWCNT0] == M6812_PWPER0 )
					_io_ports[M6812_PWCNT0] |= M6812B_PCLK0;

				if ( M6812_PWCNT0 <= M6812_PWDTY0)
					serial_print("Nivel indicado por polaridad");
				else if ( M6812_PWCNT0 > M6812_PWDTY0 )
					serial_print("Polaridad negativa");

				if ( _io_ports[M6812_PWCNT0] == M6812_PWPER0 )
					_io_ports[M6812_PWCNT0] = RETURN;
				break;

			case 1:
				if ( _io_ports[M6812_PWCNT1] == M6812_PWPER1 )
					_io_ports[M6812_PWCNT1] &= ~M6812B_PCLK1;
				else if ( _io_ports[M6812_PWCNT1] == M6812_PWPER1 )
					_io_ports[M6812_PWCNT1] |= M6812B_PCLK1;

				if ( M6812_PWCNT1 <= M6812_PWDTY1)
					serial_print("Nivel indicado por polaridad");
				else if ( M6812_PWCNT1 > M6812_PWDTY1 )
					serial_print("Polaridad negativa");

				if ( _io_ports[M6812_PWCNT1] == M6812_PWPER1 )
					_io_ports[M6812_PWCNT1] = RETURN;
				break;

			case 2:
				if ( _io_ports[M6812_PWCNT2] == M6812_PWPER2 )
					_io_ports[M6812_PWCNT2] &= ~M6812B_PCLK2;
				else if ( _io_ports[M6812_PWCNT2] == M6812_PWPER2 )
					_io_ports[M6812_PWCNT2] |= M6812B_PCLK2;

				if ( M6812_PWCNT2 <= M6812_PWDTY2 )
					serial_print("Nivel indicado por polaridad");
				else if ( M6812_PWCNT2 > M6812_PWDTY2 )
					serial_print("Polaridad negativa");

				if ( _io_ports[M6812_PWCNT2] == M6812_PWPER2 )
					_io_ports[M6812_PWCNT2] = RETURN;
				break;

			case 3:
				if ( _io_ports[M6812_PWCNT3] == M6812_PWPER3 )
					_io_ports[M6812_PWCNT3] &= ~M6812B_PCLK3;
				else if ( _io_ports[M6812_PWCNT3] == M6812_PWPER3 )
					_io_ports[M6812_PWCNT3] |= M6812B_PCLK3;

				if ( M6812_PWCNT3 <= M6812_PWDTY3)
					serial_print("Nivel indicado por polaridad");
				else if ( M6812_PWCNT3 > M6812_PWDTY3 )
					serial_print("Polaridad negativa");

				if ( _io_ports[M6812_PWCNT3] == M6812_PWPER3 )
					_io_ports[M6812_PWCNT3] = RETURN;
				break;
		}
	}


}


void pwm_period( int8_t PERetapas_,int8_t canal)
{
	switch (canal)
	{
		case 0:
			_io_ports[M6812_PWPER0] = PERetapas_;
			break;

		case 1:
			_io_ports[M6812_PWPER1] = PERetapas_;
			break;

		case 2:
			_io_ports[M6812_PWPER2] = PERetapas_;
			break;

		case 3:
			_io_ports[M6812_PWPER3] = PERetapas_;
			break;
	}
}


void pwm_habilitacion_canal (int8_t canal) {
	switch (canal){
		case 0:
			_io_ports[M6812_PWEN] |= M6812B_PWEN0; //Activar pwm pin 0
			break;
		case 1:
			_io_ports[M6812_PWEN] |= M6812B_PWEN1;
			break;
		case 2:
			_io_ports[M6812_PWEN] |= M6812B_PWEN2;
			break;
		case 3:
			_io_ports[M6812_PWEN] |= M6812B_PWEN3; // Activar pwm pin 3
			break;
	}
}
void pwm_deshabilitacion_canal (int8_t canal){
	switch (canal){
		case 0:
			_io_ports[M6812_PWEN] &= ~M6812B_PWEN0; //Activar pwm pin 0
			break;
		case 1:
			_io_ports[M6812_PWEN] &= ~M6812B_PWEN1;
			break;
		case 2:
			_io_ports[M6812_PWEN] &= ~M6812B_PWEN2;
			break;
		case 3:
			_io_ports[M6812_PWEN] &= ~M6812B_PWEN3; // Activar pwm pin 3
			break;
	}
}




void pwm_modificar_etapas( int8_t etapas, int8_t canal)
{
	switch (canal)
	{
		case 0:
			_io_ports[M6812_PWDTY0] = etapas;
			break;

		case 1:
			_io_ports[M6812_PWDTY1] = etapas;
			break;

		case 2:
			_io_ports[M6812_PWDTY2] = etapas;
			break;

		case 3:
			_io_ports[M6812_PWDTY3] = etapas;
			break;
	}
}

void pwm_modificar_porcentaje( int8_t porcentaje, int8_t canal)
{
	int8_t cociente = porcentaje;
 	int8_t total = 100;
	int8_t  divisor = MCD(cociente,total);
	cociente /= divisor;
	total /= divisor;
/*
  serial_print("\r\ndivisor: ");
  serial_printdecbyte(divisor);
  serial_print("\r\ncociente: ");
  serial_printdecbyte(cociente);
  serial_print("\r\ntotal: ");
  serial_printdecbyte(total);
  serial_print("\r\n:_io_ports[M6812_PWPER0]/total ");
  serial_printdecbyte(_io_ports[M6812_PWPER0]/total);

  serial_print("\r\nper*%/100 = ");
  serial_printdecbyte((uint16_t)_io_ports[M6812_PWPER0]*porcentaje/100);
*/

	switch (canal)
	{
	case 0:
		pwm_modificar_etapas(_io_ports[M6812_PWPER0]/total*cociente,canal);
		break;
	case 1:
		pwm_modificar_etapas(_io_ports[M6812_PWPER1]/total*cociente,canal);
		break;
	case 2:
		pwm_modificar_etapas(_io_ports[M6812_PWPER2]/total*cociente,canal);
		break;
	case 3:
		pwm_modificar_etapas(_io_ports[M6812_PWPER3]/total*cociente,canal);
		break;
	}

}

int8_t MCD(int8_t a, int8_t b){
	if (b == 0) return a;
	return MCD(b,a%b);
}

