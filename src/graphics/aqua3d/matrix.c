/*
 * matrix.c
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


#include "matrix.h"

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

gl_matrix gl_matrix_multiply(gl_matrix _1, gl_matrix _2, gl_matrix _3, int length) {
	gl_matrix result;
	
	int c;
	for (c = 0; c < length; c++) {
		result[c] = _1[c] * _2[c] * _3[c];
		
	}
	
	return result;
	
}
