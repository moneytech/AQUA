/*
 * paging.c
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


#include "paging.h"

extern void load_page_dir(uint32*);
extern void enable_paging(void);

uint32 page_directory[1024] __attribute__((aligned(0x1000)));
uint32 first_page_table[1024] __attribute__((aligned(0x1000)));

void blank_pages(void) {
	int i;
	for (i = 0; i < 1024; i++) {
		page_directory[i] = 0x00000002;
		first_page_table[i] = (i * 0x1000) | 3;
		
	}
	
	page_directory[0] = ((uint32) first_page_table) | 3;
	
}

void map_page(uint32 physical_address, uint32 virtual_address, int _size) {
	uint32 page_directory_index = virtual_address >> 22 & 0x3FF;
	uint32 page_table_index = virtual_address >> 11 & 0x3FF;
	
	uint32* _page_table = page_directory[page_directory_index];
	
	int i;
	for (i = 0; i < _size; i++) {
		_page_table[page_table_index + i] = physical_address + (i * 0x1000) | 3;
		flush_tlb(virtual_address);
		
	}
	
}

void identity_paging(uint32 from, int _size) {
	uint32* table = &first_page_table;
	from &= 0xFFFFF000;
	
	for (; _size > 0; from += 0x1000, _size -= 0x1000, table++) {
		*table = from | 1;
		
	}
	
}

inline void flush_tlb(uint32 address) {
	asm volatile("invlpg (%0)" :: "r" (address) : "memory");
	
}

void page_fault(void) {
	while (TRUE) println("FAULT!!!!!!!", 0x3e);
	
}

void init_paging(void) {
	irq_add_handler(14, page_fault);
	load_page_dir(page_directory);
	enable_paging();
	
}
