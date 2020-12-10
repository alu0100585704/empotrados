#include <stdio.h>
#include <timer.h>
#include <gpio.h>

char valor7Seg[] = {0, 0, 0, 0};
uint16_t retardo;
uint8_t cual;


void refresh();

void sieteSeg_valor (uint16_t valor)
{
	
	
	
}

void sieteSeg_digitos (uint8_t* valor)
{
		
			gpio_write_port(M6812_PORTG, valor7Seg[cual]); 			
			cual++;
			if (cual == 4 )
			 cual = 0;
			
	
}

void sieteSeg_init ()
{
	gpio_set_output_all_reg(SET_PIN_G);
	cual = 0;
	valor7Seg[0] = 0x10;
	valor7Seg[1] = 0x21;
	valor7Seg[2] = 0x42;
	valor7Seg[3] = 0x83;
	timer_init(5);
	timer_repeat_call(1000,&refresh);
	
	
//	sieteSeg_digitos(valor7Seg);
	
}
void refresh()
{
	sieteSeg_digitos(valor7Seg);
}

int main ()
{
	serial_init();
	serial_print("\nFuncion del decodificador de Siete Segmentos");
	
	sieteSeg_init();
	
	while (1)
	{
		
		
	}
}
