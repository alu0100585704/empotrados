// Fecha: 2020-12-09
// Autor: Alberto Hamilton Castro

// Prueba de la función `ad_tiempo_muestreo ()`

#include <types.h>
#include <sys/interrupts.h>
#include <sys/sio.h>
#include <ad.h>

int main() {

  serial_init();
  serial_print("\ntest_ad_tiempo_muestreo.c ==========\n");

  while(1) {
    serial_print("\n\nNueva configuración");
    ad_ciclos_muestreo(0,1);
    ad_ocho_o_diez_bits(0,1);

    ad_tiempo_muestreo();
  }
}

