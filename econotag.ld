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

	/* Sección .bss */
        /* Generamos una sección para las variables globales sin inicializar */

        /* Gestión de las pilas */
	/* Generar una sección al final de la RAM para las pilas de cada modo y definir símbolos para el tope de cada pila */

 	/* Gestión del heap */
	/* Generar una sección que ocupe el espacio entre la sección .bss y las pilas para el heap, con los símbolos de inicio y fin del heap */
}

