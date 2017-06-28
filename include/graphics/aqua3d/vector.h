/*
 * vector.h
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


#ifndef AQUA_3D_VECTOR_H
	#define AQUA_3D_VECTOR_H
	
	#include "../../types.h"
	#include "../../utils/math.h"
	
	#include "float.h"
	
	typedef struct {
		gl_float x;
		gl_float y;
		gl_float z;
		gl_float w;
		
	} gl_vector_4d; // quaternion
	
	typedef struct {
		gl_float x;
		gl_float y;
		gl_float z;
		
	} gl_vector_3d;
	
	typedef struct {
		gl_float x;
		gl_float y;
		
	} gl_vector_2d;
	
	#include "matrix.h"
	
	gl_vector_3d gl_transform_coord(gl_vector_3d coord, gl_matrix trans);
	
	gl_vector_3d gl_new_vector_3d(gl_float x, gl_float y, gl_float z);
	gl_vector_2d gl_new_vector_2d(gl_float x, gl_float y);
	
	gl_float gl_vector_dot(gl_vector_3d left, gl_vector_3d right);
	gl_float gl_vector_length(gl_vector_3d vector);
	
	gl_vector_3d gl_vector_subtract(gl_vector_3d left, gl_vector_3d right);
	gl_vector_3d gl_vector_cross(gl_vector_3d left, gl_vector_3d right);
	gl_vector_3d gl_vector_normalize(gl_vector_3d vector);
	
	gl_vector_4d gl_quaternion_rotation_YPR(gl_float yaw, gl_float pitch, gl_float roll);
	
#endif
