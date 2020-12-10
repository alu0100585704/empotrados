#include <sys/param.h>
#include <sys/ports.h>
#include <sys/interrupts.h>
#include <types.h>
#include <sys/sio.h>
#include <sys/locks.h>
#include <pwm.h>


int main(){
  serial_init();
  serial_print("\r\nTest avanzado de PWM"
               "\r\nA continuacion vamos a generar el reloj del PWM."
               "\r\nEl reloj del pwm se basa en la division del reloj"
               "\r\nbase de la placa (25Mhz) apartir de ellos se puede usar"
               "\r\nuna presicion de grano fino o de grano grueso. En la "
               "\r\nprecion de grano grueso, se emplea la siguiente formula"
               "\r\nReloj_base / 2^N = Reloj_PWM, 0 <= N < 8. Si emplemaos la presicion de"
               "\r\ngrano fino apartir de reloj generado en el grano grueso se"
               "\r\naplica la siguiente formula: "
               "\r\nReloj_PWM /2 *(valor_contador + 1) = Reloj_PWM_Grano_fino."
               "\r\nSeleciona el reloj que quieres configurar Reloj_A"
               "\r\nIntroduce 0 para reloj_A o 1 para reloj_B: ");
  uint8_t reloj_a = serial_getbinbyte();
  serial_print("\r\nIntroduce el factor N valores entre 0 - 7: ");
  uint8_t factor_n = serial_getdecbyte();
  serial_print("\r\nSeleciona la precicion 0 para Grano_grueso o 1 para Grano_Fino: ");
  uint8_t grano_fino = serial_getbinbyte();
  serial_print("\r\nIntroduce el valor del valor_contador, su valores entre 0-255:");
  uint8_t valor_contador = serial_getdecbyte();
  pwm_set_clock(reloj_a,factor_n,grano_fino,valor_contador);
  serial_print("\r\nSelecional canal ha habilitar 0-4:");
  uint8_t canal = serial_getdecbyte();
  serial_print("\r\nSeleciona 1 para polaridad en alta y 0 para polaridad en baja:");
  uint8_t polaridad_alta = serial_getbinbyte();
  serial_print("\r\nSeleciona 1 para alineamiento a la izquier y 0 para alineamiento a la derecha: ");
  uint8_t alinneamiento_izq = serial_getbinbyte();
  serial_print("\r\nSeleciona el periodo del pwm en un rango de 0-255: ");
  uint8_t periodo = serial_getdecbyte();
  pwm_set_polarity(canal,polaridad_alta);
  pwm_aling(alinneamiento_izq,canal);
  pwm_period(periodo,canal);
  pwm_habilitacion_canal(canal);
  while(1){
    serial_print("\r\n Introduzca nuevo duty:");
    int8_t a =serial_getdecbyte();
    pwm_modificar_etapas(a,0);
  }
}
