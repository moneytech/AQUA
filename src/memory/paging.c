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

static void blank_pages(void) {
	int i;
	for (i = 0; i < 1024; i++) {
		page_directory[i] = 0x00000002;
		first_page_table[i] = (i * 0x1000) | 3;
		
	}
	
	page_directory[0] = ((uint32) first_page_table) | 3;
	
}

void init_paging(void) {
	load_page_dir(page_directory);
	enable_paging();
	
}
