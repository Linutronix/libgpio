#include <gpio.h>
#include <errno.h>
#include <pthread.h>

static int pin1_cb(struct _gpio_pin * gp)
{
	if (!gp)
		return -1;
		
	printf("Kikou lol!!!!\n");
	
	return 0;
}

int main (int argc, char **argv)
{
	gpio_pin pin1, pin2;
	int ret,cnt;
	struct gpio_irq * gi = NULL;
	
	ret = gpio_open_by_name (&pin1, "zoomi");
	if (ret) {
		fprintf (stderr, "open testpin failed\n");
		goto quit;
	}

	ret = gpio_open_by_name (&pin2, "zoomo");
	if (ret) {
		fprintf (stderr, "open testpin failed\n");
		goto quit;
	}
	
	gi = gpio_irq_init(10);
	if (!gi) {
		fprintf (stderr, "gpio_irq_init failed\n");
		goto quit;
	}
	
	gpio_enable_irq_callback (&pin1, GPIO_FALLING, pin1_cb, NULL);
	gpio_enable_irq_callback (&pin2, GPIO_FALLING, NULL, NULL);
	gpio_irq_add(gi, &pin1);
	gpio_irq_add(gi, &pin2);
	
	gpio_irq_start_loop(gi);
	
	for (cnt = 0 ; cnt < 10 ; cnt++) {
		sleep(10);
		printf("Ten other second\n");
	}
		
	ret = gpio_close (&pin1);
	ret = gpio_close (&pin2);
quit:
	gpio_destroy ();
	return ret;
}
