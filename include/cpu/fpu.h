/*
 * fpu.h
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


#ifndef FPU_H
	#define FPU_H
	
	#include "../types.h"
	#include "../screen.h"
	#include "../interrupts/irq.h"
	
	void fpu_init(void);
	void fpu_install(void);
	void fpu_set_control(uint16 value);
	
	void fpu_enable(void);
	void fpu_disable(void);
	
	void fpu_invalid_op(struct registers* regs);
	void fpu_switch(void);
	void fpu_unswitch(void);
	
#endif
