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
    } > ram
    
    
        /* Gestión de las pilas */
	/* Generar una sección al final de la RAM para las pilas de cada modo y definir símbolos para el tope de cada pila */
	_ram_limit = ORIGIN(ram) + LENGTH(ram);
	_sys_stack_size  = 0x400;   /* 1 KB */
	_irq_stack_size  = 0x100;   /* 256 B */
	_fiq_stack_size  = 0x100;   /* 256 B */
	_svc_stack_size  = 0x200;   /* 512 B */ 
	_abt_stack_size  = 0x20;    /* 16 B */ 
	_und_stack_size  = 0x20;    /* 16 B */
	_stack_size = _stacks_top - _stacks_bottom;
	
	.stacks _ram_limit - _stack_size :
	{
	    _stacks_bottom = .;     /* Final de la pila */
	    . += _sys_stack_size;
	    _sys_stack_top = .;     /* Tope de pila para modo System/User */
	    
	    . += _svc_stack_size;
	    _svc_stack_top = .;     /* Tope de pila para modo Supervisor */
	    
	    . += _stack_size;
	    _abt_stack_top = .;     /* Tope de pila para modo Abort */
	    	    
	    . += _stack_size;
	    _und_stack_top = .;     /* Tope de pila para modo Undefined */
	    	    
	    . += _stack_size;
	    _irq_stack_top = .;     /* Tope de pila para modo IRQ */
	    	    
	    . += _stack_size;
	    _fiq_stack_top = .;     /* Tope de pila para modo FIQ */
	    _stacks_top = .;        /* Tope de la pila */
    }

 	/* Gestión del heap */
	/* Generar una sección que ocupe el espacio entre la sección .bss y las pilas para el heap, con los símbolos de inicio y fin del heap */
	_heap_size = _stack_bottom - _bss_end;
	
	.heap _bss_end :
	{
	    _heap_start = .; /* En la siguiente palabra de memoria */
	    . += _heap_size; /* a _bss_end */
	    _heap_end   = .;
    }
	
}

