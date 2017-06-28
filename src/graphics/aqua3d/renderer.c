/*
 * renderer.c
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


#include "renderer.h"

gl_renderer gl_new_renderer(uint32 width, uint32 height, uint8* writeable_buffer) {
	gl_renderer renderer;
	renderer.width = width;
	renderer.height = height;
	renderer.writeable_buffer = writeable_buffer;
	
	return renderer;
	
}

gl_renderer gl_clear(gl_renderer renderer, gl_colour colour) {
	uint32 hex = GFX_compile_rgb_to_hex(colour.r, colour.g, colour.b);
	renderer.back_buffer = GFX_fill_rect_hex_to_ptr8(renderer.back_buffer, renderer.width, 0, 0, renderer.width, renderer.height, hex);
	
	return renderer;
	
}

gl_renderer gl_present(gl_renderer renderer) {
	renderer.writeable_buffer = GFX_blit_image_to_ptr8(renderer.writeable_buffer, renderer.width, 0, 0, renderer.width, renderer.height, renderer.back_buffer);
	return renderer;
	
}

gl_renderer gl_put_pixel(gl_renderer renderer, uint32 x, uint32 y, gl_colour colour) {
	uint32 hex = GFX_compile_rgb_to_hex(colour.r, colour.g, colour.b);
	renderer.back_buffer = GFX_put_pixel_hex_to_ptr8(renderer.back_buffer, renderer.width, x, y, hex);
	
	return renderer;
	
}

gl_vector_2d gl_project(gl_renderer renderer, gl_vector_3d coord, gl_matrix transformation_matrix) {
	gl_vector_3d point = gl_transform_coord(coord, transformation_matrix);
	gl_float x = point.x * renderer.width + renderer.width / 2.0f;
	gl_float y = -point.y * renderer.height + renderer.height / 2.0f;
	
	return gl_new_vector_2d(x, y);
	
}

gl_renderer gl_draw_point(gl_renderer renderer, gl_vector_2d point) {
	if (point.x >= 0 && point.y >= 0 && point.x < renderer.width && point.y < renderer.height) {
		return put_pixel(renderer, (uint32) point.x, (uint32) point.y, gl_new_colour(1.0f, 1.0f, 1.0f));
		
	}
	
	return renderer;
	
}

void gl_render(gl_renderer renderer, gl_camera camera, gl_batch batch) {
	gl_matrix view_matrix = gl_look_at_LH(camera.position, camera.target, GL_UNIT_Y);
	gl_matrix projection_matrix = gl_perspective_FOV_RH(camera.fov, (gl_float) renderer.width / renderer.height, camera.clipping_near, camera.clipping_far);
	
	gl_matrix world_matrix;
	gl_matrix transform_matrix;
	gl_vector_2d point;
	
	int m;
	int v;
	for (m = 0; m < batch.mesh_count; m++) {
		world_matrix = gl_matrix_rotation_YPR(batch.meshes[m].rotation.y, batch.meshes[m].rotation.x, batch.meshes[m].rotation.z) * gl_matrix_translation(batch.meshes[m].position);
		transform_matrix = gl_matrix_multiply(world_matrix, view_matrix, projection_matrix, 16);
		
		for (v = 0; v < batch.meshes[m].length; v++) {
			point = gl_project(renderer, batch.meshes[m].vertices[v], transform_matrix);
			renderer = gl_draw_point(renderer, point);
			
		}
		
	}
	
}
