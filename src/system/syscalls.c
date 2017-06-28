/*
 * syscalls.c
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


#include "syscalls.h"

syscall_handle_interrupt(struct registers* regs) {
	switch (regs->eax) {
		case 4: {
			print("New syscall recieved (", 0x0f);
			print((char*) regs->eax, 0x0f);
			println(")", 0x0f);
			break;
			
		} default: {
			break;
			
		}
		
	}
	
}

void syscall_install(void) {
	irq_add_handler(0x80, syscall_handle_interrupt);
	
}
