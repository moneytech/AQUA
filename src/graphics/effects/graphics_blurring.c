/*
 * graphics_blurring.c
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


// Special thanks to http://blog.ivank.net/fastest-gaussian-blur.html

#include "graphics_blurring.h"

/*uint8* blur(uint8* surface, uint32 width, uint32 height, uint8 radius) {
	uint8* where = surface;
	
	uint8* source = (uint8*) kmalloc(width * height * 3);
	source = GFX_blit_image_to_ptr8(source, width, 0, 0, width, height, surface);
	
	int kernel_size = radius * 2;
	int kernel_average[3];
	int kernel_average_length;
	
	int kx;
	int ky;
	
	int x;
	int y;
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			kernel_average_length = 0;
			
			kernel_average[0] = 0;
			kernel_average[1] = 0;
			kernel_average[2] = 0;
			
			for (kx = -kernel_size; kx < kernel_size; kx++) {
				for (ky = -kernel_size; ky < kernel_size; ky++) {
					if (kx + x > 0 && kx + x < width && ky + y > 0 && ky + y < height) {
						kernel_average[0] += source[(width * 3) * (ky + y) + (kx + x) * 3];
						kernel_average[1] += source[((width * 3) * (ky + y) + (kx + x) * 3) + 1];
						kernel_average[2] += source[((width * 3) * (ky + y) + (kx + x) * 3) + 2];
						kernel_average_length++;
						
					}
					
				}
				
			}
			
			where[3 * x] = kernel_average[0] / kernel_average_length;
			where[3 * x + 1] = kernel_average[1] / kernel_average_length;
			where[3 * x + 2] = kernel_average[2] / kernel_average_length;
			
		}
		
		where += width * 3;
		
	}
	
	kfree(source, width * height * 3);
	return surface;
	
}*/

static int* boxes_for_gaussian(float sigma, int n) {
	float w_ideal = sqrt((12 * sigma * sigma / n) + 1);
	int wl = floor(w_ideal);
	
	if (wl % 2 == 0) wl--;
	
	int wu = wl + 2;
	float m_ideal = (12 * sigma * sigma - n * wl * wl - 4 * n * wl - 3 * n) / (-4 * wl - 4);
	int m = round(m_ideal);
	
	int sizes[n];
	int i;
	for (i = 0; i < n; i++) {
		sizes[i] = i < m ? wl : wu;
		
	}
	
	return (int*) sizes;
	
}

static uint8* box_blur_H_4(uint8* scl, uint32 w, uint32 h, uint8 r) {
	uint8* tcl = (uint8*) kmalloc(w * h);
	memcpy(tcl, scl, w * h);
	
	float iarr = 1.0 / (r + r + 1);
	
	int i;
	int j;
	for (i = 0; i < h; i++) {
		int ti = i * w;
		int li = ti;
		int ri = ti + r;
		
		int fv = scl[ti];
		int lv = scl[ti + w - 1];
		int val = (r + 1) * fv;
		
		for (j = 0; j < r; j++) { val += scl[ti + j]; } 
		for (j = 0; j <= r; j++) { val += scl[ri++] - fv; tcl[ti++] = round(val * iarr); } 
		for (j = r + 1; j < w - r; j++) { val += scl[ri++] - scl[li++]; tcl[ti++] = round(val * iarr); } 
		for (j = w - r; j < w; j++) { val += lv - scl[li++]; tcl[ti++] = round(val * iarr); } 
		
	}
	
	memcpy(scl, tcl, w * h);
	kfree(tcl, w * h);
	
	return scl;
	
}

static uint8* box_blur_T_4(uint8* scl, uint32 w, uint32 h, uint8 r) {
	uint8* tcl = (uint8*) kmalloc(w * h);
	memcpy(tcl, scl, w * h);
	
	float iarr = 1.0 / (r + r + 1);
	
	int i;
	int j;
	for (i = 0; i < w; i++) {
		int ti = i;
		int li = ti;
		int ri = ti + r * w;
		
		int fv = scl[ti];
		int lv = scl[ti + w * (h - 1)];
		int val = (r + 1) * fv;
		
		for (j = 0; j < r; j++) { val += scl[ti + j * w]; } 
		for (j = 0; j <= r; j++) { val += scl[ri] - fv; tcl[ti] = round(val * iarr); ri += w; ti += w; } 
		for (j = r + 1; j < h - r; j++) { val += scl[ri] - scl[li]; tcl[ti] = round(val * iarr); li += w; ri += w; ti += w; } 
		for (j = h - r; j < h; j++) { val += lv - scl[li]; tcl[ti] = round(val * iarr); li += w; ti += w; } 
		
	}
	
	memcpy(scl, tcl, w * h);
	kfree(tcl, w * h);
	
	return scl;
	
}

