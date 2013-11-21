/*****************************************************************************/
/*                                                                           */
/* Sistemas Empotrados                                                       */
/*                                                                           */
/* Linker script para la Redwire EconoTAG                                    */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/
/* Punto de entrada                                                          */
/*****************************************************************************/

ENTRY(_start)

/*
 * Mapa de memoria de la placa
 */
MEMORY
{
        /* Los 96KB de SRAM están organizados en 4 bloques,
         * con direcciones contiguas:
         */
        ram0 :  org = 0x00400000,  len = 0x00002000       /* (8 KB)  */
        ram1 :  org = 0x00402000,  len = 0x00006000       /* (24 KB) */
        ram2 :  org = 0x00408000,  len = 0x00008000       /* (32 KB) */
        ram3 :  org = 0x00410000,  len = 0x00008000       /* (32 KB) */
}

SECTIONS
{
        /* Código del programa */
        .text : ALIGN(4)
        {
            *(.text);
        } > ram0
        
        .data : ALIGN(4)
        {
            *(.data);
        } > ram1
}
