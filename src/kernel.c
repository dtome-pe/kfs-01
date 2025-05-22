#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

#include "../inc/terminal.h"
#include "../inc/lib.h"

void kernel_main(void) 
{
		/* Initialize terminal interface */
		terminal_initialize();

		/* Print each number from 42 through 70, one per line */
		/* for (int n = 42; n <= 70; n++) {

				terminal_putchar('0' + (n / 10));
				terminal_putchar('0' + (n % 10));
				terminal_putchar('\n');
		} */
		/* terminal_writestring("hi!\n");
		terminal_writestring("hi\n"); */
		printk("hola %s %s nick: %s\n", "diego", "tome", "dtome-pe");
}