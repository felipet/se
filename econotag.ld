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
	} > ram
	
	/* Código del programa */
	.text :
	{
	    *(.text);
    } > ram
       
    .rodata :
    {
        *(.rodata);
        . = ALIGN(4);
    } > ram

    .data :
    {
        _data_start = .;
        *(.data);
        . = ALIGN(4);
        _data_end = .;   
    } > ram

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
	_stack_size = 0x400;    /* 1 KB para la pila de cada modo */
	
	.stacks _ram_limit - _stack_size*6 :
	{
	    _sys_stack_top = .;     /* Tope de pila para modo System/User */
	    . += _stack_size;       
	    
	    _svc_stack_top = .;     /* Tope de pila para modo Supervisor */
	    . += _stack_size;
	    
	    _abt_stack_top = .;     /* Tope de pila para modo Abort */
	    . += _stack_size;
	    
	    _und_stack_top = .;     /* Tope de pila para modo Undefined */
	    . += _stack_size;
	    
	    _irq_stack_top = .;     /* Tope de pila para modo IRQ */
	    . += _stack_size;
	    
	    _fiq_stack_top = .;     /* Tope de pila para modo FIQ */
	    . += _stack_size;
	    _stacks_bottom = .;        /* Final de la pila */
    }

 	/* Gestión del heap */
	/* Generar una sección que ocupe el espacio entre la sección .bss y las pilas para el heap, con los símbolos de inicio y fin del heap */
	.heap :
	{
	    _heap_start = _bss_end + 0x4; /* En la siguiente palabra de memoria */
	    *(.heap);                      /* a _bss_end */
	    _heap_end   = _sys_stack_top - 0x4;
    } > ram
	
}

