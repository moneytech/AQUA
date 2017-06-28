/*
 * renderer.h
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


#ifndef AQUA_3D_RENDERER_H
	#define AQUA_3D_RENDERER_H
	
	#include "../../types.h"
	#include "../GFX.h"
	
	#include "float.h"
	#include "vector.h"
	#include "matrix.h"
	
	#include "mesh.h"
	#include "camera.h"
	
	typedef struct gl_batch {};
	
	typedef struct {
		uint8* back_buffer;
		uint8* writeable_buffer;
		
		uint32 width;
		uint32 height;
		
	} gl_renderer;
	
	gl_renderer gl_new_renderer(uint32 width, uint32 height, uint8* writeable_buffer);
	gl_renderer gl_clear(gl_renderer renderer, gl_colour colour);
	gl_renderer gl_present(gl_renderer renderer);
	gl_renderer gl_put_pixel(gl_renderer renderer, uint32 x, uint32 y, gl_colour colour);
	gl_vector_2d gl_project(gl_renderer renderer, gl_vector_3d coord, gl_matrix transformation_matrix);
	gl_renderer gl_draw_point(gl_renderer renderer, gl_vector_2d point);
	void gl_render(gl_renderer renderer, gl_camera camera, gl_batch batch);
	
#endif
