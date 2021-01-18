#include <sys/param.h>
#include <sys/ports.h>
#include <sys/interrupts.h>
#include <types.h>
#include <sys/sio.h>
#include <sys/locks.h>
#include <pwm.h>


int main(){
  serial_init();
    serial_print("\r\nM6812_pwclk: ");
  serial_printbinbyte(_io_ports[M6812_PWCLK]);
  serial_print("\r\nM6812_pwpol: ");
  serial_printbinbyte( _io_ports[M6812_PWPOL]);

  serial_print("\r\nM6812_PWSCAL0: ");
  serial_printdecbyte( _io_ports[M6812_PWSCAL0]);

  serial_print("\r\nM6812_pwcnt0: ");
  serial_printdecbyte( _io_ports[M6812_PWCNT0]);
  serial_print("\r\nM6812_pwper0: ");
  serial_printdecbyte( _io_ports[M6812_PWPER0]);
  serial_print("\r\nM6812_pwdty0: ");
  serial_printdecbyte( _io_ports[M6812_PWDTY0]);
  serial_print("\r\nM6812_pwEN: ");
  serial_printbinbyte( _io_ports[M6812_PWEN]);

  pwm_set_clock(RELOJ_A,7,GRANO_FINO,3);
  pwm_set_polarity(0,ALTA);
  pwm_aling(ALINEAMIENTO_IZQ,0);
  pwm_period(200,0);
  pwm_habilitacion_canal(0);
  pwm_modificar_etapas(20,0);

  serial_print("\r\n\nValores tras configuración: ");


  serial_print("\r\nM6812_pwclk: ");
  serial_printbinbyte(_io_ports[M6812_PWCLK]);
  serial_print("\r\nM6812_pwpol: ");
  serial_printbinbyte( _io_ports[M6812_PWPOL]);

  serial_print("\r\nM6812_PWSCAL0: ");
  serial_printdecbyte( _io_ports[M6812_PWSCAL0]);

  serial_print("\r\nM6812_pwcnt0: ");
  serial_printdecbyte( _io_ports[M6812_PWCNT0]);
  serial_print("\r\nM6812_pwper0: ");
  serial_printdecbyte( _io_ports[M6812_PWPER0]);
  serial_print("\r\nM6812_pwdty0: ");
  serial_printdecbyte( _io_ports[M6812_PWDTY0]);
  serial_print("\r\nM6812_pwEN: ");
  serial_printbinbyte( _io_ports[M6812_PWEN]);

   while(1){
     serial_print("\r\n Introduzca nuevo duty %:");
     int8_t a =serial_getdecbyte();
     pwm_modificar_porcentaje(a,0);

    serial_print("\r\nM6812_pwcnt0: ");
    serial_printdecbyte( _io_ports[M6812_PWCNT0]);
    serial_print("\r\nM6812_pwper0: ");
    serial_printdecbyte( _io_ports[M6812_PWPER0]);
    serial_print("\r\nM6812_pwdty0: ");
    serial_printdecbyte( _io_ports[M6812_PWDTY0]);

   }
}
