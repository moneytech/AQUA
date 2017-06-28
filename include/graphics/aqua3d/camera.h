/*
 * camera.h
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


#ifndef AQUA_3D_CAMERA_H
	#define AQUA_3D_CAMERA_H
	
	#include "../../types.h"
	
	#include "float.h"
	#include "vector.h"
	
	typedef struct {
		gl_vector_3d position;
		gl_vector_3d target;
		
		gl_float fov; // 0.78f
		gl_float clipping_near; // 0.01f
		gl_float clipping_far; // 1.0f
		
	} gl_camera;
	
	gl_camera gl_new_camera(gl_vector_3d position, gl_vector_3d target, gl_float fov, gl_float clipping_near, gl_float clipping_far);
	
#endif