static uint8* box_blur_4(uint8* surface, uint32 width, uint32 height, uint8 radius) {
	surface = box_blur_H_4(surface, width, height, radius);
	surface = box_blur_T_4(surface, width, height, radius);
	
	return surface;
	
}

uint8* gaussian_blur_4(uint8* surface, uint32 width, uint32 height, uint8 radius) {
	int* boxes = boxes_for_gaussian(radius, 3);
	surface = box_blur_4(surface, width, height, (boxes[0] - 1) / 2);
	surface = box_blur_4(surface, width, height, (boxes[1] - 1) / 2);
	surface = box_blur_4(surface, width, height, (boxes[2] - 1) / 2);
	
	return surface;
	
}

uint8* blur(uint8* surface, uint32 width, uint32 height, uint8 radius) {
	uint8* red = (uint8*) kmalloc(width * height);
	uint8* green = (uint8*) kmalloc(width * height);
	uint8* blue = (uint8*) kmalloc(width * height);
	
	int p;
	for (p = 0; p < width * height * 3; p += 3) {
		red[p / 3] = surface[p];
		green[p / 3] = surface[p + 1];
		blue[p / 3] = surface[p + 2];
		
	}
	
	red = gaussian_blur_4(red, width, height, radius);
	green = gaussian_blur_4(green, width, height, radius);
	blue = gaussian_blur_4(blue, width, height, radius);
	
	for (p = 0; p < width * height * 3; p += 3) {
		surface[p] = red[p / 3];
		surface[p + 1] = green[p / 3];
		surface[p + 2] = green[p / 3];
		
	}
	
	kfree(red, width * height);
	kfree(green, width * height);
	kfree(blue, width * height);
	
	return surface;
	
}

uint8* perfect_gaussian(uint8* surface, uint32 width, uint32 height, uint8 radius) {
	uint8* where = surface;
	
	uint8* source = (uint8*) kmalloc(width * height * 3);
	source = GFX_blit_image_to_ptr8(source, width, 0, 0, width, height, surface);
	
	int rs = ceil(radius * 2.57);
	
	float val;
	float wsum;
	
	int iy;
	int ix;
	
	int x;
	int y;
	int dsq;
	float weight;
	
	int i;
	int j;
	int c;
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			for (c = 0; c < 3; c++) {
				val = 0;
				wsum = 0;
				
				for (iy = i - rs; iy < i + rs + 1; iy++) {
					for (ix = j - rs; ix < j + rs + 1; ix++) {
						x = min(width - 1, max(0, ix));
						y = min(height - 1, max(0, iy));
						
						dsq = (ix - j) * (ix - j) + (iy - i) * (iy - i);
						weight = exp(-dsq / (2 * radius * radius)) / (PI * 2 * radius * radius);
						
						val += source[((width * 3) * y + x * 3) + c] * weight;
						wsum += weight;
						
					}
					
				}
				
				where[3 * j + c] = round(val / wsum);
				
			}
			
		}
		
		where += width * 3;
		
	}
	
	return surface;
	
}

uint8* blurring_wrapper(char type, uint8* surface, uint32 width, uint32 height, uint8 radius) {
	uint8* source = (uint8*) kmalloc(width * height * 3);
	source = GFX_blit_image_to_ptr8(source, width, 0, 0, width, height, surface);
	
	uint8* red_channel = (uint8*) kmalloc(width * height);
	uint8* green_channel = (uint8*) kmalloc(width * height);
	uint8* blue_channel = (uint8*) kmalloc(width * height);
	
	uint8* red_channel_cpy = (uint8*) kmalloc(width * height);
	uint8* green_channel_cpy = (uint8*) kmalloc(width * height);
	uint8* blue_channel_cpy = (uint8*) kmalloc(width * height);
	
	int i;
	for (i = 0; i < width * height * 3; i += 3) {
		red_channel[i / 3] = source[i];
		green_channel[i / 3] = source[i + 1];
		blue_channel[i / 3] = source[i + 2];
		
	}
	
	if (type == 'n') red_channel = gauss_blur(red_channel_cpy, red_channel, width, height, radius);
	kfree(red_channel_cpy, width * height);
	
	if (type == 'n') green_channel = gauss_blur(green_channel_cpy, green_channel, width, height, radius);
	kfree(green_channel_cpy, width * height);
	
	if (type == 'n') blue_channel = gauss_blur(blue_channel_cpy, blue_channel, width, height, radius);
	kfree(blue_channel_cpy, width * height);
	
	for (i = 0; i < width * height * 3; i += 3) {
		surface[i] = red_channel[i / 3];
		surface[i + 1] = green_channel[i / 3];
		surface[i + 2] = blue_channel[i / 3];
		
	}
	
	kfree(red_channel, width * height);
	kfree(green_channel, width * height);
	kfree(blue_channel, width * height);
	
	return surface;
	
}

