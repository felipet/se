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
        gpio :  org = 0x80000000,  len = 0x00008000       /* 32 kB   */
}


SECTIONS
{
        /* Código del programa */
        .text : ALIGN(4)
        {
            *(.text);
        } > ram0
        
        .data :
        {
            *(.data);
            . = ALIGN(4);
        } > ram1
        
        
        
        .gpio : ALIGN(4)
        {  
            GPIO_PAD_DIR0       = . + 0x00000000;
            GPIO_PAD_DIR1       = . + 0x00000004;
            GPIO_DATA0          = . + 0x00000008;
            GPIO_DATA_SET0      = . + 0x00000048;
            GPIO_DATA_SET1      = . + 0x0000004c;
            GPIO_DATA_RESET0    = . + 0x00000050;
            GPIO_DATA_RESET1    = . + 0x00000054;
            
        } > gpio
}
