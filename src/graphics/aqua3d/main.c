
#include "main.h"

static gl_float GL_ZERO_TOLERANCE = 1e-6f;
static gl_vector_3d GL_UNIT_X = {1.0f, 0.0f, 0.0f};
static gl_vector_3d GL_UNIT_Y = {0.0f, 1.0f, 0.0f};
static gl_vector_3d GL_UNIT_Z = {0.0f, 0.0f, 1.0f};

gl_colour gl_new_colour(gl_float r, gl_float g, gl_float b) {
	gl_colour colour;
	colour.r = (uint8) (r * 255);
	colour.g = (uint8) (g * 255);
	colour.b = (uint8) (b * 255);
	
	return colour;
	
}

gl_vector_3d gl_transform_coord(gl_vector_3d coord, gl_matrix trans) {
	gl_vector_4d vector;
	vector.x = (coord.x * trans[GL_M11]) + (coord.y * trans[GL_M21]) + (coord.z * trans[GL_M31]) + trans[GL_M41];
	vector.y = (coord.x * trans[GL_M12]) + (coord.y * trans[GL_M22]) + (coord.z * trans[GL_M32]) + trans[GL_M42];
	vector.z = (coord.x * trans[GL_M13]) + (coord.y * trans[GL_M23]) + (coord.z * trans[GL_M33]) + trans[GL_M43];
	vector.w = 1.0f / ((coord.x * trans[GL_M14]) + (coord.y * trans[GL_M24]) + (coord.z * trans[GL_M34]) + trans[GL_M44]);
	
	return gl_new_vector_3d(vector.x * vector.w, vector.y * vector.w, vector.z * vector.w);
	
}

gl_vector_3d gl_new_vector_3d(gl_float x, gl_float y, gl_float z) {
	gl_vector_3d vector;
	vector.x = x;
	vector.y = y;
	vector.z = z;
	
	return vector;
	
}

gl_vector_2d gl_new_vector_2d(gl_float x, gl_float y) {
	gl_vector_2d vector;
	vector.x = x;
	vector.y = y;
	
	return vector;
	
}

gl_float gl_vector_dot(gl_vector_3d left, gl_vector_3d right) {
	return (left.x * right.x) + (left.y * right.y) + (left.z * right.z);
	
}

gl_float gl_vector_length(gl_vector_3d vector) {
	return (gl_float) sqrt((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));
	
}

gl_vector_3d gl_vector_subtract(gl_vector_3d left, gl_vector_3d right) {
	return gl_new_vector_3d(left.x - right.x, left.y - right.y, left.z - right.z);
	
}

gl_vector_3d gl_vector_cross(gl_vector_3d left, gl_vector_3d right) {
	return gl_new_vector_3d((left.y * right.z) - (left.z * right.y), \
		(left.z * right.x) - (left.x * right.z), \
		(left.x * right.y) - (left.y * right.x));
	
}

gl_vector_3d gl_vector_normalize(gl_vector_3d vector) {
	gl_float length = gl_vector_length(vector);
	gl_float inv;
	
	if (!(fabs(length) < GL_ZERO_TOLERANCE)) {
		inv = 1.0f / length;
		vector.x *= inv;
		vector.y *= inv;
		vector.z *= inv;
		
	}
	
	return vector;
	
}

gl_vector_4d gl_quaternion_rotation_YPR(gl_float yaw, gl_float pitch, gl_float roll) {
	gl_float half_roll = roll * 0.5f;
	gl_float half_pitch = pitch * 0.5f;
	gl_float half_yaw = yaw * 0.5f;
	
	gl_float sin_roll = (gl_float) sin(half_roll);
	gl_float cos_roll = (gl_float) cos(half_roll);
	
	gl_float sin_pitch = (gl_float) sin(half_pitch);
	gl_float cos_pitch = (gl_float) cos(half_pitch);
	
	gl_float sin_yaw = (gl_float) sin(half_yaw);
	gl_float cos_yaw = (gl_float) cos(half_yaw);
	
	gl_vector_4d quaternion;
	
	quaternion.x = (cos_yaw * sin_pitch * cos_roll) + (sin_yaw * cos_pitch * sin_roll);
	quaternion.y = (sin_yaw * cos_pitch * cos_roll) - (cos_yaw * sin_pitch * sin_roll);
	quaternion.z = (cos_yaw * cos_pitch * sin_roll) - (sin_yaw * sin_pitch * cos_roll);
	quaternion.w = (cos_yaw * cos_pitch * cos_roll) + (sin_yaw * sin_pitch * sin_roll);
	
	return quaternion;
	
}

