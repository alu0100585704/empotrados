#include <stdio.h>
#include <timer.h>
#include <gpio.h>

uint8_t valor7Seg[] = {0, 0, 0, 0};
uint16_t retardo;
uint8_t cual;


void refresh();
void sieteSeg_valor (uint16_t valor);
void sieteSeg_digitos (uint8_t * valor);
void sieteSeg_init ();


void sieteSeg_valor (uint16_t valor)
{
	uint8_t i = 0;
	uint8_t led = 0x10;
	
   if ((valor >=0) && (valor <=9999))
   {
	
    while (valor > 10) {

        valor7Seg[i] = valor % 10;
		led = led << i;
		valor7Seg[i] |= led; 
        valor = valor / 10;
        i++;
    }
	if (valor != 0)
	{
		valor7Seg[i] = valor;
		led = led << i;
		valor7Seg[i] |= led; 
	}
	
   }
	
	
}

void sieteSeg_digitos (uint8_t * valor)
{
	int i;
		for (i = 0; i < 4; i++)
			valor7Seg[i] = valor[i]; ///copio los valores a mi vector.
			
	
}

void sieteSeg_init ()
{
	gpio_set_output_all_reg(SET_PIN_G); ///configuramos puerto G con todos sus pines en modo salida
	sieteSeg_valor(0);
	timer_init(5);
	timer_repeat_call(1000,&refresh);
	cual = 0;
}
void refresh()
{
			gpio_write_port(M6812_PORTG, valor7Seg[cual]); 			
			cual++;
			if (cual == 4 )
			 cual = 0;

}

int main ()
{
	uint16_t number=0;
	char c;
	serial_init();
	serial_print("\nFuncion del decodificador de Siete Segmentos");
	
	sieteSeg_init();
	
	while (1)
	{
		
		while((c = serial_recv()) != ' ');
		number ++;
		sieteSeg_valor(number);
	}
}
