/*
 * Sistemas operativos empotrados
 * Driver para el GPIO del MC1322x
 */

#include "system.h"

/*****************************************************************************/

/**
 * Acceso estructurado a los registros de control del gpio del MC1322x
 * Estrucutra de arrays, la posición 0 -> 00-31; 1 -> 32-63
 */
typedef struct
{
	uint32_t GPIO_PAD_DIR[2];        // GPIO Pad Direction
    uint32_t GPIO_DATA[2];           // GPIO Data
	uint32_t GPIO_PAD_PU_EN[2];      // GPIO Pad Pull-up Enable
	uint32_t GPIO_FUNC_SEL[4];       // GPIO Function Select
	uint32_t GPIO_DATA_SEL[2];       // GPIO Data Select
	uint32_t GPIO_PAD_PU_SEL[2];     // GPIO Pad Pull-up Select
	uint32_t GPIO_PAD_HYST_EN[2];    // GPIO Pad Hysteresis Enable
	uint32_t GPIO_PAD_KEEP[2];       // GPIO Pad Keeper Enable
	uint32_t GPIO_DATA_SET[2];       // GPIO Data Set
	uint32_t GPIO_DATA_RESET[2];     // GPIO Data Reset
	uint32_t GPIO_PAD_DIR_SET[2];    // GPIO Pad Direction Set
	uint32_t GPIO_PAD_DIR_RESET[2];  // GPIO Pad Direction Reset
} gpio_regs_t;

static volatile gpio_regs_t* const gpio_regs = GPIO_BASE;

/*****************************************************************************/

/**
 * Fija la dirección de los pines seleccionados en la máscara como de entrada
 *
 * @param 	port 	Puerto
 * @param 	mask 	Máscara para seleccionar los pines
 * @return	gpio_no_error si los parámetros de entrada son corectos o
 *			gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_set_port_dir_input (gpio_port_t port, uint32_t mask)
{
    if(mask > 31)
        return gpio_invalid_parameter;
    if(port != gpio_port_0 && port != gpio_port_1)
        return gpio_invalid_parameter;
        
	gpio_regs->GPIO_PAD_DIR_RESET[port] = mask;

	return gpio_no_error;
}

/*****************************************************************************/

/**
 * Fija la dirección de los pines seleccionados en la máscara como de salida
 *
 * @param	port 	Puerto
 * @param	mask 	Máscara para seleccionar los pines
 * @return	gpio_no_error si los parámetros de entrada son corectos o
 *			gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_set_port_dir_output (gpio_port_t port, uint32_t mask)
{
	if(mask > 31)
        return gpio_invalid_parameter;
    if(port != gpio_port_0 && port != gpio_port_1)
        return gpio_invalid_parameter;
        
	gpio_regs->GPIO_PAD_DIR_SET[port] = mask;

	return gpio_no_error;
}

/*****************************************************************************/

/**
 * Fija la dirección del pin indicado como de entrada
 *
 * @param	pin 	Número de pin
 * @return	gpio_no_error si los parámetros de entrada son corectos o
 *			gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_set_pin_dir_input (gpio_pin_t pin)
{
    // Los pines > 31 empiezan en 1 en la 5ª posición
    if(pin == gpio_pin_max) return gpio_invalid_parameter;
	
	gpio_regs->GPIO_PAD_DIR_RESET[pin >> 5] = 1 << (pin & 0x1f);

	return gpio_no_error;
}

/*****************************************************************************/

/**
 * Fija la dirección del pin indicado como de salida
 *
 * @param	pin 	Número de pin
 * @return	gpio_no_error si los parámetros de entrada son corectos o
 *			gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_set_pin_dir_output (gpio_pin_t pin)
{
	if(pin == gpio_pin_max) return gpio_invalid_parameter;
	
	gpio_regs->GPIO_PAD_DIR_SET[pin >> 5] = 1 << (pin & 0x1f);

	return gpio_no_error;
}

/*****************************************************************************/

