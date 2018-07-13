#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <errno.h>
#include <libconfig.h>
#include <gpio.h>

static config_t cfg = {
	.root = NULL,
};

static int init_cfg (void)
{
	int ret = CONFIG_TRUE;

	if (cfg.root)
		return ret;

	config_init (&cfg);

	ret = config_read_file (&cfg, CFG_FILE);
	if (ret == CONFIG_FALSE) {
		fprintf (stderr, "setup libconfig failed: %s\n", config_error_text (&cfg));
		fprintf (stderr, "probably: %s doesn't exist\n", CFG_FILE);
		config_destroy (&cfg);
	}

	return ret;
}

void gpio_destroy (void)
{
	config_destroy (&cfg);
}

static int get_pin (const char *name)
{
	int no;
	int ret = init_cfg ();

	if (ret == CONFIG_FALSE)
		return -EINVAL;

	ret = config_lookup_int (&cfg, name, &no);
	if (ret == CONFIG_FALSE) {
		fprintf (stderr, "%s not found in %s\n", name, CFG_FILE);
		return -EINVAL;
	}

	return (int) no;
}

int gpio_open_by_name (gpio_pin *pin, const char *name)
{
	int ret = get_pin (name);

	if (ret > 0)
		return gpio_open (pin, ret);

	return ret;
}

int gpio_open_by_name_dir (gpio_pin *pin, const char *name, gpio_direction dir)
{
	int ret = get_pin (name);

	if (ret > 0)
		return gpio_open_dir (pin, ret, dir);

	return ret;
}
