/*
 * network_wrapper.c
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

#include "network_wrapper.h"

static int64 card_id;
static PCI_device_descriptor network_card;

boolean networking_get_card(void) {
	boolean networking = FALSE; 
	PCI_driver_descriptor driver;
	
	PCI_device_descriptor device_descriptor;
	base_address_register bar;
	int function_count;
	
	int bus;
	int device;
	int function;
	int bar_num;
	
	for (bus = 0; bus < 8; bus++) {
		for (device = 0; device < 32; device++) {
			function_count = PCI_device_has_functions(bus, device) ? 8 : 1;
			
			for (function = 0; function < function_count; function++) {
				device_descriptor = PCI_get_device_descriptor(bus, device, function);
				
				if (device_descriptor.vendor_id == 0x0000 || device_descriptor.vendor_id == 0xFFFF) {
					continue;
					
				}
				
				for (bar_num = 0; bar_num < 6; bar_num++) {
					bar = PCI_get_base_address_register(bus, device, function, bar_num);
					
					if (bar.address && (bar.type == input_output)) {
						device_descriptor.port_base = (uint32) bar.address;
						
					}
					
				}
				
			}
			
			driver = PCI_get_driver(device_descriptor);
			
			if (str_eql(driver.device_type, "Network Controller")) {
				networking = TRUE;
				network_card = device_descriptor;
				
				println("A network controller was found.", 0x0f);
				
				print("\tVENDOR: ", 0x07);
				println(driver.vendor_name, 0x07);
				
				print("\tDEVICE NAME: ", 0x07);
				println(driver.device_name, 0x07);
				
				if (str_eql(driver.device_name, "AM79C973")) card_id = CARD_AMD_AM79C973;
				else if (str_eql(driver.device_name, "I8254X")) card_id = CARD_I8254X;
				else if (str_eql(driver.device_name, "RTL8139")) card_id = CARD_RTL8139;
				
				else {
					println("WARNING discovered network controller is unknown. Continuing to search ...", 0x06);
					networking = FALSE;
					
				} if (networking) break;
				
			}
			
		} if (networking) break;
		
	}
	
	if (networking) {
		network_card = device_descriptor;
		
	} else {
		println("WARNING no networking card was discovered.", 0x06);
		
	}
	
	return networking;
	
}

PCI_device_descriptor networking_get_PCI_device(void) {
	return network_card;
	
}

void networking_analyse_status(void) {
	switch (card_id) {
		case CARD_AMD_AM79C973: {
			amd_am79c973_analyse_status();
			break;
			
		} default: {
			break;
			
		}
		
	}
	
}

void networking_install(void) {
	switch (card_id) {
		case CARD_AMD_AM79C973: {
			amd_am79c973_init(&network_card);
			//irq_add_handler(network_card.interrupt, &amd_am79c973_handler);
			break;
			
		} case CARD_I8254X: {
			//i8254x_init(&network_card);
			//irq_add_handler(network_card.interrupt, &i8254x_handler);
			break;
			
		} case CARD_RTL8139: {
			//rtl8139_init(&network_card);
			//irq_add_handler(network_card.interrupt, &rtl8139_handler);
			break;
			
		} default: {
			break;
			
		}
		
	}
	
}

void networking_activate(void) {
	switch (card_id) {
		case CARD_AMD_AM79C973: {
			amd_am79c973_activate();
			break;
			
		} default: {
			break;
			
		}
		
	}
	
}

void networking_send(uint8* data, int _size) {
	switch (card_id) {
		case CARD_AMD_AM79C973: {
			amd_am79c973_send(data, _size);
			break;
			
		} default: {
			break;
			
		}
		
	}
	
}

uint64 networking_get_MAC_address(void) {
	switch (card_id) {
		case CARD_AMD_AM79C973: {
			return amd_am79c973_MAC_address();
			break;
			
		} default: {
			return 0;
			break;
			
		}
		
	}
	
}

uint32 networking_get_IP_address(void) {
	switch (card_id) {
		case CARD_AMD_AM79C973: {
			return amd_am79c973_IP_address();
			break;
			
		} default: {
			return 0;
			break;
			
		}
		
	}
	
}
