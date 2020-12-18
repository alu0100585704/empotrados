#include "teclado.h"

char teclado_getch_timeout(uint32_t milis)
{
char tecla='T';
Optional data;


///ponemos todo a cero para estar seguro que las columnas devuelven 1
gpio_write_port(M6812_PORTH,0);

//// Espero por si ya hay una tecla apretada


   data = gpio_read_port(M6812_PORTH);	
while (!(data.data & C1) || !(data.data & C2) || !(data.data & C3))
{
	while (!(data.data & C1) || !(data.data & C2) || !(data.data & C3))
	 {
		
		data = gpio_read_port(M6812_PORTH);	
	 }

   timer_sleep(20000);
data = gpio_read_port(M6812_PORTH); ///leo otra vez después de estar estabilizado
}

///Vuelvo a poner todas las filas a cero, para que las columnas den 1, indicando que no hay nada pulsado
gpio_write_port(M6812_PORTH,0);


////esperamos pulsación

uint32_t contador = 0;
while ((data.data & C1) && (data.data & C2) && (data.data & C3) && contador<milis)
{
	 while ((data.data & C1) && (data.data & C2) && (data.data & C3) && contador<milis)
	 {
		contador++;
		data = gpio_read_port(M6812_PORTH);	
		timer_sleep(1000);
	 }

 timer_sleep(20000);
 data = gpio_read_port(M6812_PORTH);///leo otra vez despues de estabilizado
}

///ponemos todos a 1 para que el valor de la columna se ponga a uno, ahora vamos a escanear poniendo cada fila a cero, para ver cuando
///el valor de la columna se vuelve a poner a cero

gpio_write_port(M6812_PORTH,0XFF);

 if (!(data.data & C1))
   {
      int encontrada = 0;
   
	if (!encontrada)
	{
           
	gpio_write_port(M6812_PORTH,0xDF); ///FILA 1 A CERO Y DEMÁS A 1
  	 data = gpio_read_port(M6812_PORTH);


	if (!(data.data & C1))
             {
			tecla = '1';
			encontrada = 1;
	      }

	}

	if (!encontrada)
	{
           
	gpio_write_port(M6812_PORTH,0xFE); ///FILA 2 A CERO Y DEMÁS A 1
	
  	  data = gpio_read_port(M6812_PORTH);

	if (!(data.data & C1))
             {
			tecla = '4';
			encontrada = 1;
	      }

	}
	
	if (!encontrada)
	{
           
	gpio_write_port(M6812_PORTH,0xFD); ///FILA 3 A CERO Y DEMÁS A 1
  	  data = gpio_read_port(M6812_PORTH);

	if (!(data.data & C1))
             {
			tecla = '7';
			encontrada = 1;
	      }

	}
	if (!encontrada)
	{
           
	gpio_write_port(M6812_PORTH,0xF7); ///FILA 4 A CERO Y DEMÁS A 1
  	  data = gpio_read_port(M6812_PORTH);

	if (!(data.data & C1))
             {
			tecla = '*';
			encontrada = 1;
	      }

	}

    }

 if (!(data.data & C2))
   {
      int encontrada = 0;
   
	if (!encontrada)
	{
           
	gpio_write_port(M6812_PORTH,0xDF); ///FILA 1 A CERO Y DEMÁS A 1
  	 data = gpio_read_port(M6812_PORTH);


	if (!(data.data & C2))
             {
			tecla = '2';
			encontrada = 1;
	      }

	}

	if (!encontrada)
	{
           
	gpio_write_port(M6812_PORTH,0xFE); ///FILA 2 A CERO Y DEMÁS A 1
	
  	  data = gpio_read_port(M6812_PORTH);

	if (!(data.data & C2))
             {
			tecla = '5';
			encontrada = 1;
	      }

	}
	
	if (!encontrada)
	{
           
	gpio_write_port(M6812_PORTH,0xFD); ///FILA 3 A CERO Y DEMÁS A 1
  	  data = gpio_read_port(M6812_PORTH);

	if (!(data.data & C2))
             {
			tecla = '8';
			encontrada = 1;
	      }

	}
	if (!encontrada)
	{
           
	gpio_write_port(M6812_PORTH,0xF7); ///FILA 4 A CERO Y DEMÁS A 1
  	  data = gpio_read_port(M6812_PORTH);

	if (!(data.data & C2))
             {
			tecla = '0';
			encontrada = 1;
	      }

	}

    }

 if (!(data.data & C3))
   {
      int encontrada = 0;
   
	if (!encontrada)
	{
           
	gpio_write_port(M6812_PORTH,0xDF); ///FILA 1 A CERO Y DEMÁS A 1
  	 data = gpio_read_port(M6812_PORTH);


	if (!(data.data & C3))
             {
			tecla = '3';
			encontrada = 1;
	      }

	}

	if (!encontrada)
	{
           
	gpio_write_port(M6812_PORTH,0xFE); ///FILA 2 A CERO Y DEMÁS A 1
	
  	  data = gpio_read_port(M6812_PORTH);

	if (!(data.data & C3))
             {
			tecla = '6';
			encontrada = 1;
	      }

	}
	
	if (!encontrada)
	{
           
	gpio_write_port(M6812_PORTH,0xFD); ///FILA 3 A CERO Y DEMÁS A 1
  	  data = gpio_read_port(M6812_PORTH);

	if (!(data.data & C3))
             {
			tecla = '9';
			encontrada = 1;
	      }

	}
	if (!encontrada)
	{
           
	gpio_write_port(M6812_PORTH,0xF7); ///FILA 4 A CERO Y DEMÁS A 1
  	  data = gpio_read_port(M6812_PORTH);

	if (!(data.data & C3))
             {
			tecla = '#';
			encontrada = 1;
	      }

	}

    }
return tecla;
}

