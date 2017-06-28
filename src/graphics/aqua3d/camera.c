/*
 * camera.c
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


#include "camera.h"

gl_camera gl_new_camera(gl_vector_3d position, gl_vector_3d target, gl_float fov, gl_float clipping_near, gl_float clipping_far) {
	gl_camera camera;
	
	camera.position = position;
	camera.target = target;
	
	camera.fov = fov;
	camera.clipping_near = clipping_near;
	camera.clipping_far = clipping_far;
	
	return camera;
	
}
