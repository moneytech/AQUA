/*
 * left_game.h
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


#ifndef LEFT_GAME_H
	#define LEFT_GAME_H
	
	#include "types.h"
	#include "drivers/PIT_driver.h"
	#include "graphics/GFX.h"
	
	static uint32 game_width;
	static uint32 game_height;
	static uint16 game_fps;
	
	static uint8* game_double_buffer;
	static uint8* game_final_buffer;
	
	static boolean game_running;
	
	static int game_pid;
	
	static uint8 game_key_press;
	static uint8 game_key_release;
	
	static boolean game_holding_left;
	static boolean game_holding_right;
	static boolean game_holding_jump;
	static boolean game_holding_sprint;
	
	static int player_x;
	static int player_y;
	
	static int dy;
	static int speed;
	
	void game_physics_loop(void) {
		game_key_press = poll_key();
		game_key_release = poll_key_release();
		
		switch (game_key_press) {
			case 75:
			case 30: {
				game_holding_left = TRUE;
				break;
				
			} case 77:
			case 32: {
				game_holding_right = TRUE;
				break;
				
			} case 72:
			case 57:
			case 17: {
				game_holding_jump = TRUE;
				break;
				
			} default: {
				break;
				
			}
			
		} switch (game_key_release) {
			case 75:
			case 30: {
				game_holding_left = FALSE;
				break;
				
			} case 77:
			case 32: {
				game_holding_right = FALSE;
				break;
				
			} case 72:
			case 57:
			case 17: {
				game_holding_jump = FALSE;
				break;
				
			} default: {
				break;
				
			}
			
		}
		
		if (game_holding_left) {
			player_x -= speed;
			
		} if (game_holding_right) {
			player_x += speed;
			
		} if (game_holding_jump && dy == 0) {
			dy = 20;
			
		}
		
		dy -= 2;
		player_y -= dy;
		
		if (player_y > 500) {
			player_y = 500;
			dy = 0;
			
		}
		
	}
	
	void left_game(void) {
		game_holding_left = FALSE;
		game_holding_right = FALSE;
		game_holding_jump = FALSE;
		game_holding_sprint = FALSE;
		
		player_x = 100;
		player_y = 100;
		
		dy = 0;
		speed = 5;
		
		game_width = GFX_get_mode_info('w');
		game_height = GFX_get_mode_info('h');
		
		game_double_buffer = (uint8*) kmalloc(game_width * game_height * 3);
		game_final_buffer = (uint8*) kmalloc(game_width * game_height * 3);
		
		game_fps = 30;
		timer_phase(game_fps);
		game_pid = pit_add_process(game_physics_loop);
		
		int anim = 0;
		int anim_dir = 0;
		
		game_running = TRUE;
		while (game_running) {
			game_double_buffer = GFX_fill_rect_hex_to_ptr8(game_double_buffer, game_width, 0, 0, game_width, game_height, 0x00001122);
			game_double_buffer = GFX_fill_rect_hex_to_ptr8(game_double_buffer, game_width, 50, 50, 50, 50, 0x00FFEECC);
			
			player_y -= 100;
			
			game_double_buffer = GFX_fill_rect_hex_to_ptr8(game_double_buffer, game_width, player_x - 25, player_y + 30, 100, 80, 0x00000000);
			game_double_buffer = GFX_fill_rect_hex_to_ptr8(game_double_buffer, game_width, player_x + 15, player_y + 20, 20, 30, 0x00000000);
			
			game_double_buffer = GFX_fill_rect_hex_to_ptr8(game_double_buffer, game_width, player_x, player_y + 110, 20, 30, 0x00000000);
			game_double_buffer = GFX_fill_rect_hex_to_ptr8(game_double_buffer, game_width, player_x + 30, player_y + 110, 20, 30, 0x00000000);
			
			game_double_buffer = GFX_fill_rect_hex_to_ptr8(game_double_buffer, game_width, player_x, player_y, 50, 20, 0x00000000);
			game_double_buffer = GFX_fill_rect_hex_to_ptr8(game_double_buffer, game_width, player_x + 10, player_y - 30 + anim, 30, 30, 0x00000000);
			
			game_double_buffer = GFX_fill_rect_hex_to_ptr8(game_double_buffer, game_width, player_x + 15, player_y - 20 + anim, 2, 2, 0x00FFFFFF);
			game_double_buffer = GFX_fill_rect_hex_to_ptr8(game_double_buffer, game_width, player_x + 30, player_y - 20 + anim, 2, 2, 0x00FFFFFF);
			
			player_y += 100;
			
			if (anim_dir) anim++;
			else anim--;
			
			if (anim < -10) anim_dir = 1;
			if (anim > 10) anim_dir = 0;
			
			GFX_blit_image(0, 0, game_width, game_height, game_double_buffer);
			
		}
		
		kfree(game_double_buffer, game_width * game_height * 3);
		kfree(game_final_buffer, game_width * game_height * 3);
		
		pit_remove_process(game_pid);
		game_running = FALSE;
		
	}
	
#endif
