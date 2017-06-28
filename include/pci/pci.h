/*
 * pci.h
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

#ifndef PCI_H
	#define PCI_H
	
	#include "../types.h"
	#include "../screen.h"
	#include "../system/system.h"
	
	#define PCI_DATA_PORT 0xCFC//0xCF8 // PCI_CONFIG_ADDRESS
	#define PCI_COMMAND_PORT 0xCF8//0xCFC // PCI_CONFIG_DATA
	
	typedef struct PCI_device {
		uint8 bus;
		uint8 slot;
		uint8 flags;
		
	} PCI_device;
	
	typedef struct {
		uint32 port_base;
		uint32 interrupt;
		
		uint16 bus;
		uint16 device;
		uint16 function;
		
		uint16 vendor_id;
		uint16 device_id;
		
		uint8 class_id;
		uint8 subclass_id;
		uint8 interface_id;
		
		uint8 revision;
		
	} PCI_device_descriptor;
	
	typedef enum base_address_register_type {
		memory_mapping = FALSE,
		input_output = TRUE
		
	} base_address_register_type;
	
	typedef struct {
		boolean prefetchable;
		uint8* address;
		uint32 _size;
		base_address_register_type type;
		
	} base_address_register;
	
	typedef struct {
		string vendor_name;
		string device_name;
		string device_type;
		
	} PCI_driver_descriptor;
	
	uint16 PCI_read_word(uint8 bus, uint8 slot, uint8 func, uint8 offset);
	uint32 PCI_read_dword(uint8 bus, uint8 slot, uint8 func, uint8 offset);
	uint16 get_PCI_vendor(uint8 bus, uint8 slot, uint8 function);
	
	PCI_device get_PCI_device(uint8 tclass, uint8 tsub_class, uint8 tprog_IF);
	void PCI_debug(void);
	void print_PCI_class_code(uint8 class_code, uint8 sub_class, uint8 prog_IF);
	
	uint32 PCI_read(uint16 bus, uint16 device, uint16 function, uint32 register_offset);
	void PCI_write(uint16 bus, uint16 device, uint16 function, uint32 register_offset, uint32 value);
	boolean PCI_device_has_functions(uint16 bus, uint16 device);
	
	PCI_device_descriptor PCI_get_device_descriptor(uint16 bus, uint16 device, uint16 function);
	base_address_register PCI_get_base_address_register(uint16 bus, uint16 device, uint16 function, uint16 bar);
	
	PCI_driver_descriptor PCI_get_driver(PCI_device_descriptor device);
	
#endif
