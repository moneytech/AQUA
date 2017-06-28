/*
 * AMD_AM79C973_driver.c
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


#include "AMD_AM79C973_driver.h"

static uint16 MAC_address_0_port;
static uint16 MAC_address_2_port;
static uint16 MAC_address_4_port;

static uint16 IP_address_0_port;
static uint16 IP_address_2_port;

static uint16 register_data_port;
static uint16 register_address_port;

static uint16 reset_port;
static uint16 bus_control_register_data_port;

static uint64 MAC0;
static uint64 MAC1;
static uint64 MAC2;
static uint64 MAC3;
static uint64 MAC4;
static uint64 MAC5;

static uint32 IP0;
static uint32 IP1;

static uint32 IP;
static uint64 MAC;

static initialization_block init_block;

static buffer_desc* send_buffer_desc;
static uint8 send_buffers[2 * 1024 + 15][8];
static uint8 send_buffer_desc_memory[2048 + 15];
static uint8 current_send_buffer;

static buffer_desc* recv_buffer_desc;
static uint8 recv_buffers[2 * 1024 + 15][8];
static uint8 recv_buffer_desc_memory[2048 + 15];
static uint8 current_recv_buffer;

static PCI_device_descriptor* device;

void amd_am79c973_analyse_status(void) {
	outportw(register_address_port, 0);
	uint16 status = inportw(register_data_port);
	
	println(itoa(status), 0x48);
	
	if ((status & 0x8000) == 0x8000) println("AMD am79c973 ERROR", 0x06);
	if ((status & 0x2000) == 0x2000) println("AMD am79c973 COLLISION ERROR", 0x06);
	if ((status & 0x1000) == 0x1000) println("AMD am79c973 MISSED FRAME", 0x06);
	if ((status & 0x0800) == 0x0800) println("AMD am79c973 MEMORY ERROR", 0x06);
	if ((status & 0x0200) == 0x0200) println("AMD am79c973 sent data", 0x0f);
	
	if ((status & 0x0400) == 0x0400) {
		println("AMD am79c973 received data", 0x0f);
		amd_am79c973_receive();
		
	}
	
	outportw(register_address_port, 0);
	outportw(register_data_port, status);
	
	if ((status & 0x0100) == 0x0100) println("AMD am79c973 init done", 0x0f);
	
}

void amd_am79c973_handler(struct registers* regs) {
	println("Interrupt recieved from AMD am79c973 network card. Analysing ...", 0x0f);
	amd_am79c973_analyse_status();
	
}

void amd_am79c973_init(PCI_device_descriptor* _device) {
	device = _device;
	
	MAC_address_0_port = device->port_base;
	MAC_address_2_port = device->port_base + 0x02;
	MAC_address_4_port = device->port_base + 0x04;

	IP_address_0_port = device->port_base + 0x08;
	IP_address_2_port = device->port_base + 0x10; // NOTE: I have no idea where to find the IP address

	register_data_port = device->port_base + 0x10;
	register_address_port = device->port_base + 0x12;

	reset_port = device->port_base + 0x14;
	bus_control_register_data_port = device->port_base + 0x16;
	
	current_send_buffer = 0;
	current_recv_buffer = 0;
	
	MAC0 = inportw(MAC_address_0_port) % 256;
	MAC1 = inportw(MAC_address_0_port) / 256;
	MAC2 = inportw(MAC_address_2_port) % 256;
	MAC3 = inportw(MAC_address_2_port) / 256;
	MAC4 = inportw(MAC_address_4_port) % 256;
	MAC5 = inportw(MAC_address_4_port) / 256;
	
	IP0 = inportw(IP_address_0_port) % 256;
	IP1 = inportw(IP_address_2_port) / 256;
	
	MAC = MAC5 << 40 \
		| MAC4 << 32 \
		| MAC3 << 24 \
		| MAC2 << 16 \
		| MAC1 << 8 \
		| MAC0;
	
	IP = IP1 << 8 \
		| IP0;
	
	outportw(register_address_port, 20);
	outportw(bus_control_register_data_port, 0x102);
	
	outportw(register_address_port, 0);
	outportw(register_data_port, 0x04);
	
	init_block.mode = 0x0000;
	
	init_block.reserved1 = 0;
	init_block.reserved2 = 0;
	init_block.reserved3 = 0;
	
	init_block.num_send_buffers = 3;
	init_block.num_recv_buffers = 3;
	
	init_block.physical_address = MAC;
	init_block.logical_address = 0;
	
	send_buffer_desc = (buffer_desc*) ((((uint32) &send_buffer_desc_memory[0]) + 15) & ~((uint32) 0xF));
	recv_buffer_desc = (buffer_desc*) ((((uint32) &recv_buffer_desc_memory[0]) + 15) & ~((uint32) 0xF));
	
	init_block.send_buffer_desc_address = (uint32) send_buffer_desc;
	init_block.recv_buffer_desc_address = (uint32) recv_buffer_desc;
	
	uint8 i;
	for (i = 0; i < 8; i++) {
		send_buffer_desc[i].address = (((uint32) &send_buffers[i]) + 15) & ~((uint32) 0xF);
		send_buffer_desc[i].flags = 0x7FF | 0xF000;
		send_buffer_desc[i].flags2 = FALSE;
		send_buffer_desc[i].avail = FALSE;
		
		recv_buffer_desc[i].address = (((uint32) &recv_buffers[i]) + 15) & ~((uint32) 0xF);
		recv_buffer_desc[i].flags = 0xF7FF | 0x80000000;
		recv_buffer_desc[i].flags2 = FALSE;
		recv_buffer_desc[i].avail = FALSE;
		
	}
	
	outportw(register_address_port, 1);
	outportw(register_data_port, (uint32) (&init_block) & 0xFFFF);
	
	outportw(register_address_port, 2);
	outportw(register_data_port, ((uint32) (&init_block) >> 16) & 0xFFFF);
	
}

void amd_am79c973_activate(void) {
	outportw(register_address_port, 0);
	outportw(register_data_port, 0x41);
	
	outportw(register_address_port, 4);
	uint16 status = inportw(register_data_port);
	
	outportw(register_address_port, 4);
	outportw(register_data_port, status | 0xC00);
	
	outportw(register_address_port, 0);
	outportw(register_data_port, 0x42);
		
}

int amd_am79c973_reset(void) {
	inportw(reset_port);
	outportw(reset_port, 0);
	
	return 10;
	
}

void amd_am79c973_send(uint8* data, int _size) {
	int send_descriptor = current_send_buffer;
	current_send_buffer = (current_send_buffer + 1) % 8;
	println("WARNING you are trying to send more than 1518 bytes of data. All data that is over that limit will be discarded.", 0x06);
	if (_size > 1518) {
		println("WARNING you are trying to send more than 1518 bytes of data. All data that is over that limit will be discarded.", 0x06);
		_size = 1518;
		
	}
	
	uint8 *source = data + _size - 1, *destination = (uint8*) (send_buffer_desc[send_descriptor].address + _size - 1);
	for (; source >= data; source--, destination--) {
		*destination = *source;
		
	}
	
	send_buffer_desc[send_descriptor].avail = FALSE;
	send_buffer_desc[send_descriptor].flags2 = FALSE;
	send_buffer_desc[send_descriptor].flags = 0x8300F000 | ((uint16) ((-_size) & 0xFFF));
	
	outportw(register_address_port, 0);
	outportw(register_data_port, 0x48);
	
}

void amd_am79c973_receive(void) {
	for (; (recv_buffer_desc[current_recv_buffer].flags & 0x80000000) == FALSE; current_recv_buffer = (current_recv_buffer + 1) % 8) {
		if (!(recv_buffer_desc[current_recv_buffer].flags & 0x40000000) && (recv_buffer_desc[current_recv_buffer].flags & 0x03000000) == 0x03000000) {
			uint32 _size = recv_buffer_desc[current_recv_buffer].flags & 0xFFF;
			
			if (_size > 64) {
				_size -= 4;
				
			}
			
			uint8* buffer = (uint8*) (recv_buffer_desc[current_recv_buffer].address);
			
			int i;
			for (i = 0; i < _size; i++) {
				print_hex(buffer[i], 0x0f);
				print(" ", 0x0f);
				
			}
			
			recv_buffer_desc[current_recv_buffer].flags2 = FALSE;
			recv_buffer_desc[current_recv_buffer].flags = 0x8000F7FF;
			
		}
		
	}
	
}

uint64 amd_am79c973_MAC_address(void) {
	return MAC;
	
}

uint32 amd_am79c973_IP_address(void) {
	return IP;
	
}
