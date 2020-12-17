#include "teclado.h"


char teclado_getch()
{
char tecla='T';
Optional data;
int col=0;

	
   data = gpio_read_port(M6812_PORTH);	

	while (!(data.data & C1) || !(data.data & C2) || !(data.data & C3))
	 {
		
		data = gpio_read_port(M6812_PORTH);	
	 }

   timer_sleep(20000);
   data = gpio_read_port(M6812_PORTH);	

	 while ((data.data & C1) && (data.data & C2) && (data.data & C3))
	 {
		
		data = gpio_read_port(M6812_PORTH);	
	 }

 timer_sleep(20000);



 if (!(data.data & C1))
   {
      int encontrada = 0;
   
	if (!encontrada)
	{
           
	   ///filas 1 a cero y filas 2,3,4 a 1
	  gpio_write_pin(M6812_PORTH, 5, 0); ///fila 1	  
	  gpio_write_pin(M6812_PORTH, 0, 1); //fila 2
  	  gpio_write_pin(M6812_PORTH, 1, 1); ///fila 3
          gpio_write_pin(M6812_PORTH, 3, 1); ///fila 4
  	  data = gpio_read_port(M6812_PORTH);

	if (!(data.data & C1))
             {
			tecla = '1';
			encontrada = 1;
	      }

	}

	if (!encontrada)
	{
           
	   ///filas 1 a cero y filas 2,3,4 a 1
	  gpio_write_pin(M6812_PORTH, 5, 1); ///fila 1	  
	  gpio_write_pin(M6812_PORTH, 0, 0); //fila 2
  	  gpio_write_pin(M6812_PORTH, 1, 1); ///fila 3
          gpio_write_pin(M6812_PORTH, 3, 1); ///fila 4
  	  data = gpio_read_port(M6812_PORTH);

	if (!(data.data & C1))
             {
			tecla = '4';
			encontrada = 1;
	      }

	}
	
	if (!encontrada)
	{
           
	   ///filas 1 a cero y filas 2,3,4 a 1
	  gpio_write_pin(M6812_PORTH, 5, 1); ///fila 1	  
	  gpio_write_pin(M6812_PORTH, 0, 1); //fila 2
  	  gpio_write_pin(M6812_PORTH, 1, 0); ///fila 3
          gpio_write_pin(M6812_PORTH, 3, 1); ///fila 4
  	  data = gpio_read_port(M6812_PORTH);

	if (!(data.data & C1))
             {
			tecla = '7';
			encontrada = 1;
	      }

	}
    }
/*	col = 1;

 if (!(data.data & C2))
	col = 2;

 if (!(data.data & C3))
	col = 3;
    	  
	 
	///filas 1 a cero y filas 2,3,4 a 1
	  gpio_write_pin(M6812_PORTH, 5, 1); ///fila 1	  
	  gpio_write_pin(M6812_PORTH, 0, 1); //fila 2
  	  gpio_write_pin(M6812_PORTH, 1, 1); ///fila 3
          gpio_write_pin(M6812_PORTH, 3, 1); ///fila 4

	///filas 1 a cero y filas 2,3,4 a 1
	  gpio_write_pin(M6812_PORTH, 5, 0); ///fila 1	  
	  gpio_write_pin(M6812_PORTH, 0, 1); //fila 2
  	  gpio_write_pin(M6812_PORTH, 1, 1); ///fila 3
          gpio_write_pin(M6812_PORTH, 3, 1); ///fila 4

	  data = gpio_read_port(M6812_PORTH);
	  
 	 if (col==1)
		if (!(data.data & C1))
			tecla = '1';
	
 	 if (col==2)
		if (!(data.data & C2))
			tecla = '2';
			
 	 if (col==3)
		if (!(data.data & C3))
			tecla = '3';	

	///filas 1 a cero y filas 2,3,4 a 1
	  gpio_write_pin(M6812_PORTH, 5, 1); ///fila 1	  
	  gpio_write_pin(M6812_PORTH, 0, 0); //fila 2
  	  gpio_write_pin(M6812_PORTH, 1, 1); ///fila 3
          gpio_write_pin(M6812_PORTH, 3, 1); ///fila 4

	  data = gpio_read_port(M6812_PORTH);
	  
 	 if (col==1)
		if (!(data.data & C1))
			tecla = '4';
	
 	 if (col==2)
		if (!(data.data & C2))
			tecla = '5';
			
 	 if (col==3)
		if (!(data.data & C3))
			tecla = '6';	

	  gpio_write_pin(M6812_PORTH, 5, 0); ///fila 1	  
	  gpio_write_pin(M6812_PORTH, 0, 0); //fila 2
  	  gpio_write_pin(M6812_PORTH, 1, 0); ///fila 3
          gpio_write_pin(M6812_PORTH, 3, 0); ///fila 4*/
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
  
	  gpio_write_pin(M6812_PORTH, 5, 0); ///fila 1	  
	  gpio_write_pin(M6812_PORTH, 0, 0); //fila 2
  	  gpio_write_pin(M6812_PORTH, 1, 0); ///fila 3
          gpio_write_pin(M6812_PORTH, 3, 0); ///fila 4

  
  ///configuramos puerto G como entrada los pines de las columnas
  gpio_set_input(SET_PIN_H,2);
  gpio_set_input(SET_PIN_H,4);
  gpio_set_input(SET_PIN_H,6);
 
  
  
}
