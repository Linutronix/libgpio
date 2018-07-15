// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2013 Linutronix GmbH
 * Author: Benedikt Spranger <b.spranger@linutronix.de>
 */

#include <assert.h>
#include <errno.h>
#include <gpio.h>

int main (int argc __attribute__((unused)), char **argv __attribute__((unused)))
{
	int ret;

	/* try to open invalid config file */
	ret = gpio_init ("invalid");
	assert (ret == -EINVAL);

	/* try to open valid config file */
	ret = gpio_init (TESTCFG);
	assert (ret == 0);

	/* try to reopen valid config file */
	ret = gpio_init (TESTCFG);
	assert (ret == -EINVAL);

	return 0;
}
