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

/* Dirección del registro de control de dirección del GPIO 0-GPIO 31 */
volatile uint32_t * const reg_gpio_pad_dir0    = (uint32_t *) 0x80000000;

/* Dirección del registro de control de dirección del GPIO32-GPIO63 */
volatile uint32_t * const reg_gpio_pad_dir1    = (uint32_t *) 0x80000004;

/* Dirección del registro para consultar el estado de los pulsadores
   GPIO DATA 00-31 */
volatile uint32_t * const reg_gpio_data0       = (uint32_t *) 0x80000008;

/* Dirección del registro de activación de los botones GPIO 00-31 */
volatile uint32_t * const reg_gpio_data_set0   = (uint32_t *) 0x80000048;

/* Dirección del registro de activación de bits del GPIO32-GPIO63 */
// Para los LEDs
volatile uint32_t * const reg_gpio_data_set1   = (uint32_t *) 0x8000004c;

/* Dirección del registro de limpieza de bits del GPIO32-GPIO63 */
volatile uint32_t * const reg_gpio_data_reset1 = (uint32_t *) 0x80000054;

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
	/* Configuramos el GPIO44,45 para que sea de salida */
	gpio_set_port_dir_output(gpio_port_1, led_green_mask | led_red_mask);
	
	// Configuramos el GPIO22,23
	gpio_set_port_dir_output(gpio_port_0, btn_22_ou | btn_23_ou);
	gpio_set_port(gpio_port_0, btn_22_ou | btn_23_ou);
	
}

/*****************************************************************************/

/*
 * Enciende los leds indicados en la máscara
 * @param mask Máscara para seleccionar leds
 */
void leds_on (uint32_t mask)
{
	/* Encendemos los leds indicados en la máscara */
	//err = gpio_set_port(gpio_port_0, mask);
	*reg_gpio_data_set1 = *reg_gpio_data_set1 | mask;
	//gpio_set_port(gpio_port_1, mask);
}

/*****************************************************************************/

/*
 * Apaga los leds indicados en la máscara
 * @param mask Máscara para seleccionar leds
 */
void leds_off (uint32_t mask)
{
	/* Apagamos los leds indicados en la máscara */
	*reg_gpio_data_reset1 = mask;
	//gpio_clear_port(gpio_port_1, mask);
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
    
    //itc_force_interrupt(itc_src_asm);
    
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