gl_matrix gl_look_at_LH(gl_vector_3d eye, gl_vector_3d target, gl_vector_3d up) {
	gl_vector_3d x_axis;
	gl_vector_3d y_axis;
	gl_vector_3d z_axis;
	
	z_axis = gl_vector_subtract(target, eye);
	z_axis = gl_vector_normalize(z_axis);
	
	x_axis = gl_vector_cross(up, z_axis);
	x_axis = gl_vector_normalize(x_axis);
	
	y_axis = gl_vector_cross(z_axis, x_axis);
	gl_matrix result;
	
	result[GL_M11] = x_axis.x; result[GL_M21] = x_axis.y; result[GL_M31] = x_axis.z;
	result[GL_M12] = y_axis.x; result[GL_M22] = y_axis.y; result[GL_M32] = y_axis.z;
	result[GL_M13] = z_axis.x; result[GL_M23] = z_axis.y; result[GL_M33] = z_axis.z;
	
	result[GL_M41] = -gl_vector_dot(x_axis, eye);
	result[GL_M42] = -gl_vector_dot(y_axis, eye);
	result[GL_M43] = -gl_vector_dot(z_axis, eye);
	
	return result;
	
}

gl_matrix gl_perspective_FOV_RH(gl_float fov, gl_float aspect, gl_float clipping_near, gl_float clipping_far) {
	gl_float y_scale = (gl_float) (1.0f / tan(fov * 0.5f));
	gl_float q = clipping_far / (clipping_near - clipping_far);
	gl_matrix matrix;
	
	matrix[GL_M11] = y_scale / aspect;
	matrix[GL_M22] = y_scale;
	matrix[GL_M33] = q;
	
	matrix[GL_M34] = -1.0f;
	matrix[GL_M43] = q * clipping_near;
	
	return matrix;
	
}

gl_matrix gl_quaternion_rotation(gl_vector_4d rotation) {
	gl_float xx = rotation.x * rotation.x;
	gl_float yy = rotation.y * rotation.y;
	gl_float zz = rotation.z * rotation.z;
	gl_float xy = rotation.x * rotation.y;
	gl_float zw = rotation.z * rotation.w;
	gl_float zx = rotation.z * rotation.x;
	gl_float yw = rotation.y * rotation.w;
	gl_float yz = rotation.y * rotation.z;
	gl_float xw = rotation.x * rotation.w;
	
	gl_matrix matrix;
	
	matrix[GL_M11] = 1.0f - (2.0f * (yy + zz));
	matrix[GL_M12] = 2.0f * (xy + zw);
	matrix[GL_M13] = 2.0f * (zx - yw);
	matrix[GL_M21] = 2.0f * (xy - zw);
	matrix[GL_M22] = 1.0f - (2.0f * (zz + xx));
	matrix[GL_M23] = 2.0f * (yz + xw);
	matrix[GL_M31] = 2.0f * (zx + yw);
	matrix[GL_M32] = 2.0f * (yz - xw);
	matrix[GL_M33] = 1.0f - (2.0f * (yy + xx));
	
	return matrix;
	
}

gl_matrix gl_matrix_rotation_YPR(gl_float yaw, gl_float pitch, gl_float roll) {
	gl_matrix result;
	gl_vector_4d quaternion;
	
	quaternion = gl_quaternion_rotation_YPR(yaw, pitch, roll);
	return gl_quaternion_rotation(quaternion);
	
}

gl_matrix gl_matrix_translation(gl_vector_3d vector) {
	gl_matrix matrix;
	matrix[GL_M41] = vector.x;
	matrix[GL_M42] = vector.y;
	matrix[GL_M43] = vector.z;
	
	return matrix;
	
}

