/*
 * PIT_driver.h
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


#ifndef PIT_DRIVER_H
	#define PIT_DRIVER_H
	
	#include "../types.h"
	#include "../system/system.h"
	#include "../interrupts/irq.h"
	#include "../graphics/GFX.h"
	
	#include "../user/sleep.h"
	#include "../user/cursor_task.h"
	
	int pit_add_process(void (*process) (void));
	void pit_remove_process(int id);
	
	void timer_handler(struct registers* r);
	void reset_timer(void);
	
	void timer_phase(int hertz);
	void timer_disable(void);
	void timer_install(boolean _buffer_blit);
	
#endif
