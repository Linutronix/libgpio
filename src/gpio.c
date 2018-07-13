/* libgpio
 *
 * Copyright 2012 Manuel Traut <manut@linutronix.de>
 *
 * LGPL licensed
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <gpio.h>

#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>

#define SYSFS     "/sys/class/gpio/"
#define EXPORT    SYSFS"export"
#define UNEXPORT  SYSFS"unexport"
#define STR_LEN   64

#ifdef DEBUG
#define debug(...) printf (__VA_ARGS__);
#else
#define debug(...) ;
#endif

static int export_un_export (gpio_pin *pin, int export)
{
	char c[STR_LEN];
	ssize_t len;
	int ret = 0;

	int fd;

	if (export)
		fd = open (EXPORT, O_WRONLY);
	else
		fd = open (UNEXPORT, O_WRONLY);

	if (fd == -1)
	{
		ret = errno;
		if (export)
			fprintf (stderr, "open %s failed: %d\n", EXPORT, ret);
		else
			fprintf (stderr, "open %s failed: %d\n", UNEXPORT, ret);
		pin->valid = GPIO_INVALID;
		return -ret;
	}

	ret = snprintf (c, STR_LEN, "%d", pin->no);

	if (ret < 0)
	{
		fprintf (stderr, "gpio no %d cannot be converted into a string", pin->no);
		pin->valid = GPIO_INVALID;
		goto close_out;
	}

	len = write (fd, c, ret);

	if (len != ret)
	{
		if (export)
			fprintf (stderr, "export gpio no %d failed: %d\n", pin->no, errno);
		else
			fprintf (stderr, "unexport gpio no %d failed: %d\n", pin->no, errno);

		ret = -errno;
		pin->valid = GPIO_INVALID;
		goto close_out;
	}

	ret = 0;

	pin->valid = GPIO_VALID;

close_out:
	close (fd);
	return ret;
}

inline static int export(gpio_pin *pin)
{
	return export_un_export (pin, 1);
}

inline static int unexport(gpio_pin *pin)
{
	return export_un_export (pin, 0);
}

static int get_direction (gpio_pin *pin)
{
	char c[STR_LEN];
	char dir;
	int fd = 0;
	int ret = 0;
	ssize_t len = 0;

	ret = snprintf (c, STR_LEN, SYSFS"gpio%d/direction", pin->no);

	if (ret < 0)
	{
		fprintf (stderr, "cannot create dir sysfs string for gpio %d\n", pin->no);
		return ret;
	}

	ret = 0;

	fd = open (c, O_RDONLY);

	if (fd == -1)
	{
		ret = errno;
		fprintf (stderr, "open %s failed: %d\n", c, ret);
		return -ret;
	}

	len = read (fd, &dir, 1);
	if (len != 1) {
		ret = errno;
		fprintf (stderr, "read direction from %s failed: %d\n", c, ret);
		return -ret;
	}

	switch (dir)
	{
		case 'i':
			pin->direction = GPIO_IN;
			break;
		case 'o':
			pin->direction = GPIO_OUT;
			break;
		default:
			ret = -errno;
			fprintf (stderr, "get direction of gpio %d failed\n", pin->no);
			goto close_out;
	}

close_out:
	close (fd);
	return ret;
}

static int open_value_fd (gpio_pin *pin)
{
	char c[STR_LEN];
	int ret = 0;

	ret = snprintf (c, STR_LEN, SYSFS"gpio%d/value", pin->no);

	if (ret < 0)
	{
		fprintf (stderr, "cannot create value sysfs string for gpio %d\n", pin->no);
		return ret;
	}

	ret = 0;

	switch (pin->direction)
	{
		case GPIO_OUT:
			pin->fd = open (c, O_RDWR);
			break;
		case GPIO_IN:
			pin->fd = open (c, O_RDONLY);
			break;
		default:
			fprintf (stderr, "gpio %d direction not set\n", pin->no);
			return -EINVAL;
	}

	if (pin->fd < 0)
	{
		ret = -errno;
		fprintf (stderr, "gpio %d open value fd failed: %d\n", pin->no, ret);
	}

	return ret;
}

static int direction (gpio_pin *pin, char *dir)
{
	char c[STR_LEN];
	int fd, ret = 0;

	ret = snprintf (c, STR_LEN, SYSFS"gpio%d/direction", pin->no);

	if (ret < 0)
	{
		fprintf (stderr, "cannot create dir sysfs string for gpio %d\n", pin->no);
		pin->valid = GPIO_INVALID;
		return ret;
	}

	fd = open (c, O_RDWR);

	if (fd < 0)
	{
		ret = -errno;
		fprintf (stderr, "gpio %d open dir fd failed: %d\n", pin->no, ret);
		pin->valid = GPIO_INVALID;
		return ret;
	}

	ret = write (fd, dir, strnlen (dir, 3));

	/*
	 * strnlen (dir, 3) <= 3 < SSIZE_MAX
	 * cast to ssize_t to avoid a annoying warning from the compiler:
	 * gpio.c:217:10: warning: comparison between signed and unsigned
	 * integer expressions [-Wsign-compare]
	 */
	if (ret != (ssize_t) strnlen (dir, 3))
	{
		ret = -errno;
		fprintf (stderr, "set direction: %s on gpio %d failed: %d\n",
			dir, pin->no, ret);
		pin->valid = GPIO_INVALID;
		goto close_out;
	}

	ret = 0;

