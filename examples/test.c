#include <gpio.h>

int main (int argc, char **argv)
{
	gpio_pin pin;
	int ret = gpio_open_by_name (&pin, "test");
	if (ret) {
		fprintf (stderr, "open testpin failed\n");
		goto quit;
	}
	ret = gpio_close (&pin);
	if (ret) {
		fprintf (stderr, "close testpin failed\n");
	}
quit:
	gpio_destroy ();
	return ret;
}
