#include <timer.h>

uint32_t anterior = 0;

void test_print_once (void){
	serial_print("\nFunction called once ");
  serial_printdeclong(timer_get_mili());

}
void test_print (void){
	serial_print("\nFunction called repeatedly ");
  uint32_t ahora = timer_get_mili();
  serial_printdeclong(ahora - anterior);
  anterior = ahora;
}

int main(void) {

  serial_init();
  serial_print("\n test_timer_2.c");

  timer_init(2);

  serial_print("\n_io_ports[M6812_TMSK2]: ");
  serial_printbinbyte(_io_ports[M6812_TMSK2]);

	_io_ports[M6812_TSCR] = M6812B_TEN;

	serial_print("\nActual cycles : ");
	serial_printdeclong(timer_get_cycle());

	serial_print("\nActual time (ms): ");
	serial_printdeclong(timer_get_mili());

	serial_print("\nActual time (us): ");
	serial_printdeclong(timer_get_micro());

	serial_print("\nWaiting 2 second... ");
	timer_sleep(2000000UL);
	serial_print("\nDone waiting!");

	serial_print("\nActual time (ms): ");
	serial_printdeclong(timer_get_mili());

	serial_print("\nActual time (us): ");
	serial_printdeclong(timer_get_micro());



	serial_print("\nCalling once and repeteadly: ");

	timer_schedule_call(10000000UL, test_print_once);
	timer_repeat_call(3500000UL, test_print);

	while(1) {
    timer_sleep(2000000UL);

    serial_print("\nActual time (ms): ");
    serial_printdeclong(timer_get_mili());

  }
}
