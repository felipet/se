@
@ Sistemas Empotrados
@ Felipe Torres
@ El "hola mundo2" en la Redwire EconoTAG
@

@
@ Constantes
@
	    @ Registro de control de dirección del GPIO0-31
	    .set GPIO_PAD_DIR0,    0x80000000		
        @ Registro de control de dirección del GPIO32-63
        .set GPIO_PAD_DIR1,    0x80000004

        @ Registro para consultar el estado de los pulsadores
        @ GPIO DATA 00-31
        .set GPIO_DATA0,       0x80000008

        @ Registro de activación de btis del GPIO00-31
        @ Inicializar los GPIO de los botones
        .set GPIO_DATA_SET0,   0x80000048
        @ Registro de activación de bits del GPIO32-GPIO63
        @ Para encender los leds
        .set GPIO_DATA_SET1,   0x8000004c

        @ Registro de limpieza de bits del GPIO32-GPIO63
        .set GPIO_DATA_RESET1, 0x80000054

        @ El led rojo está en el GPIO 44
        .set LED_RED_MASK,     0x00001000
        @ El led verde está en el GPIO 45
        .set LED_GREEN_MASK,   0x00002000

        @ Retardo para el parpadeo
        .set DELAY,            0x00200000

@
@ Punto de entrada
@

        .code 32
        .text
        .global _start
        .type   _start, %function

_start:
gpio_init:
        @ Configuración de los pines
        @ Configuramos el GPIO44 para que sea de salida
        ldr     r4, =GPIO_PAD_DIR1
        ldr     r5, =LED_RED_MASK
        str     r5, [r4]

        ldr     r5, =LED_GREEN_MASK
        str     r5, [r4]

        @ Direcciones de los registros GPIO_DATA_SET1 y GPIO_DATA_RESET1
        ldr     r6, =GPIO_DATA_SET1
        ldr     r7, =GPIO_DATA_RESET1

loop:
        @ Encendemos el led
        str     r5, [r6]

        @ Pausa corta
        ldr     r0, =DELAY
        bl      pause

        @ Apagamos el led
        str     r5, [r7]

        @ Pausa corta
        ldr     r0, =DELAY
        bl      pause

        @ Bucle infinito
        b       loop
        
@
@ Función que produce un retardo
@ r0: iteraciones del retardo
@
        .type   pause, %function
pause:
        subs    r0, r0, #1
        bne     pause
        mov     pc, lr

