#include <timer.h>

void test_print_once (void){
	serial_print("\nFunction called once");
}
void test_print (void){
	serial_print("\nFunction called repeatedly");
}

int main(void) {

  timer_init(3);

	serial_print("\nActual cycles : ");
	serial_printdeclong(timer_get_cycle());

	serial_print("\nActual time (ms): ");
	serial_printdeclong(timer_get_mili());

	serial_print("\nActual time (us): ");
	serial_printdeclong(timer_get_micro());

	serial_print("\nWaiting 2 second... ");
	timer_sleep(2000000);
	serial_print("\nDone waiting!");

	serial_print("\nActual time (ms): ");
	serial_printdeclong(timer_get_mili());

	serial_print("\nActual time (us): ");
	serial_printdeclong(timer_get_micro());



	serial_print("\nCalling once and repeteadly: ");

	timer_schedule_call(3000000, test_print_once);
	timer_repeat_call(3000000, test_print);

	while(1);
}
