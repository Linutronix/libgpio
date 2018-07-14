/* libgpio
 *
 * Copyright 2012 Manuel Traut <manut@linutronix.de>
 *
 * LGPL licensed
 */

#ifndef _LIB_GPIO_H_
#define _LIB_GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <libconfig.h>

/**
 * @defgroup libgpio_public_decl libgpio public available types and declarations
 * @ingroup libgpio
 * @brief public types and declarations
 * @{
 */

/**
 * @enum _gpio_direction GPIO direction
 */
typedef enum _gpio_direction {
	GPIO_IN,		/**< GPIO acts as input pin */
	GPIO_OUT,		/**< GPIO acts as ouput pin */
} gpio_direction;

/**
 * @enum _gpio_value GPIO status
 */
typedef enum _gpio_value {
	GPIO_LOW = 0,		/**< value of GPIO pin is low */
	GPIO_HIGH = 1,		/**< value of GPIO pin is high */
} gpio_value;

/**
 * @enum _gpio_irq_mode GPIO interrupt mode
 */
typedef enum _gpio_irq_mode {
	GPIO_NONE,		/**< raise no interrupt */
	GPIO_RISING,		/**< raise interrupt on rising edge */
	GPIO_FALLING,		/**< raise interrupt on falling edge */
	GPIO_BOTH,		/**< raise interrupt on both edges */
} gpio_irq_mode;

/**
 * @enum _gpio_status libgpio GPIO status
*/
typedef enum _gpio_status {
	GPIO_INVALID = 0,	/**< use of GPIO is invalid */
	GPIO_VALID = 1,		/**< GPIO can be used by libgpio */
} gpio_status;

/**
 * @struct _gpio_pin
 * libgpio internal GPIO representation
 * @todo remove expose of internal representation to the public
 * @deprecated Do not use any struct member directly. The internal
 * structure is surely to change in future versions of the library.
 * Please use the accessor functions instead.
*/
typedef struct _gpio_pin {
	unsigned int no;		/**< GPIO number */
	gpio_direction direction;	/**< GPIO direction */
	gpio_irq_mode irq_mode;		/**< GPIO interrupt mode */
	int fd;				/**< GPIO interrupt notification fd */
	gpio_status valid;		/**< internal GPIO status */
} gpio_pin;

/** @} */

int gpio_open (gpio_pin *pin, unsigned int no);
int gpio_open_by_name (gpio_pin *pin, const char *name);
int gpio_open_dir (gpio_pin *pin, unsigned int no, gpio_direction dir);
int gpio_open_by_name_dir (gpio_pin *pin, const char *name, gpio_direction dir);

int gpio_close (gpio_pin *pin);
void gpio_destroy (void);

int gpio_out (gpio_pin *pin);
int gpio_in (gpio_pin *pin);

int gpio_set_value (gpio_pin *pin, gpio_value value);
int gpio_get_value (gpio_pin *pin, gpio_value *value);

int gpio_enable_irq (gpio_pin *pin, gpio_irq_mode m);
int gpio_irq_wait (gpio_pin *pin, gpio_value *value);
int gpio_irq_timed_wait (gpio_pin *pin, gpio_value *value, int timeout_ms);

int gpio_get_fd (gpio_pin *pin);

#ifdef __cplusplus
}
#endif

#endif
