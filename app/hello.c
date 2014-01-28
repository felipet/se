/*****************************************************************************/
/*                                                                           */
/* Sistemas Empotrados                                                       */
/* Ejercicio 2 de la práctica 5                                              */
/*                                                                           */
/*****************************************************************************/

#include <stdint.h>
#include <system.h>
#include "gpio.h"

/*
 * Constantes relativas a la plataforma
 */

// El led rojo está en el GPIO 44 (el bit 12 de los registros GPIO_X_1) */
uint32_t const led_red_mask     = (1 << (44-32));
// El led verde está en el GPIO 45
uint32_t const led_green_mask   = (1 << (45-32));

// Máscaras para los botones
uint32_t const btn_22_ou        = 0x00400000;
uint32_t const btn_23_ou        = 0x00800000;
uint32_t const btn_26_in        = 0x04000000;
uint32_t const btn_27_in        = 0x08000000;

/*
 * Constantes relativas a la aplicacion
 */
uint32_t const delay = 0x10000;

/*
 * Máscara del led que se hará parpadear
 */
uint32_t the_led;
 
/*****************************************************************************/

/*
 * Inicialización de los pines de E/S
 */
void gpio_init(void)
{   
    gpio_err_t err;
	/* Configuramos el GPIO44,45 para que sea de salida */
	err = gpio_set_port_dir_output(gpio_port_1, led_red_mask | led_green_mask);
	
	// Configuramos el GPIO22,23
	err = gpio_set_port_dir_output(gpio_port_0, btn_22_ou | btn_23_ou);
	err = gpio_set_port(gpio_port_0, btn_22_ou | btn_23_ou);
}

/*****************************************************************************/

/*
 * Enciende los leds indicados en la máscara
 * @param mask Máscara para seleccionar leds
 */
void leds_on (uint32_t mask)
{
    gpio_err_t err;
	/* Encendemos los leds indicados en la máscara */
	err = gpio_set_port(gpio_port_1, mask);
	
}

/*****************************************************************************/

/*
 * Apaga los leds indicados en la máscara
 * @param mask Máscara para seleccionar leds
 */
void leds_off (uint32_t mask)
{
    gpio_err_t err;
	
	/* Apagamos los leds indicados en la máscara */
	err = gpio_clear_port(gpio_port_1, mask);
}

/*****************************************************************************/

/*
 * Retardo para el parpedeo
 */
void pause(void)
{
    uint32_t i;
	for (i=0 ; i<delay ; i++);
}

/*****************************************************************************/

void asm_isr(void)
{
    itc_unforce_interrupt(itc_src_asm);
    leds_on(led_green_mask);
}

/*****************************************************************************/

typedef enum {green, red} led;


/*
 * Programa principal
 */
int main ()
{
    uint32_t bits, pulsados;
    led bombillita = red;
    
    // Fijar el manejador para las IRQ
    excep_set_handler(excep_irq, excep_nonnested_irq_handler);
    itc_set_handler(itc_src_asm, asm_isr);
    itc_enable_interrupt(itc_src_asm);
      
    // Inicialización del GPIO dentro de una sección crítica
    bits = excep_disable_ints();
	gpio_init();
	excep_restore_ints(bits);
    
    itc_force_interrupt(itc_src_asm);
    
	while (1)
	{
	    pulsados = gpio_get_port(gpio_port_0);
	    
	    if(pulsados & btn_26_in)
	        bombillita = green;
        else if(pulsados & btn_27_in)
                bombillita = red;
                
        the_led = bombillita == green? led_green_mask : led_red_mask;
        
		leds_on(the_led);
                pause();

		leds_off(the_led);
                pause();
	}

        return 0;
}

/*****************************************************************************/