int* boxes_for_gauss(float sigma, float n) {
	float ideal_width = sqrt((12.0f * sigma * sigma / n) + 1.0f);
	
	int wl = floor(ideal_width);
	if (wl & 1 == 0) wl--;
	float wu = wl + 2.0f;
	
	float ideal_m = (12.0f * sigma * sigma - n * wl * wl - 4.0f * n * wl - 3.0f * n) / (-4.0f * wl - 4.0f);
	float m = round(ideal_m);
	
	int* sizes = (int*) kmalloc(n);
	
	int i;
	for (i = 0; i < n; i++) {
		sizes[i] = i < m ? wl : wu;
		
	}
	
	return sizes;
	
}

uint8* gauss_blur(uint8* source, uint8* target, uint32 width, uint32 height, uint8 radius) {
	int* boxes = boxes_for_gauss(radius, 3);
	box_blur(source, target, width, height, (boxes[0] - 1) / 2);
	box_blur(source, target, width, height, (boxes[1] - 1) / 2);
	box_blur(source, target, width, height, (boxes[2] - 1) / 2);
	
	return target;
	
}

uint8* box_blur(uint8* source, uint8* target, uint32 width, uint32 height, uint8 radius) {
	int i;
	for (i = 0; i < width * height; i++) {
		target[i] = source[i];
		
	}
	
	box_blur_H(target, source, width, height, radius);
	box_blur_T(source, target, width, height, radius);
	
	return target;
	
}

uint8* box_blur_H(uint8* source, uint8* target, uint32 width, uint32 height, uint8 radius) {
	int iarr = 1 / (radius + radius + 1);
	
	int ti;
	int li;
	int ri;
	
	int fv;
	int lv;
	int val;
	
	int i;
	int j;
	for (i = 0; i < height; i++) {
		ti = i * width;
		li = ti;
		ri = ti + radius;
		
		fv = source[ti];
		lv = source[ti + width - 1];
		val = (radius + 1) * fv;
		
		for (j = 0; j < radius; j++) val += source[ti + j];
		for (j = 0; j <= radius; j++) {
			val += source[ri++] - fv;
			target[ti++] = round(val * iarr);
			
		} for (j = radius + 1; j < width - radius; j++) {
			val += source[ri++] - source[li++];
			target[ti++] = round(val * iarr);
			
		} for (j = width - radius; j < width; j++) {
			val += lv - source[li++];
			target[ti++] = round(val * iarr);
			
		}
		
	}
	
	return target;
	
}

uint8* box_blur_T(uint8* source, uint8* target, uint32 width, uint32 height, uint8 radius) {
	int iarr = 1 / (radius + radius + 1);
	
	int ti;
	int li;
	int ri;
	
	int fv;
	int lv;
	int val;
	
	int i;
	int j;
	for (i = 0; i < height; i++) {
		ti = i;
		li = ti;
		ri = ti + radius * width;
		
		fv = source[ti];
		lv = source[ti + width - 1];
		val = (radius + 1) * fv;
		
		for (j = 0; j < radius; j++) val += source[ti + j * width];
		for (j = 0; j <= radius; j++) {
			val += source[ri] - fv;
			target[ti] = round(val * iarr);
			
			ri += width;
			ti += width;
			
		} for (j = radius + 1; j < height; j++) {
			val += source[ri] - source[li];
			target[ti] = round(val * iarr);
			
			li += width;
			ri += width;
			ti += width;
			
		} for (j = height - radius; j < height - radius; j++) {
			val += lv - source[li];
			target[ti] = round(val * iarr);
			
			li += width;
			ti += width;
			
		}
		
	}
	
	return target;
	
}
