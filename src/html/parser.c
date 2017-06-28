/*
 * parser.c
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


#include "parser.h"

static uint8** font_15px; // 257
static uint8** font_20px; // 439
static uint8** font_50px; // 2681

static uint32 common_packed_pointer;
static string title;

void init_html_parser(
	uint8** _font_15px, \
	uint8** _font_20px, \
	uint8** _font_50px \
	\
) {
	font_15px = _font_15px;
	font_20px = _font_20px;
	font_50px = _font_50px;
	
}

html_page html_new_page(uint32 width, uint32 height) {
	uint8* surface = (uint8*) kmalloc(width * height * 3);
	surface = GFX_fill_rect_hex_to_ptr8(surface, width, 0, 0, width, height, HTML_DEFAULT_BACKGROUND_COLOUR);
	
	html_page page = {
		.background_colour = HTML_DEFAULT_BACKGROUND_COLOUR,
		.surface = surface,
		
		.width = width,
		.height = height,
		
	};
	
	return page;
	
}

static html_element_tree html_parse_tree_code(string code) {
	int nest_index = 0;
	
	int c;
	for (c = 0; c < str_len(code); c++) {
		//code[c];
		
	}
	
}

html_page html_parse_code(html_page page, string code) {
	page.tree = html_parse_tree_code(code);
	
}

static uint8* html_render_element(uint32 width, uint32 height, uint8* surface, html_element element) {
	uint32 x;
	uint32 y;
	
	if (element.pos.type == HTML_POSITION_ABSOLUTE) {
		x = element.pos.x;
		y = element.pos.y;
		
	} else {
		x = 10;
		y = common_packed_pointer + 10;
		
	}
	
	switch (element.id) {
		case HTML_H1:
		case HTML_H2:
		case HTML_H3:
		case HTML_H4:
		case HTML_H5:
		case HTML_H6: {
			break;
			
		} case HTML_A: {
			break;
			
		} case HTML_TITLE: {
			title = element.nest.code;
			break;
			
		} case HTML_P:
		default: {
			//surface = GFX_blit_text_hex_to_ptr8(surface, width, x, y, 0, 257, font_15px, element, element.colour);
			break;
			
		}
		
	}
	
}

static uint8* html_render_tree(uint32 width, uint32 height, uint8* surface, html_element_tree tree) {
	int e;
	for (e = 0; e < tree.element_count; e++) {
		surface = html_render_element(width, height, surface, tree.elements[e]);
		
	}
	
	return surface;
	
}

html_page html_render_page(html_page page) {
	common_packed_pointer = 0;
	page.surface = GFX_fill_rect_hex_to_ptr8(page.surface, page.width, 0, 0, page.width, page.height, page.background_colour);
	page.surface = html_render_tree(page.width, page.height, page.surface, page.tree);
	
	return page;
	
}

void html_display_page(uint32 x, uint32 y, html_page page) {
	GFX_blit_image(x, y, page.width, page.height, page.surface);
	
}

