


#ifndef AQUA_3D_MAIN_H
	#define AQUA_3D_MAIN_H
	
	#include "../../types.h"
	#include "../GFX.h"
	#include "../../utils/math.h"
	
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
	
	typedef float gl_float;
	typedef gl_float* gl_matrix;
	
	typedef struct {
		uint8 r;
		uint8 g;
		uint8 b;
		
	} gl_colour;
	
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
	
	typedef struct {
		uint8* back_buffer;
		uint8* writeable_buffer;
		
		uint32 width;
		uint32 height;
		
	} gl_renderer;
	
	typedef struct {
		int64 name;
		gl_vector_3d* vertices;
		int vertex_count;
		
		gl_vector_3d position;
		gl_vector_3d rotation;
		
	} gl_mesh;
	
	typedef struct {
		gl_vector_3d position;
		gl_vector_3d target;
		
		gl_float fov; // 0.78f
		gl_float clipping_near; // 0.01f
		gl_float clipping_far; // 1.0f
		
	} gl_camera;
	
	typedef struct {
		gl_mesh* meshes;
		int mesh_count;
		
	} gl_batch;
	
	typedef struct gl_scene {
		gl_renderer renderer;
		gl_camera camera;
		gl_batch batch;
		gl_colour clear_colour;
		
	} gl_scene;
	
	gl_colour gl_new_colour(gl_float r, gl_float g, gl_float b);
	
	gl_vector_3d gl_transform_coord(gl_vector_3d coord, gl_matrix trans);
	
	gl_vector_3d gl_new_vector_3d(gl_float x, gl_float y, gl_float z);
	gl_vector_2d gl_new_vector_2d(gl_float x, gl_float y);
	
	gl_float gl_vector_dot(gl_vector_3d left, gl_vector_3d right);
	gl_float gl_vector_length(gl_vector_3d vector);
	
	gl_vector_3d gl_vector_subtract(gl_vector_3d left, gl_vector_3d right);
	gl_vector_3d gl_vector_cross(gl_vector_3d left, gl_vector_3d right);
	gl_vector_3d gl_vector_normalize(gl_vector_3d vector);
	
	gl_vector_4d gl_quaternion_rotation_YPR(gl_float yaw, gl_float pitch, gl_float roll);
	
	gl_matrix gl_look_at_LH(gl_vector_3d eye, gl_vector_3d target, gl_vector_3d up);
	gl_matrix gl_perspective_FOV_RH(gl_float fov, gl_float aspect, gl_float clipping_near, gl_float clipping_far);
	gl_matrix gl_quaternion_rotation(gl_vector_4d rotation);
	
	gl_matrix gl_matrix_rotation_YPR(gl_float yaw, gl_float pitch, gl_float roll);
	gl_matrix gl_matrix_translation(gl_vector_3d vector);
	
	gl_matrix gl_matrix_multiply_3(gl_matrix _1, gl_matrix _2, gl_matrix _3);
	gl_matrix gl_matrix_multiply_2(gl_matrix _1, gl_matrix _2);
	
	gl_mesh gl_new_mesh(int64 name, int vertex_count);
	
	gl_camera gl_new_camera(gl_vector_3d position, gl_vector_3d target, gl_float fov, gl_float clipping_near, gl_float clipping_far);
	
	gl_batch gl_new_batch(void);
	gl_batch gl_add_to_batch(gl_batch batch, gl_mesh mesh);
	
	gl_renderer gl_new_renderer(uint32 width, uint32 height);
	gl_renderer gl_clear(gl_renderer renderer, gl_colour colour);
	gl_renderer gl_present(gl_renderer renderer);
	
	gl_renderer gl_put_pixel(gl_renderer renderer, uint32 x, uint32 y, gl_colour colour);
	gl_vector_2d gl_project(gl_renderer renderer, gl_vector_3d coord, gl_matrix transformation_matrix);
	gl_renderer gl_draw_point(gl_renderer renderer, gl_vector_2d point);
	
	gl_renderer gl_render_batch(gl_renderer renderer, gl_camera camera, gl_batch batch);
	
	gl_scene gl_gross_setup(uint32 width, uint32 height);
	gl_scene gl_gross_render(gl_scene scene);
	int gl_find_mesh(gl_scene scene, int64 name);
	gl_scene gl_primitive_add_cube(gl_scene scene, int64 name, gl_float scale, gl_vector_3d position, gl_vector_3d rotation);
	void gl_set_loop(void* loop);
	
#endif
