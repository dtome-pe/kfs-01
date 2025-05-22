#include <stddef.h>
#include <stdint.h>
#include "../inc/vga.h"
#include "../inc/lib.h"

size_t		terminal_column;
size_t		terminal_row;
uint8_t		terminal_color;
uint16_t*	terminal_buffer = (uint16_t*)VGA_MEMORY;
int			color_fg = 0;
int			color_bg = 15;

void terminal_initialize(void) 
{
		terminal_row = 0;
		terminal_column = 0;
		terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
		
		for (size_t y = 0; y < VGA_HEIGHT; y++) {
				for (size_t x = 0; x < VGA_WIDTH; x++) {
						const size_t index = y * VGA_WIDTH + x;
						terminal_buffer[index] = vga_entry(' ', terminal_color);
				}
		}
}


static inline void outb(uint16_t port, uint8_t val) {
	__asm__ volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

void terminal_update_cursor(void) {
	uint16_t pos = terminal_row * VGA_WIDTH + terminal_column;

	outb(0x3D4, 0x0F);                      // Tell VGA we're setting the low byte
	outb(0x3D5, (uint8_t)(pos & 0xFF));     // Send low byte

	outb(0x3D4, 0x0E);                      // Now the high byte
	outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF)); // Send high byte
}

void terminal_setcolor(uint8_t color) 
{
		terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) 
{
		const size_t index = y * VGA_WIDTH + x;
		terminal_buffer[index] = vga_entry(c, color);
}

static void terminal_scroll()
{
    // Move all rows up by one
    for (size_t y = 1; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            terminal_buffer[(y - 1) * VGA_WIDTH + x] =
                terminal_buffer[y * VGA_WIDTH + x];
        }
    }

    // Clear the last line (bottom row)
    for (size_t x = 0; x < VGA_WIDTH; x++) {
        terminal_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] =
            vga_entry(' ', vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
    }

	terminal_row = VGA_HEIGHT - 1;
	terminal_update_cursor();
}

void terminal_putchar(char c) 
{
		if (c == '\n') {
				terminal_column = 0;
				if (++terminal_row == VGA_HEIGHT) {
						terminal_scroll();  // will update cursor inside
				} else {
						terminal_update_cursor();  // sync after newline
				}
				return;
		}

/* 
		color_fg = (color_fg + 1) % 16;
		color_bg = (color_bg - 1 + 16) % 16; 
		terminal_color = vga_entry_color(color_fg, color_bg);
 */
 
		terminal_putentryat(c, terminal_color, terminal_column, terminal_row);

		if (++terminal_column == VGA_WIDTH) {
				terminal_column = 0;
				if (++terminal_row == VGA_HEIGHT) {
						terminal_scroll();
						return;
				}
		}

		terminal_update_cursor();	
}

void terminal_putstr(const char* data) 
{
		for (size_t i = 0; i < strlen(data); i++)
				terminal_putchar(data[i]);
}