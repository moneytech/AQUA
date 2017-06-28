/*
 * fpu.c
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


#include "fpu.h"

void fpu_init(void) {
	asm volatile("fninit");
	
}

void fpu_install(void) {
	fpu_enable();
	fpu_disable();
	irq_add_handler(7, &fpu_invalid_op);
	
}

void fpu_set_control(uint16 value) {
	asm volatile("fldcw %0" :: "m" (value));
	
}

void fpu_enable(void) {
	asm volatile("clts");
	uint32 _size;
	
	asm volatile("mov %%cr0, %0" : "=r" (_size));
	
	_size &= ~(1 << 2);
	_size |= (1 << 1);
	
	asm volatile("mov %0, %%cr0" :: "r" (_size));
	asm volatile("mov %%cr4, %0" : "=r" (_size));
	
	_size |= 3 << 9;
	
	asm volatile("mov %0, %%cr4" :: "r" (_size));
	
}

void fpu_disable(void) {
	uint32 _size;
	asm volatile("mov %%cr0, %0" : "=r"(_size));
	
	_size |= 1 << 3;
	asm volatile("mov %0, %%cr0" :: "r"(_size));
	
}

void fpu_invalid_op(struct registers* regs) {
	fpu_enable();
	
}

void fpu_switch(void) {
	fpu_disable();
	
}

void fpu_unswitch(void) {
	
	
}
