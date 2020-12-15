/* ****************************************
 * @author Juan Siverio Rojas
 * @author Felipe Esquivel Delgado
 * @date 18/12/2020
	************************************** */

#include "sieteseg.h"

uint8_t valor7Seg[] = {0, 0, 0, 0}; ///inicializados valores del display a 0
uint8_t cual; ///Variable para seleccionar display del decodificador se modifica

/*
 * @brief Función para convertir el valor recibido y mostrarlo por los display
 * correspondientes.
 * @param valor Valor en decimal que se desea mostrar por el decodificador que
 * está en el rango de 0-9999.
 */
void sieteSeg_valor (uint16_t valor)
{
  uint8_t i = 0;
  uint8_t led;

  valor7Seg[0] = 0x1f;
  valor7Seg[1] = 0x2f;
  valor7Seg[2] = 0x4f;
  valor7Seg[3] = 0x8f;

  if ((valor > 0) && (valor <= 9999))
  {
    while (valor > 0) {
      valor7Seg[i] = valor % 10;
      led = 0x10 << i;
      valor7Seg[i] |= led;
      valor = valor / 10;
      i ++;
    }
  }
  else
    valor7Seg[0] = 0x10;
}

/*
 * @brief Funcion para poder pasarle los valores directamente y poder
 * imprimirlos.
 * @param valor Puntero al vector de valores para asignar al vector que se
 * imprimirá en los distintos display
 */
void sieteSeg_digitos (uint8_t * valor)
{
  valor7Seg[0] = valor[3] | 0x10;
  valor7Seg[1] = valor[2] | 0x20;
  valor7Seg[2] = valor[1] | 0x40;
  valor7Seg[3] = valor[0] | 0x80;
}

/*
 * @brief Funcion para inicializar el Decodificador y el Timer.
 */
void sieteSeg_init ()
{
	///configuramos puerto G con todos sus pines en modo salida
  gpio_set_output_all_reg(SET_PIN_G);
  sieteSeg_valor(50);
  timer_init(3); ///factor de escala respecto al reloj principal
  timer_repeat_call(5000, &refresh); ///valor en microsegundos de llamada
  cual = 0;
}

/*
 * @brief Funcion de refresco que escribe el valor en el display seleccionado
 * y va comprobando los siguientes cada cierto tiempo.
 */
void refresh()
{
  gpio_write_port(M6812_PORTG, valor7Seg[cual]);
  cual ++;
  if (cual == 4 )
	  cual = 0;
}
