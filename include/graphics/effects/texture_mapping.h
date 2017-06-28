/*
 * texture_mapping.h
 * 
 * Copyright 2017 obiwac
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


#ifndef TEXTURE_MAPPING_H
	#define TEXTURE_MAPPING_H
	
	#include "../../types.h"
	#include "../GFX.h"
	
	typedef struct {
		float x;
		float y;
		
	} mapping_vector_2d;
	
	typedef struct {
		mapping_vector_2d a;
		mapping_vector_2d b;
		mapping_vector_2d c;
		mapping_vector_2d d;
		
	} mapping_matrix;
	
	mapping_vector_2d new_mapping_vector_2d(float x, float y);
	mapping_matrix new_mapping_matrix(mapping_vector_2d a, mapping_vector_2d b, mapping_vector_2d c, mapping_vector_2d d);
	
	uint8* texture_map(uint8* texture, uint32 width, uint32 height, mapping_matrix matrix);
	
#endif
