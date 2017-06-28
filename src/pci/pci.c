/*
 * pci.c
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

#include "pci.h"

uint16 PCI_read_word(uint8 bus, uint8 slot, uint8 func, uint8 offset) {
	uint32 lbus = (uint32) bus;
	uint32 lslot = (uint32) slot;
	uint32 lfunc = (uint32) func;
	
	uint32 address = (uint32) ((lbus << 16) | (lslot << 11) | (lfunc << 8) | (offset & 0xfc) | ((uint32) 0x80000000));
	outportl(0xCF8, address);
	uint16 tmp = (uint16) ((inportl(0xCFC) >> ((offset & 2) * 8)) & 0xffff);
	
	return tmp;
	
}

uint32 PCI_read_dword(uint8 bus, uint8 slot, uint8 func, uint8 offset) {
	uint32 lbus = (uint32) bus;
	uint32 lslot = (uint32) slot;
	uint32 lfunc = (uint32) func;
	
	uint32 address = (uint32) ((lbus << 16) | (lslot << 11) | (lfunc << 8) | (offset & 0xfc) | ((uint32) 0x80000000));
	outportl(0xCF8, address);
	uint32 tmp = (uint32) ((inportl(0xCFC) >> ((offset & 2) * 8)) & 0xffff);
	
	return tmp;
	
}

uint16 get_PCI_vendor(uint8 bus, uint8 slot, uint8 function) {
	uint16 vendor = PCI_read_word(bus, slot, function, 0);
	uint16 device;
	
	if (vendor != 0xFFFF) {
		device = PCI_read_word(bus, slot, 0, 2);
		
	}
	
	return vendor;
	
}

PCI_device get_PCI_device(uint8 tclass, uint8 tsub_class, uint8 tprog_IF) {
	uint16 class_and_sub;
	uint8 class;
	uint8 sub_class;
	uint8 prog_IF;
	
	PCI_device pci_device = {.bus = 0, .slot = 0, .flags = 0};
	boolean found_device = FALSE;
	
	int bus;
	int device;
	for (bus = 0; bus < 256; bus++) {
		for (device = 0; device < 32; device++) {
			if (get_PCI_vendor(bus, device, 0) != 0xFFFF) {
				class_and_sub = PCI_read_word(bus, device, 0, 10);
				class = (uint8) (class_and_sub >> 8);
				sub_class = (uint8) class_and_sub;
				prog_IF = (uint8) (PCI_read_word(bus, device, 0, 8) >> 8);
				
				if (class == tclass && sub_class == tsub_class && prog_IF == tprog_IF) {
					if (!found_device) {
						pci_device.bus = bus;
						pci_device.slot = device;
						pci_device.flags |= 0b00000001;
						
					} else {
						pci_device.flags |= 0b00000010;
						
					}
					
				}
				
			}
			
		}
		
	}
	
	return pci_device;
	
}

uint32 PCI_make_addr(uint16 bus, uint16 slot, uint16 func, uint16 offset) {
	uint32 lbus = (uint32) bus;
	uint32 lslot = (uint32) slot;
	uint32 lfunc = (uint32) func;
	
	return (uint32) ((lbus << 16) | (lslot << 11) | (lfunc << 8) | (offset & 0xfc) | ((uint32) 0x80000000));
	
}

void PCI_write_long(uint16 bus, uint16 slot, uint16 func, uint16 offset, uint32 data) {
	uint32 address = PCI_make_addr(bus, slot, func, offset);
	outportl(PCI_DATA_PORT, address);
	outportl(PCI_COMMAND_PORT, data);
	
}

void PCI_debug(void) {
	println("PCI: Checking all PCI buses ...", 0x0f);
	
	int bus;
	int device;
	for (bus = 0; bus < 256; bus++) {
		for (device = 0; device < 32; device++) {
			if (get_PCI_vendor(bus, device, 0) != 0xFFFF) {
				print("\tPCI: Found ", 0x07);
				
				uint16 class_and_sub = PCI_read_word(bus, device, 0, 10);
				print_PCI_class_code((uint8) (class_and_sub >> 8), (uint8) class_and_sub, (uint8) (PCI_read_word(bus, device, 0, 8) >> 8));
				
				print(" at 0x", 0x07);
				print_hex(bus, 0x07);
				
				print(", 0x", 0x07);
				print_hex(device, 0x07);
				
				if (((uint8) (PCI_read_word(bus, device, 0, 14) & 0xFF)) & 0x80 != 0) {
					println(" with multiple functions.", 0x07);
					
				}
				
				printch('\n', 0x07);
				
			}
			
		}
		
	}
	
}

void print_PCI_class_code(uint8 class_code, uint8 sub_class, uint8 prog_IF) {
	switch (class_code) {
		case 0x1: {
			print("Mass Storage Controller", 0x07);
			break;
			
		} case 0x2: {
			print("Network Controller", 0x07);
			break;
			
		} case 0x3: {
			print("Display Controller", 0x07);
			break;
			
		} case 0x4: {
			print("Mutimedia Controller", 0x07);
			break;
			
		} case 0x5: {
			print("Memory Controller", 0x07);
			break;
			
		} case 0x6: {
			print("Bridge Device", 0x07);
			break;
			
		} case 0x7: {
			print("Simple Communication Controller", 0x07);
			break;
			
		} case 0x8: {
			print("Base System Peripheral", 0x07);
			break;
			
		} case 0x9: {
			print("Input Device", 0x07);
			break;
			
		} case 0xA: {
			print("Docking Station", 0x07);
			break;
			
		} case 0xB: {
			print("Processor", 0x07);
			break;
			
		} case 0xC: {
			print("Serial Bus Controller", 0x07);
			break;
			
		} case 0xD: {
			print("Wireless Controller", 0x07);
			break;
			
		} case 0xE: {
			print("Intelligent IO Controller", 0x07);
			break;
			
		} case 0xF: {
			print("Satellite Communication Controller", 0x07);
			break;
			
		} case 0x10: {
			print("Encryption Controller", 0x07);
			break;
			
		} case 0x11: {
			print("Data Acquisition and Signal Processing Controller", 0x07);
			break;
			
		} default: {
			print("Other Device (", 0x07);
			print_hex(class_code, 0x07);
			printch(')', 0x07);
			
		}
		
	}
	
	print(" (Sub ", 0x07);
	print_hex(sub_class, 0x07);
	
	print(", ", 0x07);
	print_hex(prog_IF, 0x07);
	printch(')', 0x07);
	
}

static uint32 get_PCI_id(uint16 bus, uint16 device, uint16 function, uint32 register_offset) {
	return 0x1 << 31 | ((bus & 0xFF) << 16) | ((device & 0x1F) << 11) | ((function & 0x07) << 8) | (register_offset & 0xFC);
	
}

uint32 PCI_read(uint16 bus, uint16 device, uint16 function, uint32 register_offset) {
	uint32 id = get_PCI_id(bus, device, function, register_offset);
	outportl(PCI_COMMAND_PORT, id);
	uint32 result = inportl(PCI_DATA_PORT);
	
	return result >> (8 * (register_offset % 4));
	
}

void PCI_write(uint16 bus, uint16 device, uint16 function, uint32 register_offset, uint32 value) {
	uint32 id = get_PCI_id(bus, device, function, register_offset);
	outportl(PCI_COMMAND_PORT, id);
	outportl(PCI_DATA_PORT, value);
	
}

boolean PCI_device_has_functions(uint16 bus, uint16 device) {
	return (boolean) (PCI_read(bus, device, 0, 0x0E) & (1 << 7));
	
}

PCI_device_descriptor PCI_get_device_descriptor(uint16 bus, uint16 device, uint16 function) {
	PCI_device_descriptor result;
	
	result.bus = bus;
	result.device = device;
	result.function = function;
	
	result.vendor_id = PCI_read(bus, device, function, 0x00);
	result.device_id = PCI_read(bus, device, function, 0x02);
	
	result.class_id = PCI_read(bus, device, function, 0x0B);
	result.subclass_id = PCI_read(bus, device, function, 0x0A);
	result.interface_id = PCI_read(bus, device, function, 0x09);
	
	result.revision = PCI_read(bus, device, function, 0x08);
	result.interrupt = PCI_read(bus, device, function, 0x3C);
	
	return result;
	
}

base_address_register PCI_get_base_address_register(uint16 bus, uint16 device, uint16 function, uint16 bar) {
	base_address_register result;
	
	uint32 header_type = PCI_read(bus, device, function, 0x0E) & 0x7F;
	int max_BARs = 6 - (4 * header_type);
	
	if (bar >= max_BARs) {
		return result;
		
	}
	
	uint32 bar_value = PCI_read(bus, device, function, 0x10 + 4 * bar);
	result.type = (bar_value & 0x1) ? input_output : memory_mapping;
	uint32 temp;
	
	if (result.type == memory_mapping) {
		switch ((bar_value >> 1) & 0x3) {
			case 0: // 32 bit
			case 1: // 20 bit
			case 2: { // 64 bit
				break;
				
			}
			
		}
		
	} else {
		result.address = (uint8*) (bar_value & ~0x3);
		result.prefetchable = FALSE;
		
	}
	
	return result;
	
}

PCI_driver_descriptor PCI_get_driver(PCI_device_descriptor device) {
	PCI_driver_descriptor result;
	
	result.vendor_name = "unknown";
	result.device_name = "unknown";
	result.device_type = "unknown";
	
	switch (device.vendor_id) {
		case 0x05AC:
		case 0x106B: { // Apple CRASH THE COMPUTER
			println("ERROR an apple device was detected!!! The computer will now crash ...", 0x04);
			asm volatile("int $0x80");
			break;
			
		} case 0x1022:
		case 0x1002: { // AMD
			println("AMD vendor detected.", 0x0f);
			result.vendor_name = "AMD";
			
			switch (device.device) {
				case 0x2000:
				case 0x0003: { // this is for a bug in vb
					println("PCNet network card detected (assuming its an AM79C973).", 0x0f);
					result.device_name = "AM79C973";
					result.device_type = "Network Controller";
					
					break;
					
				} default: {
					println("WARNING unknown AMD device detected.", 0x06);
					break;
					
				}
				
			}
			
			break;
			
		} case 0x8087:
		case 0x8086: { // Intel
			println("Intel vendor detected.", 0x0f);
			result.vendor_name = "Intel";
			
			switch (device.device) {
				case 0x1977:
				case 0x27D8:
				case 0x0A0C:
				case 0x0F04:
				case 0x1C20:
				case 0x1E20:
				case 0x3B56:
				case 0x9C20: {
					println("High Definition audio controller card detected.", 0x0f);
					result.device_name = "High Definition";
					result.device_type = "Multimedia Controller";
					
					break;
					
				} case 0x2440: {
					println("ICH2 audio controller card detected.", 0x0f);
					result.device_name = "ICH2";
					result.device_type = "Multimedia Controller";
					
					break; 
					
				} case 0x24D5: {
					println("ICH5 audio controller card detected.", 0x0f);
					result.device_name = "ICH5";
					result.device_type = "Multimedia Controller";
					
					break;
					
				} case 0x2668:
				case 0x2678: {
					println("ICH6 audio controller card detected.", 0x0f);
					result.device_name = "ICH6";
					result.device_type = "Multimedia Controller";
					
					break;
					
				} case 0x27: {
					println("ICH7 audio controller card detected.", 0x0f);
					result.device_name = "ICH7";
					result.device_type = "Multimedia Controller";
					
					break;
					
				} case 0x293E: {
					println("ICH9 audio controller card detected.", 0x0f);
					result.device_name = "ICH9";
					result.device_type = "Multimedia Controller";
					
					break;
					
				} default: {
					println("WARNING unknown Intel device detected.", 0x06);
					break;
					
				}
				
			}
			
			break;
			
		} case 0x10DE: { // NVIDIA
			println("NVIDIA vendor detected.", 0x0f);
			result.vendor_name = "NVIDIA";
			
			switch (device.device) {
				default: {
					println("WARNING unknown NVIDIA device detected.", 0x06);
					break;
					
				}
				
			}
			
			break;
			
		} case 0x5143: { // Qualcomm
			println("Qualcomm vendor detected.", 0x0f);
			result.vendor_name = "Qualcomm";
			
			switch (device.device) {
				default: {
					println("WARNING unknown Qualcomm device detected.", 0x06);
					break;
					
				}
				
			}
			
			break;
			
		} case 0x10EC: { // Realtek
			println("Realtek vendor detected.", 0x0f);
			result.vendor_name = "Realtek";
			
			switch (device.device) {
				default: {
					println("WARNING unknown Realtek device detected.", 0x06);
					break;
					
				}
				
			}
			
			break;
			
		} default: {
			//println("WARNING unknown vendor detected.", 0x06);
			break;
			
		}
		
	} switch (device.class_id) {
		case 0x03: { // graphics
			println("graphics card detected.", 0x0f);
			result.device_type = "Graphics card";
			
			switch (device.subclass_id) {
				case 0x00: { // VGA
					println("VGA graphics card detected.", 0x0f);
					result.device_name = "VGA";
					
					break;
					
				} default: {
					println("WARNING unknown graphics card detected.", 0x06);
					break;
					
				}
				
			}
			
			break;
			
		} default: {
			//println("WARNING unknown device detected.", 0x06);
			break;
			
		}
		
	}
	
	return result;
	
}


