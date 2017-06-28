/*
 * obsidian.h
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


#ifndef OBSIDIAN_H
	#define OBSIDIAN_H
	
	#include "../types.h"
	#include "aqua3d.h"
	
	typedef struct {
		uint16 command;
		
		gl_pos_t* position;
		int position_count;
		
		gl_colour_t* colour;
		int colour_count;
		
	} obsidian_command;
	
	typedef struct {
		gl_camera_t camera;
		obsidian_command* commands;
		int command_count;
		
	} obsidian_frame;
	
	typedef struct {
		uint16 fps;
		gl_colour_t clear_colour;
		
		boolean render_points;
		boolean render_edges;
		boolean render_faces;
		
		boolean render_fog;
		boolean render_bloom;
		boolean render_billboards;
		
		boolean depth_test;
		
		gl_float_t fog_start;
		gl_float_t fog_end;
		uint32 fog_colour;
		
		uint32 bloom_radius;
		
		uint32 billboard_size;
		uint8 billboard_alpha;
		
	} obsidian_scene;
	
	typedef struct {
		uint32 width;
		uint32 height;
		uint16 fps;
		
		uint32 frames;
		uint8** video;
		
	} obsidian_final_render;
	
#endif
