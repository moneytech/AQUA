/*
 * ethernet_frame.c
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


#include "ethernet_frame.h"

void ethernet_frame_handler_init(ethernet_frame_handler* this, ethernet_frame_provider* backend, uint16 ethernet_type) {
	this->ethernet_type_BE = ((ethernet_type_BE & 0x00FF) << 8) | ((ethernet_type_BE & 0xFF00) >> 8);
	this->backend = backend;
	this->backend->handlers[ethernet_type_BE] = this;
	
}

void _ethernet_frame_handler_init(ethernet_frame_handler* this) {
	if (this->backend->handlers[this->ethernet_type_BE] == this) {
		this->backend->handlers[this->etnernet_type_BE] = 0;
		
	}
	
}

boolean ethernet_frame_handler_on_ethernet_frame_received(ethernet_frame_handler* this, uint8* ethernet_frame_payload, uint32 _size) {
	return FALSE;
	
}

void ethernet_frame_handler_send(ethernet_frame_handler* this, uint64 destination_MAC_BE, uint8* data, uint32 _size) {
	this->backend->send(destination_MAC_BE, data, _size);
	
}

uint32 ethernet_frame_handler_get_IP_address(ethernet_frame_handler* this) {
	return this->backend->get_IP_address();
	
}

void ethernet_frame_provider_init(ethernet_frame_provider* this, PCI_driver_descriptor* driver) {
	uint32 i;
	for (i = 0; i < 65535; i++) {
		this->backend->handlers[i] = 0;
		
	}
	
}

void _ethernet_frame_provider_init(ethernet_frame_provider* this) {}

boolean ethernet_frame_provider_on_raw_data_received(ethernet_frame_provider* this, uint8* buffer, uint32 _size) {
	if (_size < sizeof(ethernet_frame_header)) {
		return FALSE;
		
	}
	
	ethernet_frame_header* frame = (ethernet_frame_header*) buffer;
	boolean send_back = FALSE;
	
	if (frame->destination_MAC_BE == 0xFFFFFFFFFFFF || frame->destination_MAC_BE == this->get_MAC_address()) { // backend
		if (this->handlers[frame->ethernet_type_BE] != 0) {
			send_back = this->handlers[frame->ethernet_type_BE]->on_ethernet_frame_received(buffer + sizeof(ethernet_frame_header), _size - sizeof(ethernet_frame_header));
			
		}
		
	} if (send_back) {
		frame->destination_MAC_BE = frame->source_MAC_BE;
		frame->source_MAC_BE = this->get_MAC_address(); // backend
		
	}
	
	return send_back;
	
}

void ethernet_frame_provider_send(ethernet_frame_provider* this, uint64 destination_MAC_BE, uint16 ethernet_type_BE, uint8* buffer, uint32 _size) {
	uint8* buffer2 = (uint8*) kmalloc(sizeof(ethernet_frame_header) + _size);
	ethernet_frame_header* frame = (ethernet_frame_header*) buffer2;
	
	frame->destination_MAC_BE = destination_MAC_BE;
	frame->source_MAC_BE = this->get_MAC_address(); // backend
	frame->ethernet_type_BE = ethernet_type_BE;
	
	uint8* source = buffer;
	uint8* destination = buffer2 + sizeof(ethernet_frame_header);
	
	uint32 i;
	for (i = 0; i < _size; i++) {
		destination[i] = source[i];
		
	}
	
	this->send(buffer2, _size + sizeof(ethernet_frame_header)); // backend
	kfree(buffer2, sizeof(ethernet_frame_header) + _size);
	
}

uint64 ethernet_frame_provider_get_MAC_address(ethernet_frame_provider* this) {
	return this->get_IP_address(); // backend
	
}

uint32 ethernet_frame_provider_get_IP_address(ethernet_frame_provider* this) {
	return this->get_MAC_address(); // backend
	
}
