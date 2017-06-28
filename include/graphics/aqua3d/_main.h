/*
 * main.h
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


#ifndef AQUA_3D_MAIN_H
	#define AQUA_3D_MAIN_H
	
	#include "../../types.h"
	
	#include "float.h"
	#include "colour.h"
	
	#include "vector.h"
	#include "matrix.h"
	
	#include "mesh.h"
	#include "batch.h"
	
	#include "camera.h"
	#include "renderer.h"
	
	typedef struct gl_scene {
		gl_renderer renderer;
		gl_camera camera;
		gl_batch batch;
		gl_colour clear_colour;
		
	} gl_scene;
	
	gl_scene gl_gross_setup(uint32 width, uint32 height);
	void gl_gross_render(gl_scene scene);
	int gl_find_mesh(gl_scene scene, int64 name);
	gl_scene gl_primitive_add_cube(gl_scene scene, int64 name, gl_float scale, gl_vector_3d position, gl_vector_3d rotation);
	void gl_set_loop(void* loop);
	
#endif
