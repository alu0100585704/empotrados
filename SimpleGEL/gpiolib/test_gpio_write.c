
#include <types.h>
#include <sys/sio.h>
#include <gpio.h>


int main() {
  serial_init();
	gpio_pup_disable_(M6812_PORTG);

	gpio_set_output_all_reg(SET_PIN_G);

  serial_print("\n_io_ports[M6812_DDRG]:");
  serial_printbinbyte(_io_ports[M6812_DDRG]);

	gpio_write_pin(M6812_PORTG, 0, 1);
	gpio_write_pin(M6812_PORTG, 1, 0);
	gpio_write_pin(M6812_PORTG, 2, 1);
	gpio_write_pin(M6812_PORTG, 3, 0);
	gpio_write_pin(M6812_PORTG, 4, 1);
	gpio_write_pin(M6812_PORTG, 5, 0);
	gpio_write_pin(M6812_PORTG, 6, 1);
	gpio_write_pin(M6812_PORTG, 7, 0);

	serial_print("\n_io_ports[M6812_PORTG]:");
	serial_printbinbyte(_io_ports[M6812_PORTG]);

  while(1);
}
