/*
 * network_wrapper.h
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


#ifndef NETWORK_WRAPPER_H
	#define NETWORK_WRAPPER_H
	
	#include "../types.h"
	#include "../screen.h"
	
	#include "../pci/pci.h"
	#include "../interrupts/irq.h"
	#include "../utils/string.h"
	
	#include "cards/AMD_AM79C973_driver.h"
	//#include "cards/i8254x_driver.h"
	//#include "cards/rtl8139_driver.h"
	
	#define CARD_AMD_AM79C973 1
	#define CARD_I8254X 2
	#define CARD_RTL8139 3
	
	boolean networking_get_card(void);
	PCI_device_descriptor networking_get_PCI_device(void);
	
	void networking_analyse_status(void);
	void networking_install(void);
	void networking_activate(void);
	
	void networking_send(uint8* data, int _size);
	
	uint64 networking_get_MAC_address(void);
	uint32 networking_get_IP_address(void);
	
#endif
