/*
 * link.ld
 * 
 * Copyright 2017 Obiwac
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


ENTRY(start)
OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386:i386)

SECTIONS {
	. = 0x0100000;
	krnlstart = .;
	
	/*. = 0x100000; to be commented*/
	.text : AT(ADDR(.text) - 0x00000000) {*(.text)}
	
	.data : AT(ADDR(.data) - 0x00000000) {
		start_ctors = .;
		KEEP(*(.init_array));
		KEEP(*(SORT_BY_INIT_PRIORITY(.init_array.*)));
		end_ctors = .;
		*(.data)
		
	}
	
	.bss : AT(ADDR(.bss) - 0x00000000) {
		*(.COMMON)
		*(.bss)
		
	}
	
	end = .; _end = .; __end = .;
	/DISCARD/ : {*(.fini_array*) *(.comment)}
	
}
