/*
 * Sistemas operativos empotrados
 * Implementación de las llamadas asl sistema
 */

#include <sys/types.h>
#include <reent.h>
#include <errno.h>

#include "system.h"

/*****************************************************************************/

/**
 * Dirección de inicio del heap, definida en el script de enlazado
 */
extern void _heap_start, _heap_end;

/*****************************************************************************/

/**
 * Incrementa el tamaño de área reservada para datos dentro del heap.
 * Las funciones de gestión de memoria dinamica (malloc) depende de esta llamada
 * al sistema.
 * @param incr	Tamaño del incremento solicitado (en bytes)
 * @return		Un puntero al nuevo bloque de memoria asignado o -1 en caso de error.
 * 				La condición de error se indica en la variable global errno.
 */
void * _sbrk (intptr_t incr)
{
	/* ESTA FUNCIÓN SE DEFINIRÁ EN LA PRÁCTICA 9 */
	return (void *) -1;
}

/*****************************************************************************/

/**
 * Abre un dispositivo/fichero
 * @param pathname	Nombre del dispositivo/fichero
 * @param flags		Modo de acceso
 * @param mode		Permisos en caso de que se cree un archivo/dispositivo
 * @return			El descriptor del dispositivo o -1 en caso de error.
 * 					La condición de error se indica en la variable global errno.
 */
int _open(const char *pathname, int flags, mode_t mode)
{
	/* ESTA FUNCIÓN SE DEFINIRÁ EN LA PRÁCTICA 9 */

	return -1;
}

/*****************************************************************************/

/**
 * Cierra un dispositivo/fichero
 * @param fd	Descriptor de fichero/dispositivo
 * @return		0 en caso de éxito o -1 en caso de error.
 * 				La condición de error se indica en la variable global errno.
 */
int _close (int fd)
{
	/* ESTA FUNCIÓN SE DEFINIRÁ EN LA PRÁCTICA 9 */
	return -1;
}

/*****************************************************************************/

/**
 * Lectura de un dispositivo/fichero
 * @param fd	Descriptor de fichero/dispositivo
 * @param buf	Puntero al búfer donde se almacenarán los datos
 * @param count	Número de bytes que se quieren leer
 * @return		El número de bytes leidos o -1 en caso de error.
 * 				La condición de error se indica en la variable global errno.
 */
ssize_t _read(int fd, char *buf, size_t count)
{
	/* ESTA FUNCIÓN SE DEFINIRÁ EN LA PRÁCTICA 9 */
	return 0;
}

/*****************************************************************************/

/**
 * Escritura en un dispositivo/fichero
 * @param fd	Descriptor de fichero/dispositivo
 * @param buf	Puntero al búfer que almacena los datos
 * @param count	Número de bytes que se quieren escribir
 * @return		El número de bytes escritos o -1 en caso de error.
 * 				La condición de error se indica en la variable global errno.
 */
ssize_t _write (int fd, char *buf, size_t count)
{
	/* ESTA FUNCIÓN SE DEFINIRÁ EN LA PRÁCTICA 9 */
	return count;
}

/*****************************************************************************/

/**
 * Modificación del desplazamiento en un dispositivo/fichero
 * @param fd		Descriptor de fichero/dispositivo
 * @param offset	Desplazamiento
 * @param whence	Base para el desplazamiento
 * @return			El nuevo desplazamiento medido en bytes desde el inicio del fichero o -1 en caso de error.
 * 					La condición de error se indica en la variable global errno
 */
off_t _lseek(int fd, off_t offset, int whence)
{
	/* ESTA FUNCIÓN SE DEFINIRÁ EN LA PRÁCTICA 9 */
	return 0;
}

/*****************************************************************************/

/**
 * Obtención de información de un dispositivo/fichero
 * @param fd	Descriptor de fichero/dispositivo
 * @param buf	Estructura para almacenar dicha información
 * @return		0 o -1 en caso de error.
 * 				La condición de error se indica en la variable global errno
 */
int _fstat(int fd, struct stat *buf)
{
	/* ESTA FUNCIÓN SE DEFINIRÁ EN LA PRÁCTICA 9 */
	return 0;
}

/*****************************************************************************/

/**
 * Chequea si el descriptor de fichero corresponde con una terminal
 * @param fd	Descriptor de fichero/dispositivo
 * @return		1 en caso de que el descriptor corresponda a una terminal o 0 en otro caso.
 * 				La condición de error se indica en la variable global errno
 */
int _isatty (int fd)
{
	/* ESTA FUNCIÓN SE DEFINIRÁ EN LA PRÁCTICA 9 */
	return 1;
}

/*****************************************************************************/
