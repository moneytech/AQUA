/*
 * kernel.c
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

#include "../include/multiboot.h"

#include "../include/types.h"
#include "../include/screen.h"

#include "../include/OS.h"
#include "../include/graphics/GFX_loader.h"

//typedef void (*constructor) ();

//extern constructor start_ctors;
//extern constructor end_ctors;
/*
extern void call_constructors(void) {
	constructor* i;
	for (i = &start_ctors; i != &end_ctors; i++) {
		(*i)();
		
	}
	
}*/

extern void main(unsigned long magic, unsigned long addr) {
	/*load_gdt();
	asm volatile("hlt");
	
	uint8* wallpaper0;
	uint8* wallpaper2;
	uint8* wallpaper5;
	uint8* wallpaper7;
	uint8** font_aqua_15px;
	uint8** font_aqua_20px;
	uint8** font_aqua_50px;
	uint8** font_mono_15px;
	uint8* icon_off;
	uint8* icon_back;
	uint8* icon_next;
	uint8* icon_edit;
	uint8* icon_circle;
	uint8* icon_save;
	uint8* icon_load;*/
	
	OS_run(\
		magic, \
		addr, \
		\
		wallpaper0, \
		wallpaper2, \
		wallpaper5, \
		wallpaper7, \
		\
		257, \
		font_aqua_15px, \
		439, \
		font_aqua_20px, \
		2681, \
		font_aqua_50px, \
		130, \
		font_mono_15px, \
		\
		icon_off, \
		icon_back, \
		icon_next, \
		icon_edit, \
		icon_circle, \
		\
		icon_save, \
		icon_load \
		\
	);
	
}
