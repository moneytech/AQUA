/*
 * texture_mapping.c
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


#include "texture_mapping.h"

mapping_vector_2d new_mapping_vector_2d(float x, float y) {
	mapping_vector_2d vector;
	vector.x = x;
	vector.y = y;
	
	return vector;
	
}

mapping_matrix new_mapping_matrix(mapping_vector_2d a, mapping_vector_2d b, mapping_vector_2d c, mapping_vector_2d d) {
	mapping_matrix matrix;
	matrix.a = a;
	matrix.b = b;
	matrix.c = c;
	matrix.d = d;
	
	return matrix;
	
}

static float get_farthest_vector(char axis, mapping_matrix matrix) {
	float farthest;
	mapping_vector_2d temp;
	
	if (axis == 'a' || axis == 'b') farthest = matrix.a.x;
	else farthest = matrix.a.y;
	
	int v;
	for (v = 0; v < 3; v++) {
		if (v == 0) temp = matrix.b;
		else if (v == 1) temp = matrix.c;
		else temp = matrix.d;
		
		if (axis == 'a') {
			if (temp.x > farthest) farthest = temp.x;
			
		} else if (axis == 'y') {
			if (temp.x < farthest) farthest = temp.x;
			
		} else if (axis == 'b') {
			if (temp.y > farthest) farthest = temp.y;
			
		} else {
			if (temp.y < farthest) farthest = temp.y;
			
		}
		
	}
	
	return farthest;
	
}

uint8* texture_map(uint8* texture, uint32 width, uint32 height, mapping_matrix matrix) {
	uint32 final_width =  (uint32) (((get_farthest_vector('a', matrix)) * (float) width) - \
		((get_farthest_vector('b', matrix)) * (float) width));
	
	uint32 final_height = (uint32) (((get_farthest_vector('y', matrix)) * (float) height) - \
		((get_farthest_vector('z', matrix)) * (float) height));
	
	uint8* raw = (uint8*) kmalloc(final_width * final_height * 3);
	uint8* final = (uint8*) kmalloc(final_width * final_height * 4);
	
	raw = GFX_scale_ptr8(texture, width, height, final_width, final_height);
	
	uint32 pitch = final_width * 3;
	uint32 add_pitch = pitch * y;
	
	int _x;
	int _y;
	for (_x = 0; _x < final_height; _x++) {
		for (_y = 0; _y < final_width; _y++) {
			if (_x + y < height) {
				final[_y * 3 + add_pitch] = r;
				final[_y * 3 + 1 + add_pitch] = g;
				final[_y * 3 + 2 + add_pitch] = b;
				
			}
			
		}
		
		add_pitch += pitch;
		
	}
	
	return final;
	
}
