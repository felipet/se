/*
 * Sistemas operativos empotrados
 * Driver para el controlador de interrupciones del MC1322x
 */

#include "system.h"

/*****************************************************************************/

/**
 * Acceso estructurado a los registros de control del ITC del MC1322x
 */
typedef struct
{
	uint32_t INTCNTL;       // Interrupt Control Register
	uint32_t NIMASK;        // Normal Interrupt Mask Register
	uint32_t INTENNUM;      // Interrupt Enable Number Register
	uint32_t INTDISNUM;     // Interrupt Disable Number Register
	uint32_t INTENABLE;     // Interrupt Enable Register
	uint32_t INTTYPE;       // Interrupt Type Register
	uint32_t ;              // Reservado 0x18 - 0x24
	uint32_t ;              // Reservado
	uint32_t ;              // Reservado
	uint32_t ;              // Reservado
	uint32_t NIVECTOR;      // Normal Interrupt Vector
	uint32_t FIVECTOR;      // Fast Interrupt Vector
	uint32_t INTSRC;        // Interrupt Source Register
	uint32_t INTFRC;        // Interrupt Force Register
	uint32_t NIPEND;        // Normal Interrupt Pending Register
	uint32_t FIPEND;	    // Fast Interrupt Pending Register
} itc_regs_t;

static volatile itc_regs_t* const itc_regs = ITC_BASE;

/**
 * Tabla de manejadores de interrupción.
 */
static itc_handler_t itc_handlers[itc_src_max];

/*****************************************************************************/

/**
 * Inicializa el controlador de interrupciones.
 * Deshabilita los bits I y F de la CPU, inicializa la tabla de manejadores a NULL,
 * y habilita el arbitraje de interrupciones Normales y rápidas en el controlador
 * de interupciones.
 */
inline void itc_init ()
{
	/* ESTA FUNCIÓN SE DEFINIRÁ EN LA PRÁCTICA 8 */
}

/*****************************************************************************/

/**
 * Asigna un manejador de interrupción
 * @param src		Identificador de la fuente
 * @param handler	Manejador
 */
inline void itc_set_handler (itc_src_t src, itc_handler_t handler)
{
    uint32_t bits;

    // Sección crítica
	bits = excep_disable_ints();
	itc_handlers[src] = handler;
	excep_restore_ints(bits);
	// Fin de sección crítica
}

/*****************************************************************************/

/**
 * Asigna una prioridad (normal o fast) a una fuente de interrupción
 * @param src		Identificador de la fuente
 * @param priority	Tipo de prioridad
 */
inline void itc_set_priority (itc_src_t src, itc_priority_t priority)
{ 
    uint32_t bits;

    // Sección crítica
	bits = excep_disable_ints();
	if(priority == itc_priority_fast) {
	    if(*INTTYPE)        // Ya hay una fuente asignada a FIQ
	        *INTTYPE = 0;   // Poner todas a IRQ
        *INTTYPE = (1 << src);
	}
	else if(priority == itc_priority_normal)
	    *INTTYPE &= ~(1 << src);    // Bit clear 
    excep_restore_ints(bits);
	// Fin de sección crítica
}

/*****************************************************************************/

/**
 * Habilita las interrupciones de una determinda fuente
 * @param src		Identificador de la fuente
 */
inline void itc_enable_interrupt (itc_src_t src)
{
	uint32_t bits;
	
	// Sección crítica
	bits = excep_disable_ints();
	*INTENNUM = src;
	excep_restore_ints(bits);
	// Fin de sección crítica
}

/*****************************************************************************/

/**
 * Deshabilita las interrupciones de una determinda fuente
 * @param src		Identificador de la fuente
 */
inline void itc_disable_interrupt (itc_src_t src)
{
	uint32_t bits;
	
	// Sección crítica
	bits = excep_disable_ints();
	*INTDISNUM = src;
	excep_restore_ints(bits);
	// Fin de sección crítica
}

/*****************************************************************************/

/**
 * Fuerza una interrupción con propósitos de depuración
 * @param src		Identificador de la fuente
 */
inline void itc_force_interrupt (itc_src_t src)
{
	/* ESTA FUNCIÓN SE DEFINIRÁ EN LA PRÁCTICA 8 */
}

/*****************************************************************************/

/**
 * Desfuerza una interrupción con propósitos de depuración
 * @param src		Identificador de la fuente
 */
inline void itc_unforce_interrupt (itc_src_t src)
{
	/* ESTA FUNCIÓN SE DEFINIRÁ EN LA PRÁCTICA 8 */
}

/*****************************************************************************/

/**
 * Da servicio a la interrupción normal pendiente de más prioridad.
 * Deshabilia las IRQ de menor prioridad hasta que se haya completado el servicio
 * de la IRQ para evitar inversiones de prioridad
 */
void itc_service_normal_interrupt ()
{
	uint32_t bits;
	
	// Sección crítica
	bits = excep_disable_irq();
	
	
	
	excep_restore_irq(bits);
	// Fin de sección crítica
}

/*****************************************************************************/

/**
 * Da servicio a la interrupción rápida pendiente de más prioridad
 */
void itc_service_fast_interrupt ()
{
	uint32_t bits;
	
	// Sección crítica
	bits = excep_disable_ints();
	
	
	
	excep_restore_ints(bits);
	// Fin de sección crítica
}

/*****************************************************************************/
