/*
 * ethernet_frame.h
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


#ifndef NET_ETHERNET_FRAME_H
	#define NET_ETHERNET_FRAME_H
	
	#include "../../types.h"
	#include "../network_wrapper.h"
	#include "../../memory/memory.h"
	
	typedef struct {
		uint64 destination_MAC_BE : 48;
		uint64 source_MAC_BE : 48;
		uint16 ethernet_type_BE;
		
	} ethernet_frame_header;
	
	typedef uint32 ethernet_frame_footer;
	
	struct ethernet_frame_provider_struct;
	struct ethernet_frame_handler_struct;
	
	typedef void (*ethernet_frame_handler_type) (struct ethernet_frame_handler_struct*, struct ethernet_frame_provider_struct*, uint16);
	typedef void (*_ethernet_frame_handler_type) (struct ethernet_frame_handler_struct*);
	
	typedef boolean (*on_ethernet_frame_received_type) (struct ethernet_frame_handler_struct*, uint8*, uint32);
	typedef void (*ethernet_frame_handler_send_type) (struct ethernet_frame_handler_struct*, uint64, uint8*, uint32);
	
	typedef uint32 (*ethernet_frame_handler_get_IP_address_type) (struct ethernet_frame_handler_struct*);
	
	typedef struct ethernet_frame_handler_struct {
		struct ethernet_frame_provider_struct* backend;
		uint16 ethernet_type_BE;
		
		ethernet_frame_handler_type init;
		_ethernet_frame_handler_type _init;
		
		on_ethernet_frame_received_type on_ethernet_frame_received;
		ethernet_frame_handler_send_type send;
		ethernet_frame_handler_get_IP_address_type get_IP_address;
		
	} ethernet_frame_handler;
	
	typedef void (*ethernet_frame_provider_type) (struct ethernet_frame_provider_struct*, PCI_driver_descriptor*);
	typedef void (*_ethernet_frame_provider_type) (struct ethernet_frame_provider_struct*);
	
	typedef boolean (*ethernet_on_raw_data_received_type) (struct ethernet_frame_provider_struct*, uint8*, uint32);
	typedef void (*ethernet_frame_provider_send_type) (struct ethernet_frame_provider_struct*, uint64, uint16, uint8*, uint32);
	
	typedef uint64 (*ethernet_frame_provider_get_MAC_address_type) (struct ethernet_frame_provider_struct*);
	typedef uint32 (*ethernet_frame_provider_get_IP_address_type) (struct ethernet_frame_provider_struct*);
	
	typedef struct ethernet_frame_provider_struct {
		ethernet_frame_handler* handlers[65535];
		
		ethernet_frame_provider_type init;
		_ethernet_frame_provider_type _init;
		
		ethernet_on_raw_data_received_type on_raw_data_received;
		ethernet_frame_provider_send_type send;
		
		ethernet_frame_provider_get_MAC_address_type get_MAC_address;
		ethernet_frame_provider_get_IP_address_type get_IP_address;
		
	} ethernet_frame_provider;
	
	void ethernet_frame_handler_init(ethernet_frame_handler* this, ethernet_frame_provider* backend, uint16 ethernet_type);
	void _ethernet_frame_handler_init(ethernet_frame_handler* this);
	
	boolean ethernet_frame_handler_on_ethernet_frame_received(ethernet_frame_handler* this, uint8* ethernet_frame_payload, uint32 _size);
	void ethernet_frame_handler_send(ethernet_frame_handler* this, uint64 destination_MAC_BE, uint8* data, uint32 _size);
	uint32 ethernet_frame_handler_get_IP_address(ethernet_frame_handler* this);
	
	void ethernet_frame_provider_init(ethernet_frame_provider* this, PCI_driver_descriptor* driver);
	void _ethernet_frame_provider_init(ethernet_frame_provider* this);
	
	boolean ethernet_frame_provider_on_raw_data_received(ethernet_frame_provider* this, uint8* buffer, uint32 _size);
	void ethernet_frame_provider_send(ethernet_frame_provider* this, uint64 destination_MAC_BE, uint16 ethernet_type_BE, uint8* buffer, uint32 _size);
	
	uint64 ethernet_frame_provider_get_MAC_address(ethernet_frame_provider* this);
	uint32 ethernet_frame_provider_get_IP_address(ethernet_frame_provider* this);
	
#endif
