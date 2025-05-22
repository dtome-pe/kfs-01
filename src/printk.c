#include <stdarg.h>
#include <stddef.h>
#include "../inc/lib.h"
#include "../inc/terminal.h"

// Define a buffer size suitable for your needs
#define PRINTK_BUFFER_SIZE 1024

// Placeholder for your custom vsnprintf implementation
size_t 	vsnprintf(char *buffer, const char *format, va_list *args);

// Placeholder for your output function
void 	console_write(const char *str);

size_t 	printk(const char *format, ...) 
{
		char buffer[PRINTK_BUFFER_SIZE];
		buffer[PRINTK_BUFFER_SIZE - 1] = '\0';
		va_list args;
		int printed;

		va_start(args, format);
		printed = vsnprintf(buffer, format, &args);
		va_end(args);

		// Output the formatted string
		terminal_putstr(buffer);

		return printed;
}

int 	append_char(char *buffer, size_t *pos, char c) 
{
		if (*pos + 1 >= PRINTK_BUFFER_SIZE) 
				return -1;

		buffer[*pos] = c;
		(*pos)++;
		buffer[*pos] = '\0';
		return 1;
}

int append_str(char *buffer, size_t *pos, char *str) 
{
		int	n = 0;

		if (!buffer || !str || !pos) {
				return -1;
		}

		while (*str) {
				if (append_char(buffer, pos, *str) == -1) {
						return -1;
				}
				str++;
				n++;
		}

		return n;
}

size_t	check_conversions(va_list *args, char const *format, char *buffer, size_t *pos)
{	
		++format;
		if (*format == 'c')
				return (append_char(buffer, pos, va_arg(*args, int)));
		else if (*format == 's')
				return (append_str(buffer, pos, va_arg(*args, char *)));
/* 		else if (*format == 'p')
				return (ft_putadd(va_arg(args, void *)));
		else if (*format == 'd')
				return (ft_putnbr(va_arg(args, int)));
		else if (*format == 'i')
				return (ft_putnbr(va_arg(args, int)));
		else if (*format == 'u')
				return (ft_putunbr(va_arg(args, unsigned int)));
		else if (*format == 'x')
				return (ft_hex(va_arg(args, unsigned int), "0123456789abcdef"));
		else if (*format == 'X')
				return (ft_hex(va_arg(args, unsigned int), "0123456789ABCDEF")); */
		else if (*format == '%')
				return (append_char(buffer, pos, '%'));
		else
				return (-1);
}

size_t	vsnprintf(char *buffer, const char *format, va_list *args)
{
		size_t	n = 0;
		size_t	pos = 0;

		while (*format)
		{
				if (*format == '%')
				{
						n += check_conversions(args, format, buffer, &pos);
						format++;
				}
				else
						n += append_char(buffer, &pos, *format);
				if ((int) n == -1)
						return (-1);
				format++;
		}
		return n;
}