/*
 * batch.h
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
	
	#include "float.h"
	#include "vector.h"
	#include "mesh.h"
	
	typedef struct {
		gl_mesh* meshes;
		int mesh_count;
		
	} gl_batch;
	
	gl_batch gl_new_batch(void);
	gl_batch gl_add_to_batch(gl_batch batch, gl_mesh mesh);
	
#endif
