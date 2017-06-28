/*
 * main.c
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


#include "main.h"

gl_scene gl_gross_setup(uint32 width, uint32 height) {
	gl_scene scene;
	scene.clear_colour = gl_new_colour(0.0f, 0.0f, 0.0f);
	
	scene.renderer = gl_new_renderer(width, height, (uint8*) kmalloc(width * height * 3));
	scene.camera = gl_new_camera(gl_new_vector_3d(0, 0, 10), gl_new_vector_3d(0, 0, 0), 0.78f, 0.01f, 1.0f);
	scene.batch = gl_new_batch();
	
	return scene;
	
}

void gl_gross_render(gl_scene scene) {
	gl_clear(scene.renderer, scene.clear_colour);
	
	gl_render(scene.renderer, scene.camera, scene.batch);
	gl_present(scene.renderer);
	
}

int gl_find_mesh(gl_scene scene, int64 name) {
	int m;
	for (m = 0; m < scene.batch.mesh_count; m++) {
		if (name == scene.batch.meshes[m].name) {
			return m;
			
		}
		
	}
	
	return 0;
	
}

gl_scene gl_primitive_add_cube(gl_scene scene, int64 name, gl_float scale, gl_vector_3d position, gl_vector_3d rotation) {
	gl_mesh cube = gl_new_mesh(name, 8);
	
	cube.vertices[0] = gl_new_vector_3d(-scale, scale, scale);
	cube.vertices[1] = gl_new_vector_3d(scale, scale, scale);
	cube.vertices[2] = gl_new_vector_3d(-scale, -scale, scale);
	cube.vertices[3] = gl_new_vector_3d(-scale, -scale, -scale);
	cube.vertices[4] = gl_new_vector_3d(-scale, scale, -scale);
	cube.vertices[5] = gl_new_vector_3d(scale, scale, -scale);
	cube.vertices[6] = gl_new_vector_3d(scale, -scale, scale);
	cube.vertices[7] = gl_new_vector_3d(scale, -scale, -scale);
	
	scene.batch = gl_add_to_batch(scene.batch, cube);
	return scene;
	
}

void gl_set_loop(void* loop) {
	
	
}
