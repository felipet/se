/*
 * Sistemas operativos empotrados
 * Gestión de excepciones del MC1322x
 */

#include "system.h"

/*****************************************************************************/

/**
 * Tabla de manejadores
 */
extern volatile excep_handler_t _excep_handlers[excep_max];

/*****************************************************************************/

/**
 * Inicializa los manejadores de excepción
 */
void excep_init ()
{
	excep_set_handler (excep_irq, excep_nonnested_irq_handler);
}

/*****************************************************************************/

/**
 * Deshabilita todas las interrupciones
 * @return	El valor de los bits I y F antes de deshabilitar las interrupciones:
 * 			0: I=0, F=0	(IRQ habilitadas,    FIQ habilitadas)
 * 			1: I=0, F=1	(IRQ habilitadas,    FIQ deshabilitadas)
 * 			2: I=1, F=0	(IRQ deshabilitadas, FIQ habilitadas)
 * 			3: I=1, F=1	(IRQ deshabilitadas, FIQ deshabilitadas)
 */
inline uint32_t excep_disable_ints ()
{
    uint32_t if_bits;
    
	asm(
	        "mrs %[status], cpsr\n\t"           // Leer el registro de estado
	        "orr r12, %[status], #0xC0\n\t"  // Coger los bits I,F
	        "msr cpsr_c, r12\n\t"    
	        : [status] "=r" (if_bits)           // Parámetros salida
            :                                   // Parámetros entrada
	        : "r12", "cc"                              // Preservar
    );
	return (if_bits >> 6) & 3;
}

/*****************************************************************************/

/**
 * Deshabilita las interrupciones normales
 * @return	El valor del bit I antes de deshabilitar las interrupciones:
 * 			0: I=0	(IRQ habilitadas)
 * 			1: I=1	(IRQ deshabilitadas)
 */
inline uint32_t excep_disable_irq ()
{
	uint32_t i_bit;
	
	asm volatile(
	        "mrs %[status], cpsr\n\t"           // Leer el registro de estado
	        "orr r12, %[status], #0x80\n\t"  // Coger el bit I
	        "msr cpsr_c, r12\n\t"    
	        : [status] "=r" (i_bit)             // Parámetros salida
	        :                                   // Parámetros entrada
	        : "r12", "cc"                              // Preservar
	);
	
	return (i_bit >> 7) & 1;
}

/*****************************************************************************/

/**
 * Deshabilita las interrupciones rápidas
 * @return	El valor del bit F antes de deshabilitar las interrupciones:
 * 			0: F=0	(FIQ habilitadas)
 * 			1: F=1	(FIQ deshabilitadas)
 */
inline uint32_t excep_disable_fiq ()
{
	uint32_t f_bit;
	
	asm volatile(
	        "mrs %[status], cpsr\n\t"           // Leer el registro de estado
	        "orr r12, %[status], #0x40\n\t"  // Coger el bit F
	        "msr cpsr_c, r12\n\t"    
	        : [status] "=r" (f_bit)             // Parámetros salida
	        :                                   // Parámetros entrada
	        : "r12", "cc"                              // Preservar
	);
	
	return (f_bit >> 6) & 1;
}

/*****************************************************************************/

/**
 * Restaura los antiguos valores de las máscaras de interrupción
 * @param if_bits	Valores anteriores de las máscaras
 * 						0: I=0, F=0	(IRQ habilitadas,    FIQ habilitadas)
 *			 			1: I=0, F=1	(IRQ habilitadas,    FIQ deshabilitadas)
 * 						2: I=1, F=0	(IRQ deshabilitadas, FIQ habilitadas)
 *			 			3: I=1, F=1	(IRQ deshabilitadas, FIQ deshabilitadas)
 */
inline void excep_restore_ints (uint32_t if_bits)
{
	asm volatile(
            "mrs r12, cpsr\n\t"          // aux <- cpsr
            "bic r12, r12, #0x0C\n\t" // Limpiamos los bits I,F
            "orr r12, r12, %[bits], LSL #6\n\t" // Restaurar los bits
            "msr cpsr_c, r12"
            :
            : [bits] "r" (if_bits & 3)  // Se le hace un & por si hay basura
            : "r12", "cc"
	);
}

/*****************************************************************************/

/**
 * Restaura el antiguo valor de la máscara de interrupciones normales
 * @param i_bit	Valor anterior de la máscara
 * 						0: I=0	(IRQ habilitadas)
 * 						1: I=1	(IRQ deshabilitadas)
 */
inline void excep_restore_irq (uint32_t i_bit)
{
    asm volatile(
            "mrs r12, cpsr\n\t"          // aux <- cpsr
            "bic r12, r12, #0x80\n\t" // Limpiamos el bit I
            "orr r12, r12, %[bit], LSL #7\n\t" // Restaurar el bit
            "msr cpsr_c, r12"
            :
            : [bit] "r" (i_bit & 1)  // Se le hace un & por si hay basura
            : "r12", "cc"
	);
}

/*****************************************************************************/

/**
 * Restaura el antiguo valor de la máscara de interrupciones rápidas
 * @param f_bit	Valor anterior de la máscara
 * 						0: F=0	(FIQ habilitadas)
 * 						1: F=1	(FIQ deshabilitadas)
 */
inline void excep_restore_fiq (uint32_t f_bit)
{
	asm volatile(
            "mrs r12, cpsr\n\t"          // aux <- cpsr
            "bic r12, r12, #0x40\n\t" // Limpiamos el bit F
            "orr r12, r12, %[bit], LSL #6\n\t" // Restaurar el bit
            "msr cpsr_c, r12"
            :
            : [bit] "r" (f_bit & 1)  // Se le hace un & por si hay basura
            : "r12", "cc"
	);
}

/*****************************************************************************/

/**
 * Asigna un manejador de interrupción/excepción
 * @param excep		Tipo de excepción
 * @param handler	Manejador
 */
inline void excep_set_handler (excep_t excep, excep_handler_t handler)
{
	_excep_handlers[excep] = handler;
}

/*****************************************************************************/

/**
 * Retorna un manejador de interrupción/excepción
 * @param excep		Tipo de excepción
 */
inline excep_handler_t excep_get_handler (excep_t excep)
{
	
    return _excep_handlers[excep];
}

/*****************************************************************************/

/**
 * Manejador en C para interrupciones normales no anidadas
 * El atributo interrupt no guarda en la pila el registro spsr, por lo que
 * sólo genera manejadores para interrupciones no anidadas
 * Para poder gestionar interrupciones anidadas y sacar partiro al controlador
 * de interrupciones es necesario escribir el manejador en ensamblador
 */
void excep_nonnested_irq_handler ()
{
	/* ESTA FUNCIÓN SE DEFINIRÁ EN LA PRÁCTICA 7 */
}

/*****************************************************************************/