/**
 * Escribe unos en los pines seleccionados en la máscara
 *
 * @param	port 	Puerto
 * @param	mask 	Máscara para seleccionar los pines
 * @return	gpio_no_error si los parámetros de entrada son corectos o
 *			gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_set_port (gpio_port_t port, uint32_t mask)
{
    /*
	if(mask > 31)
        return gpio_invalid_parameter;
    if(port != gpio_port_0 && port != gpio_port_1)
        return gpio_invalid_parameter;
    */
        
    gpio_regs->GPIO_DATA_SET[port] = mask;

	return gpio_no_error;
}

/*****************************************************************************/

/**
 * Escribe ceros en los pines seleccionados en la máscara
 *
 * @param	port 	Puerto
 * @param	mask 	Máscara para seleccionar los pines
 * @return	gpio_no_error si los parámetros de entrada son corectos o
 *			gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_clear_port (gpio_port_t port, uint32_t mask)
{
	if(mask > 31)
        return gpio_invalid_parameter;
    if(port != gpio_port_0 && port != gpio_port_1)
        return gpio_invalid_parameter;
        
    gpio_regs->GPIO_DATA_RESET[port] = mask;

	return gpio_no_error;
}

/*****************************************************************************/

/**
 * Escribe un uno en el pin indicado
 *
 * @param	pin 	Número de pin
 * @return	gpio_no_error si los parámetros de entrada son corectos o
 *			gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_set_pin (gpio_pin_t pin)
{
	if(pin == gpio_pin_max) return gpio_invalid_parameter;
	
	gpio_regs->GPIO_DATA_SET[pin >> 5] = 1 << (pin & 0x1f);

	return gpio_no_error;
}

/*****************************************************************************/

/**
 * Escribe un cero en el pin indicado
 *
 * @param	pin 	Número de pin
 * @return	gpio_no_error si los parámetros de entrada son corectos o
 *			gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_clear_pin (gpio_pin_t pin)
{
	if(pin == gpio_pin_max) return gpio_invalid_parameter;
	
	gpio_regs->GPIO_DATA_RESET[pin >> 5] = 1 << (pin & 0x1f);

	return gpio_no_error;
}

/*****************************************************************************/

/**
 * Devuelve el valor de los pines
 *
 * @param	port 	Número de puerto
 * @return	valor de los pines
 */
inline uint32_t gpio_get_port (gpio_port_t port) {
    if(port != gpio_port_0 && port != gpio_port_1)
        return gpio_invalid_parameter;
    
    return gpio_regs->GPIO_DATA[port];

}

/*****************************************************************************/

/**
 * Devuelve el valor de un pin
 *
 * @param	pin 	Número de pin
 * @return	valor del pin en la posición menos significativa del entero
 */
inline uint32_t gpio_get_pin (gpio_pin_t pin) {
    uint32_t p;
    
    if(pin == gpio_pin_max) return gpio_invalid_parameter;
	
	p = gpio_regs->GPIO_DATA[pin >> 5];
	
	return (p >>= ((pin & 0x1f) - 1));
	
}

/*****************************************************************************/

/**
 * Fija los pines seleccionados a una función
 *
 * @param	port 	Puerto
 * @param	func	Función
 * @param	mask	Máscara para seleccionar los pines
 * @return	gpio_no_error si los parámetros de entrada son corectos o
 *			gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_set_port_func (gpio_port_t port, gpio_func_t func, uint32_t mask)
{
	if(port > 4)
        return gpio_invalid_parameter;
        
    //gpio_regs->GPIO_FUNC_SEL[port];

	return gpio_no_error;
}

/*****************************************************************************/

/**
 * Fija el pin seleccionado a una función
 *
 * @param	pin 	Pin
 * @param	func	Función
 * @return	gpio_no_error si los parámetros de entrada son corectos o
 *			gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_set_pin_func (gpio_pin_t pin, gpio_func_t func)
{
	gpio_port_t port;
	
	if(pin == gpio_pin_max) return gpio_invalid_parameter;
	
	if(pin > gpio_pin_48) port = gpio_port_3;
	else if(pin > gpio_pin_32) port = gpio_port_2;
	else if(pin > gpio_pin_16) port = gpio_port_1;
	else port = gpio_port_0;
	
	gpio_regs->GPIO_FUNC_SEL[port] = (func << ((pin >> 5) - 1));

	return gpio_no_error;
}

/*****************************************************************************/
