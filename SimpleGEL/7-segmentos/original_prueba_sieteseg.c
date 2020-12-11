#include <stdio.h>
#include <timer.h>
#include <gpio.h>
#include <ad.h>

uint8_t valor7Seg[] = {0, 0, 0, 0};
uint16_t retardo;
uint8_t cual;


void refresh(); ///función utilizada por el timer, para el refresco de la pantalla de siete segmentos
void myUserFunction();
void sieteSeg_valor (uint16_t valor);
void sieteSeg_digitos (uint8_t * valor);
void sieteSeg_init ();

void myUserFunction(uint16_t * valores)
{
	
			sieteSeg_valor(valores[0]);  ///envío el valor obtenido solo si ha cambiado
}

void sieteSeg_valor (uint16_t valor)
{
	uint8_t i = 0;
	uint8_t led;
	
	
	valor7Seg[0] = 0x1f;
	valor7Seg[1] = 0x2f;
	valor7Seg[2] = 0x4f;
	valor7Seg[3] = 0x8f;
	    
	
   if ((valor >0) && (valor <=9999))
   {
		
		
    while (valor > 0) {

        valor7Seg[i] = valor % 10;
		led = 0x10 << i;
		valor7Seg[i] |= led; 
        valor = valor / 10;
        i++;
    }
   }
    else
        valor7Seg[0] = 0x10;
		
	
}

void sieteSeg_digitos (uint8_t * valor)
{
	
				valor7Seg[0] = valor[3] | 0x10; 
				valor7Seg[1] = valor[2] | 0x20; 
				valor7Seg[2] = valor[1] | 0x40; 
				valor7Seg[3] = valor[0] | 0x80; 
	
}

void sieteSeg_init ()
{
	gpio_set_output_all_reg(SET_PIN_G); ///configuramos puerto G con todos sus pines en modo salida
	sieteSeg_valor(50);
	timer_init(3); ///factor de escala respecto al reloj principal
	timer_repeat_call(5000,&refresh); ///valor en microsegundos de llamada
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
	//uint16_t number=50;
	//char c;
	serial_init();
	serial_print("\nFuncion del decodificador de Siete Segmentos");
	
	sieteSeg_init();
	
	uint8_t valor7Seg[] = {1, 2, 3, 4};
	sieteSeg_digitos(valor7Seg);
	
	    ad_pin_inicio(3,0); ///utilizaremos el pin 3 para la recogida de valores

		ad_modulo(0,0); ///módulo cero

		ad_ocho_o_diez_bits(1,0); ///resolución de 10 bits

        ad_ciclos_muestreo(3,0); ///16 ciclos por muestreo

		ad_scanMode(1,0); ///activo modo scan

		ad_cantidadConversiones(1,0); ///una conversión
	
		ad_activarInterrupMode(1); ///habilito el modo interrupción
		ad_setUserFunction(&myUserFunction); ///configuro una rutina de usuario.
		ad_start(); ///se inicia el proceso

	while (1)
	{
		
		/*while((c = serial_recv()) != ' ');
		number ++;
		sieteSeg_valor(number);*/
	}
}
