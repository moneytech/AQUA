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

void map_kernel(vaddr from, int _size) {
	//uint32* page_table = &first_page_table;
	
	from = from & 0xfffff000;
	for(; _size > 0; from += 4096 , _size -= 4096, &first_page_table[0]++){
		*first_page_table = from | 1;
		
	}
	
}

void* get_phys_address(void* virtual_address) {
	uint64 page_directory_index = (uint64) virtual_address >> 22;
	uint64 page_table_index = (uint64) virtual_address >> 12 & 0x03FF;
	
	uint64* _page_directory = (uint64*) 0xFFFFF000;
	uint64* _page_table = ((uint64*) 0xFFC00000) + (0x400 * page_directory_index);
	
	return (void*) ((_page_table[page_table_index] & ~0xFFF) + ((uint64) virtual_address & 0xFFF));
	
}

void map_page(void* phys_address, void* virtual_address, uint32 flags) {
	uint64 page_directory_index = (uint64) virtual_address >> 22;
	uint64 page_table_index = (uint64) virtual_address >> 12 & 0x03FF;
	
	uint64* _page_directory = (uint64*) 0xFFFFF000;
	uint64* _page_table = ((uint64*) 0xFFC00000) + (0x400 * page_directory_index);
	
	_page_table[page_table_index] = ((uint64) phys_address) | (flags & 0xFFF) | 0x01;
	
}

void page_fault(void) {
	println("FAULT!!!!!!!", 0x3e);
	
}

void init_paging(void) {
	irq_add_handler(14, page_fault);
	load_page_dir(page_directory);
	enable_paging();
	
}
