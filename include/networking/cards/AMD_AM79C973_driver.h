/*
 * AMD_AM79C973_driver.h
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


#ifndef AMD_AM79C973_DRIVER_H
	#define AMD_AM79C973_DRIVER_H
	
	#include "../../types.h"
	#include "../../system/system.h"
	#include "../../pci/pci.h"
	
	typedef struct {
		uint16 mode;
		
		unsigned reserved1 : 4;
		unsigned reserved2 : 4;
		uint16 reserved3;
		
		unsigned num_send_buffers : 4;
		unsigned num_recv_buffers : 4;
		
		uint64 physical_address : 48;
		uint64 logical_address; 
		
		uint32 recv_buffer_desc_address;
		uint32 send_buffer_desc_address;
		
	} initialization_block;
	
	typedef struct {
		uint32 address;
		uint32 flags;
		uint32 flags2;
		uint32 avail;
		
	} buffer_desc;
	
	void amd_am79c973_analyse_status(void);
	void amd_am79c973_handler(struct registers* regs);
	void amd_am79c973_init(PCI_device_descriptor* _device);
	void amd_am79c973_activate(void);
	int amd_am79c973_reset(void);
	
	void amd_am79c973_send(uint8* data, int _size);
	void amd_am79c973_receive(void);
	
	uint64 amd_am79c973_MAC_address(void);
	uint32 amd_am79c973_IP_address(void);
	
#endif
