#include <stddef.h>
#include "../inc/terminal.h"

size_t strlen(const char* str) 
{
		size_t len = 0;
		while (str[len])
				len++;
		return len;
}