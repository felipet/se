/*****************************************************************************/
/*                                                                           */
/* Sistemas Empotrados                                                       */
/* Ejercicio 2 de la práctica 5                                              */
/*                                                                           */
/*****************************************************************************/

#include <stdint.h>

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
 
/*****************************************************************************/

/*
 * Inicialización de los pines de E/S
 */
void gpio_init(void)
{
	/* Configuramos el GPIO44,45 para que sea de salida */
	*reg_gpio_pad_dir1 = *reg_gpio_pad_dir1 | led_red_mask | led_green_mask;
	
	// Configuramos el GPIO22,23
	*reg_gpio_pad_dir0 = *reg_gpio_pad_dir0 | btn_22_ou | btn_23_ou;
	
	*reg_gpio_data_set0 = *reg_gpio_data_set0 | btn_22_ou | btn_23_ou;
}

/*****************************************************************************/

/*
 * Enciende los leds indicados en la máscara
 * @param mask Máscara para seleccionar leds
 */
void leds_on (uint32_t mask)
{
	/* Encendemos los leds indicados en la máscara */
	*reg_gpio_data_set1 = *reg_gpio_data_set1 | mask;
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

/*
 * Máscara del led que se hará parpadear
 */
uint32_t the_led;

/*
 * Programa principal
 */
int main ()
{
	gpio_init();

    typedef enum {green, red} led;
    led bombillita = red;
    uint32_t pulsados;

	while (1)
	{
	    pulsados = *reg_gpio_data0;
	    
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