close_out:
	close (fd);

	return ret;
}

static int set_irq (gpio_pin *pin, gpio_irq_mode m)
{
	char c[STR_LEN];
	int fd, ret = 0;

	ret = snprintf (c, STR_LEN, SYSFS"gpio%d/edge", pin->no);

	if (ret < 0)
	{
		fprintf (stderr, "cannot create edge sysfs string for gpio %d\n", pin->no);
		return ret;
	}

	fd = open (c, O_RDWR);

	if (fd < 0)
	{
		ret = -errno;
		fprintf (stderr, "gpio %d open edge fd failed: %d\n", pin->no, ret);
		return ret;
	}

	switch (m)
	{
		case GPIO_RISING:
			ret = write (fd, "rising", 6);
			if (ret != 6)
				ret = -1;
			break;
		case GPIO_FALLING:
			ret = write (fd, "falling", 7);
			if (ret != 7)
				ret = -1;
			break;
		case GPIO_BOTH:
			ret = write (fd, "both", 4);
			if (ret != 4)
				ret = -1;
			break;
		case GPIO_NONE:
			ret = write (fd, "none", 4);
			if (ret != 4)
				ret = -1;
			break;
		default:
			fprintf (stderr, "unknown irq mode\n");
			return -1;
	}

	if (ret == -1)
	{
		ret = -errno;
		fprintf (stderr, "set edge on gpio %d failed: %d\n", pin->no, ret);
		goto close_out;
	}

	pin->irq_mode = m;
	ret = 0;

close_out:
	close (fd);

	return ret;

}

int gpio_out (gpio_pin *pin)
{
	int ret = 0;

	debug ("%s: %d\n", __func__, pin->no);

	ret = direction (pin, "out\0");

	if (!ret)
		pin->direction = GPIO_OUT;

	return ret;
}

int gpio_in (gpio_pin *pin)
{
	int ret = 0;

	debug ("%s: %d\n", __func__, pin->no);

	ret = direction (pin, "in\0");

	if (!ret)
		pin->direction = GPIO_IN;

	return ret;
}

int gpio_set_value (gpio_pin *pin, gpio_value value)
{
	int ret = 0;
	char c;

	debug ("%s: %d = %d\n", __func__, pin->no, value);

	if (pin->direction != GPIO_OUT)
	{
		fprintf (stderr, "%s gpio %d is configured as input, can't write value\n",
			__func__, pin->no);
		return -EINVAL;
	}

	if (pin->valid != GPIO_VALID)
	{
		fprintf (stderr, "%s gpio %d is INVALID - maybe it is uninitialized?\n",
			__func__, pin->no);
		return -ENOMEM;
	}

	if (pin->fd == -1)
		ret = open_value_fd (pin);

	if (ret)
	{
		fprintf (stderr, "%s can't access gpio %d\n", __func__, pin->no);
		return ret;
	}

	switch (value)
	{
		case GPIO_LOW:
			c = '0';
			break;
		case GPIO_HIGH:
			c = '1';
			break;
		default:
			fprintf (stderr, "%s value %d of gpio %d unknown\n", __func__,
				value, pin->no);
			return -EINVAL;
	}

	ret = write (pin->fd, &c, 1);

	if (ret != 1)
	{
		ret = -errno;
		fprintf (stderr, "%s write value of gpio %d failed %d\n", __func__,
			pin->no, ret);

		close (pin->fd);
		pin->fd = -1;
		return ret;
	}

	return 0;
}

