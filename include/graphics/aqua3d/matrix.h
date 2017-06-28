/*
 * matrix.h
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


#ifndef AQUA_3D_MATRIX_H
	#define AQUA_3D_MATRIX_H
	
	#include "../../types.h"
	#include "../../utils/math.h"
	
	#include "float.h"
	#include "vector.h"
	
	#define GL_M11 1 - 1
	#define GL_M12 2 - 1
	#define GL_M13 3 - 1
	#define GL_M14 4 - 1
	
	#define GL_M21 2 - 1
	#define GL_M22 4 - 1
	#define GL_M23 6 - 1
	#define GL_M24 8 - 1
	
	#define GL_M31 3 - 1
	#define GL_M32 6 - 1
	#define GL_M33 9 - 1
	#define GL_M34 12 - 1
	
	#define GL_M41 4 - 1
	#define GL_M42 8 - 1
	#define GL_M43 12 - 1
	#define GL_M44 16 - 1
	
	gl_matrix gl_look_at_LH(gl_vector_3d eye, gl_vector_3d target, gl_vector_3d up);
	gl_matrix gl_perspective_FOV_RH(gl_float fov, gl_float aspect, gl_float clipping_near, gl_float clipping_far);
	gl_matrix gl_quaternion_rotation(gl_vector_4d rotation);
	gl_matrix gl_matrix_rotation_YPR(gl_float yaw, gl_float pitch, gl_float roll);
	gl_matrix gl_matrix_translation(gl_vector_3d vector);
	gl_matrix gl_matrix_multiply(gl_matrix _1, gl_matrix _2, gl_matrix _3, int length);
	
#endif