char teclado_getch()
{
char tecla='T';
Optional data;


///ponemos todo a cero para estar seguro que las columnas devuelven 1
gpio_write_port(M6812_PORTH,0);

//// Espero por si ya hay una tecla apretada


   data = gpio_read_port(M6812_PORTH);	
while (!(data.data & C1) || !(data.data & C2) || !(data.data & C3))
{
	while (!(data.data & C1) || !(data.data & C2) || !(data.data & C3))
	 {
		
		data = gpio_read_port(M6812_PORTH);	
	 }

   timer_sleep(20000);
data = gpio_read_port(M6812_PORTH); ///leo otra vez después de estar estabilizado
}

///Vuelvo a poner todas las filas a cero, para que las columnas den 1, indicando que no hay nada pulsado
gpio_write_port(M6812_PORTH,0);


////esperamos pulsación

while ((data.data & C1) && (data.data & C2) && (data.data & C3))
{
	 while ((data.data & C1) && (data.data & C2) && (data.data & C3))
	 {
		
		data = gpio_read_port(M6812_PORTH);	
	 }
 timer_sleep(20000);
 data = gpio_read_port(M6812_PORTH);///leo otra vez despues de estabilizado
}

///ponemos todos a 1 para que el valor de la columna se ponga a uno, ahora vamos a escanear poniendo cada fila a cero, para ver cuando
///el valor de la columna se vuelve a poner a cero

gpio_write_port(M6812_PORTH,0XFF);

 if (!(data.data & C1))
   {
      int encontrada = 0;
   
	if (!encontrada)
	{
           
	gpio_write_port(M6812_PORTH,0xDF); ///FILA 1 A CERO Y DEMÁS A 1
  	 data = gpio_read_port(M6812_PORTH);


	if (!(data.data & C1))
             {
			tecla = '1';
			encontrada = 1;
	      }

	}

	if (!encontrada)
	{
           
	gpio_write_port(M6812_PORTH,0xFE); ///FILA 2 A CERO Y DEMÁS A 1
	
  	  data = gpio_read_port(M6812_PORTH);

	if (!(data.data & C1))
             {
			tecla = '4';
			encontrada = 1;
	      }

	}
	
	if (!encontrada)
	{
           
	gpio_write_port(M6812_PORTH,0xFD); ///FILA 3 A CERO Y DEMÁS A 1
  	  data = gpio_read_port(M6812_PORTH);

	if (!(data.data & C1))
             {
			tecla = '7';
			encontrada = 1;
	      }

	}
	if (!encontrada)
	{
           
	gpio_write_port(M6812_PORTH,0xF7); ///FILA 4 A CERO Y DEMÁS A 1
  	  data = gpio_read_port(M6812_PORTH);

	if (!(data.data & C1))
             {
			tecla = '*';
			encontrada = 1;
	      }

	}

    }

 if (!(data.data & C2))
   {
      int encontrada = 0;
   
	if (!encontrada)
	{
           
	gpio_write_port(M6812_PORTH,0xDF); ///FILA 1 A CERO Y DEMÁS A 1
  	 data = gpio_read_port(M6812_PORTH);


	if (!(data.data & C2))
             {
			tecla = '2';
			encontrada = 1;
	      }

	}

	if (!encontrada)
	{
           
	gpio_write_port(M6812_PORTH,0xFE); ///FILA 2 A CERO Y DEMÁS A 1
	
  	  data = gpio_read_port(M6812_PORTH);

	if (!(data.data & C2))
             {
			tecla = '5';
			encontrada = 1;
	      }

	}
	
	if (!encontrada)
	{
           
	gpio_write_port(M6812_PORTH,0xFD); ///FILA 3 A CERO Y DEMÁS A 1
  	  data = gpio_read_port(M6812_PORTH);

	if (!(data.data & C2))
             {
			tecla = '8';
			encontrada = 1;
	      }

	}
	if (!encontrada)
	{
           
	gpio_write_port(M6812_PORTH,0xF7); ///FILA 4 A CERO Y DEMÁS A 1
  	  data = gpio_read_port(M6812_PORTH);

	if (!(data.data & C2))
             {
			tecla = '0';
			encontrada = 1;
	      }

	}

    }

 if (!(data.data & C3))
   {
      int encontrada = 0;
   
	if (!encontrada)
	{
           
	gpio_write_port(M6812_PORTH,0xDF); ///FILA 1 A CERO Y DEMÁS A 1
  	 data = gpio_read_port(M6812_PORTH);


	if (!(data.data & C3))
             {
			tecla = '3';
			encontrada = 1;
	      }

	}

	if (!encontrada)
	{
           
	gpio_write_port(M6812_PORTH,0xFE); ///FILA 2 A CERO Y DEMÁS A 1
	
  	  data = gpio_read_port(M6812_PORTH);

	if (!(data.data & C3))
             {
			tecla = '6';
			encontrada = 1;
	      }

	}
	
	if (!encontrada)
	{
           
	gpio_write_port(M6812_PORTH,0xFD); ///FILA 3 A CERO Y DEMÁS A 1
  	  data = gpio_read_port(M6812_PORTH);

	if (!(data.data & C3))
             {
			tecla = '9';
			encontrada = 1;
	      }

	}
	if (!encontrada)
	{
           
	gpio_write_port(M6812_PORTH,0xF7); ///FILA 4 A CERO Y DEMÁS A 1
  	  data = gpio_read_port(M6812_PORTH);

	if (!(data.data & C3))
             {
			tecla = '#';
			encontrada = 1;
	      }

	}

    }
return tecla;
}


/*
 * @brief Funcion para inicializar el Decodificador y el Timer.
 */
 
void teclado_init ()
{
	 timer_init(3); ///factor de escala respecto al reloj principal
	
	gpio_pup_enable(M6812_PORTH); ///activamos resistencia de pull up en el registro
	
	///configuramos puerto H con los pines para las filas como salida y valor 0(0 voltios)
  gpio_set_output(SET_PIN_H,0);
  gpio_set_output(SET_PIN_H,1);
  gpio_set_output(SET_PIN_H,3);
  gpio_set_output(SET_PIN_H,5);
  
  ///configuramos puerto H como entrada los pines de las columnas
  gpio_set_input(SET_PIN_H,2);
  gpio_set_input(SET_PIN_H,4);
  gpio_set_input(SET_PIN_H,6);

	
gpio_write_port(M6812_PORTH,0);

  
  
}
