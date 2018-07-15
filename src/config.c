// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * Copyright (C) 2013 Linutronix GmbH
 * Author: Manuel Traut <manut@linutronix.de>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <errno.h>
#include <libconfig.h>
#include <gpio.h>

static config_t cfg = {
	.root = NULL,
};

static int init_cfg (char *cfgname)
{
	int ret = CONFIG_TRUE;

	if (cfg.root) {
		if (cfgname) {
			fprintf (stderr, "libconfig already configured\n");
			ret = CONFIG_FALSE;
		}
		goto out;
	}

	config_init (&cfg);
	if (!cfgname)
		cfgname = CFG_FILE;

	ret = config_read_file (&cfg, cfgname);
	if (ret == CONFIG_FALSE) {
		fprintf (stderr, "setup libconfig failed: %s\n", config_error_text (&cfg));
		fprintf (stderr, "probably: %s doesn't exist\n", cfgname);
		config_destroy (&cfg);
	}
out:
	return ret;
}

/**
 * @ingroup libgpio_public
 * optional init of all internal structures of libgpio with given config
 * @param cfgname path to libgpio configuration file or NULL for default
 * @return 0 on success or -EINVAL on failure
 */
int gpio_init (char *cfgname)
{
	return (init_cfg (cfgname) == CONFIG_TRUE) ? 0 : -EINVAL;
}

/**
 * @ingroup libgpio_public
 * release all internal structures of libgpio
 */
void gpio_destroy (void)
{
	config_destroy (&cfg);
}

static int get_pin (const char *name)
{
	int no;
	int ret = init_cfg (NULL);

	if (ret == CONFIG_FALSE)
		return -EINVAL;

	ret = config_lookup_int (&cfg, name, &no);
	if (ret == CONFIG_FALSE) {
		fprintf (stderr, "%s not found in %s\n", name, CFG_FILE);
		return -EINVAL;
	}

	return (int) no;
}

/**
 * @ingroup libgpio_public
 * acquire a GPIO pin by name for use
 * @param pin GPIO pointer to GPIO pin info struct
 * @param name GPIO pin name
 * @returns 0 on success or errno on failure
 */
int gpio_open_by_name (gpio_pin *pin, const char *name)
{
	int ret = get_pin (name);

	if (ret > 0)
		return gpio_open (pin, ret);

	return ret;
}

/**
 * @ingroup libgpio_public
 * acquire a GPIO pin by name for use and set pin direction
 * @param pin GPIO pointer to GPIO pin info struct
 * @param name GPIO pin name
 * @param dir GPIO pin direction
 * @returns 0 on success or errno on failure
 */
int gpio_open_by_name_dir (gpio_pin *pin, const char *name, gpio_direction dir)
{
	int ret = get_pin (name);

	if (ret > 0)
		return gpio_open_dir (pin, ret, dir);

	return ret;
}
