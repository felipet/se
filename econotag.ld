/*
 * Sistemas Empotrados
 * Linker script para la Redwire EconoTAG
 * Runtime de C para ser cargado por la BIOS de la placa
 */

/*
 * Punto de entrada
 */
ENTRY(_vector_table)

/*
 * Mapa de memoria de la placa
 */
MEMORY
{
        ram   : org = 0x00400000,       len = 0x00018000        /*  96 KB */
}

SECTIONS
{
	/* Imagen del firmware */
	/* Generar una sección al principio de la RAM que organice las secciones del firmware al comienzo de la RAM de la plataforma */
	/* Código del cargador */
	.startup :
	{
	    *(.startup);
	} > flash
	
	/* Código del programa */
	.text :
	{
	    *(.text);
    } > flash
       
    .rodata :
    {
        // ????????
    }

    .data :
    {
        _data_start = .;
        *(.data);
        . = ALIGN(4);
        _data_end = .;   
    } > ram
    //_data_flash_start = LOADADDR(.data);

	/* Sección .bss */
        /* Generamos una sección para las variables globales sin inicializar */
    .bss :
    {
        _bss_start = .;
        *(.bss);
        . = ALIGN(4);
        *(COMMON);
        . = ALIGN(4);
        _bss_end = .;
    } > flash
    
    
        /* Gestión de las pilas */
	/* Generar una sección al final de la RAM para las pilas de cada modo y definir símbolos para el tope de cada pila */
	_ram_limit = ORIGIN(ram) + LENGTH(ram);
	_stack_size = 0x800;    /* 2 KB */
	
	.stack _ram_limit - _stack_size :
	{
	    _stack_bottom = .;
	    . += _stack_size;
	    _stack_top = .;
    }

 	/* Gestión del heap */
	/* Generar una sección que ocupe el espacio entre la sección .bss y las pilas para el heap, con los símbolos de inicio y fin del heap */
	
}