int gpio_get_value (gpio_pin *pin, gpio_value *value)
{
	int ret = 0;
	char c;

	debug ("%s: %d\n", __func__, pin->no);

	if (pin->valid != GPIO_VALID)
	{
		fprintf (stderr, "%s gpio %d is INVALID - maybe it is uninitialized?\n",
			__func__, pin->no);
		return -ENOMEM;
	}

	if (pin->fd == -1)
		ret = open_value_fd (pin);

	if (ret)
	{
		fprintf (stderr, "%s can't access gpio %d\n", __func__, pin->no);
		return ret;
	}

	ret = lseek (pin->fd, 0x00, SEEK_SET);

	if (ret == -1)
	{
		ret = -errno;
		fprintf (stderr, "%s rewind of gpio %d failed %d\n", __func__,
			pin->no, ret);

		close (pin->fd);
		pin->fd = -1;
		return ret;
	}

	ret = read (pin->fd, &c, 1);

	if (ret != 1)
	{
		ret = -errno;
		fprintf (stderr, "%s read value of gpio %d failed %d\n", __func__,
			pin->no, ret);

		close (pin->fd);
		pin->fd = -1;
		return ret;
	}

	ret = 0;

	switch (c)
	{
		case '0':
			*value = GPIO_LOW;
			break;
		case '1':
			*value = GPIO_HIGH;
			break;
		default:
			fprintf (stderr, "%s value %x of gpio %d unknown\n", __func__,
				c, pin->no);
			return -EINVAL;
	}

	return ret;
}

int gpio_open (gpio_pin *pin, unsigned int no)
{
	int ret = 0;

	debug ("%s: %d\n", __func__, no);

	pin->no = no;
	pin->fd = -1;

	ret = export (pin);

	if (ret < 0 ) {
		pin->valid = GPIO_INVALID;
		return ret;
	}

	ret = get_direction (pin);
	if (ret)
		pin->valid = GPIO_INVALID;
	else
		pin->valid = GPIO_VALID;

	return ret;
}

int gpio_open_dir (gpio_pin *pin, unsigned int no, gpio_direction dir)
{
	int ret = 0;

	debug ("%s: %d - %d\n", __func__, no, dir);

	ret = gpio_open (pin, no);

	if (ret)
		return ret;

	switch (dir)
	{
		case GPIO_OUT:
			ret = gpio_out (pin);
			break;
		case GPIO_IN:
			ret = gpio_in (pin);
			break;
		default:
			fprintf (stderr, "%s - instatus direction: %d\n", __func__, dir);
			ret = -EINVAL;
	}

	return ret;
}

int gpio_close (gpio_pin *pin)
{
	int ret = 0;

	debug ("%s: %d\n", __func__, pin->no);

	if (pin->fd != -1)
	{
		close (pin->fd);
		pin->fd = -1;
	}

	ret = unexport (pin);

	return ret;
}

int gpio_enable_irq (gpio_pin *pin, gpio_irq_mode m)
{
	debug ("%s: %d\n", __func__, pin->no);

	/* outputs can't be polled */
	if (pin->direction == GPIO_OUT)
		return -1;

	if (pin->valid != GPIO_VALID)
		return -ENOMEM;

	return set_irq (pin, m);
}

int gpio_get_fd (gpio_pin *pin)
{
	if (pin->fd == -1)
		open_value_fd (pin);

	return pin->fd;
}

int gpio_irq_timed_wait (gpio_pin *pin, gpio_value *value, int timeout_ms)
{
	int ret = 0;
	struct pollfd irqdesc = {
		.events = POLLPRI | POLLERR ,
	};

	if (pin->valid != GPIO_VALID) {
		return -ENOMEM;
	}

	if (pin->fd == -1)
		ret = open_value_fd (pin);

	if (ret)
	{
		fprintf (stderr, "%s can't access gpio %d\n", __func__, pin->no);
		return ret;
	}

	irqdesc.fd = pin->fd;

	ret = poll (&irqdesc, 1, timeout_ms);

	if (ret == -1)
	{
		ret = -errno;
		close (pin->fd);
		pin->fd = -1;
		fprintf (stderr, "wait for irq failed: %d\n", ret);
		return ret;
	}

	/* timeout */
	if (ret == 0)
		return 1;

	return gpio_get_value (pin, value);
}

int gpio_irq_wait (gpio_pin *pin, gpio_value *value)
{
	return gpio_irq_timed_wait (pin, value, -1);
}
