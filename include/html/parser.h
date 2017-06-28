/*
 * parser.h
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


#ifndef HTML_PARSER_H
	#define HTML_PARSER_H
	
	#include "../types.h"
	#include "../graphics/GFX.h"
	#include "../utils/string.h"
	
	#define HTML_H1 1
	#define HTML_H2 2
	#define HTML_H3 3
	#define HTML_H4 4
	#define HTML_H5 5
	#define HTML_H6 6
	
	#define HTML_P 7
	#define HTML_A 8
	
	#define HTML_HTML 9
	#define HTML_HEAD 10
	#define HTML_BODY 11
	
	#define HTML_TITLE 12
	
	#define HTML_I 13
	#define HTML_B 14
	#define HTML_U 15
	
	#define HTML_POSITION_PACKED 1
	#define HTML_POSITION_ABSOLUTE 2
	
	#define HTML_DEFAULT_BACKGROUND_COLOUR 0x00292929
	#define HTML_DEFAULT_FOREGROUND_COLOUR 0x00FFFFFF
	#define HTML_DEFAULT_ANNOTATION_COLOUR 0x0000C8FF
	
	struct html_element_s;
	
	typedef struct {
		uint32 x;
		uint32 y;
		uint8 type;
		
	} html_position;
	
	typedef struct {
		struct html_element_s* elements;
		int element_count;
		string code;
		
	} html_element_tree;
	
	typedef struct html_element_s {
		uint16 id;
		
		uint16* tags;
		int tag_count;
		
		uint32 colour;
		html_position pos;
		
		html_element_tree nest;
		
	} html_element;
	
	typedef struct {
		uint32 background_colour;
		
		uint32 width;
		uint32 height;
		
		uint8* surface;
		html_element_tree tree;
		
	} html_page;
	
	void init_html_parser(
		uint8** _font_15px, \
		uint8** _font_20px, \
		uint8** _font_50px \
		\
	);
	
	html_page html_new_page(uint32 width, uint32 height);
	html_page html_parse_code(html_page page, string code);
	
	html_page html_render_page(html_page page);
	void html_display_page(uint32 x, uint32 y, html_page page);
	
#endif