gl_matrix gl_matrix_multiply_3(gl_matrix _1, gl_matrix _2, gl_matrix _3) {
	gl_matrix result;
	
	int c;
	for (c = 0; c < GL_M44 + 1; c++) {
		result[c] = _1[c] * _2[c] * _3[c];
		
	}
	
	return result;
	
}

gl_matrix gl_matrix_multiply_2(gl_matrix _1, gl_matrix _2) {
	gl_matrix result;
	
	int c;
	for (c = 0; c < GL_M44 + 1; c++) {
		result[c] = _1[c] * _2[c];
		
	}
	
	return result;
	
}

gl_mesh gl_new_mesh(int64 name, int vertex_count) {
	gl_mesh mesh;
	mesh.vertex_count = vertex_count;
	mesh.name = name;
	
	return mesh;
	
}

gl_camera gl_new_camera(gl_vector_3d position, gl_vector_3d target, gl_float fov, gl_float clipping_near, gl_float clipping_far) {
	gl_camera camera;
	
	camera.position = position;
	camera.target = target;
	
	camera.fov = fov;
	camera.clipping_near = clipping_near;
	camera.clipping_far = clipping_far;
	
	return camera;
	
}

gl_batch gl_new_batch(void) {
	gl_batch batch;
	batch.mesh_count = 0;
	
	return batch;
	
}

gl_batch gl_add_to_batch(gl_batch batch, gl_mesh mesh) {
	batch.meshes[batch.mesh_count] = mesh;
	batch.mesh_count++;
	
	return batch;
	
}

gl_renderer gl_new_renderer(uint32 width, uint32 height) {
	gl_renderer renderer;
	renderer.width = width;
	renderer.height = height;
	
	renderer.writeable_buffer = (uint8*) kmalloc(width * height * 3);
	renderer.writeable_buffer = (uint8*) kmalloc(width * height * 3);
	
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
		renderer = gl_put_pixel(renderer, (uint32) point.x, (uint32) point.y, gl_new_colour(1.0f, 1.0f, 1.0f));
		
	}
	
	return renderer;
	
}

gl_renderer gl_render_batch(gl_renderer renderer, gl_camera camera, gl_batch batch) {
	gl_matrix view_matrix = gl_look_at_LH(camera.position, camera.target, GL_UNIT_Y);
	gl_matrix projection_matrix = gl_perspective_FOV_RH(camera.fov, (gl_float) renderer.width / renderer.height, camera.clipping_near, camera.clipping_far);
	
	gl_matrix world_matrix;
	gl_matrix transform_matrix;
	gl_vector_2d point;
	
	int m;
	int v;
	for (m = 0; m < batch.mesh_count; m++) {
		world_matrix = gl_matrix_multiply_2(gl_matrix_rotation_YPR(batch.meshes[m].rotation.y, batch.meshes[m].rotation.x, batch.meshes[m].rotation.z), gl_matrix_translation(batch.meshes[m].position));
		transform_matrix = gl_matrix_multiply_3(world_matrix, view_matrix, projection_matrix);
		
		for (v = 0; v < batch.meshes[m].vertex_count; v++) {
			point = gl_project(renderer, batch.meshes[m].vertices[v], transform_matrix);
			renderer = gl_draw_point(renderer, point);
			
		}
		
	}
	
	return renderer;
	
}

gl_scene gl_gross_setup(uint32 width, uint32 height) {
	gl_scene scene;
	scene.clear_colour = gl_new_colour(0.0f, 0.0f, 0.0f);
	
	scene.renderer = gl_new_renderer(width, height);
	scene.camera = gl_new_camera(gl_new_vector_3d(0, 0, 10), gl_new_vector_3d(0, 0, 0), 0.78f, 0.01f, 1.0f);
	scene.batch = gl_new_batch();
	
	return scene;
	
}

gl_scene gl_gross_render(gl_scene scene) {
	scene.renderer = gl_clear(scene.renderer, scene.clear_colour);
	//scene.renderer = gl_render_batch(scene.renderer, scene.camera, scene.batch);
	
	scene.renderer = gl_present(scene.renderer);
	return scene;
	
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
