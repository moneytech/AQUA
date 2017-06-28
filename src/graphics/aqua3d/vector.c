/*
 * vector.c
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


#include "vector.h"

const gl_float GL_ZERO_TOLERANCE = 1e-6f;

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

gl_vector_3d GL_UNIT_X = {1.0f, 0.0f, 0.0f};
gl_vector_3d GL_UNIT_Y = {0.0f, 1.0f, 0.0f};
gl_vector_3d GL_UNIT_Z = {0.0f, 0.0f, 1.0f};
