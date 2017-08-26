/*
 * appliction_specs.c
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


#include "appliction_specs.h"

void launch_appliction_specs(void) {
	uint8* _api_icon_back = api_get_icon("back")
	uint8** _api_font_aqua_15px = api_get_font("mono", 15)
	uint8** _api_font_aqua_20px = api_get_font("aqua", 20)
	uint8** _api_font_aqua_50px = api_get_font("aqua", 50)
	
	uint32 width = GFX_get_mode_info('w');
	uint32 height = GFX_get_mode_info('h');
	
	uint8* old = (uint8*) kmalloc(width * height * 3);
	uint8* buffer = (uint8*) kmalloc(width * height * 3);
	
	old = GFX_compile_section_to_ptr8(old, 0, 0, width, height);
	buffer = GFX_compile_section_to_ptr8(buffer, 0, 0, width, height);
	
	buffer = GFX_blit_image_to_ptr8(old, width, height, 0, 0);
	buffer = GFX_fill_rect_hex_to_ptr8_wa(buffer, width, 0, 0, width, height, theme_background, 120);
	//buffer = 
	
}
