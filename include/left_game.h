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
	
	#define GAME_MODEL_DEBUG 0
	#define GAME_MODEL_PLAYER 1
	#define GAME_MODEL_GRAVE 2
	
	typedef struct {
		uint8 attributes; // order of the bits: affected by gravity? | ...
		
		float x;
		float y;
		
		float dy;
		float speed;
		
		uint16 model;
		int health;
		
	} game_entity;
	
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
	
	static float flt_player_x;
	static float flt_player_y;
	
	static int player_x;
	static int player_y;
	
	static uint32 player_colour;
	static uint32 eye_colour;
	static uint32 bg_colour;
	
	static int camera_x;
	static int camera_y;
	
	static int strike_x;
	static int strike_y;
	
	static int dy;
	static float speed;
	
	static int strike_speed = 6;
	static boolean game_striking;
	
	static int game_r;
	static int game_g;
	static int game_b;
	
	static int game_gravity = 2;
	static int game_jump_speed = 4;
	
	static float game_anim = 0;
	static int game_anim_dir = 0;
	
	static game_entity* game_entities;
	static int game_entity_count;
	
	int game_spawn_entity(float x, float y, float speed, uint16 model, uint8 attributes) {
		game_entity entity = {
			.attributes = attributes,
			
			.x = x,
			.y = y,
			
			.dy = 0,
			.speed = speed,
			
			.model = model,
			.health = 20
			
		};
		
		game_entities[game_entity_count] = entity;
		game_entity_count++;
		
		return game_entity_count - 1;
		
	}
	
	void game_blit_model(uint16 id, int x, int y) {
		switch (id) {
			case GAME_MODEL_PLAYER: {
				game_double_buffer = GFX_fill_rect_hex_to_ptr8(game_double_buffer, game_width, x - 25, y + 30, 100, 80, player_colour);
				game_double_buffer = GFX_fill_rect_hex_to_ptr8(game_double_buffer, game_width, x - 50, y + 40, 150, 20, player_colour);
				game_double_buffer = GFX_fill_rect_hex_to_ptr8(game_double_buffer, game_width, x + 15, y + 20, 20, 30, player_colour);
				
				game_double_buffer = GFX_fill_rect_hex_to_ptr8(game_double_buffer, game_width, x, y + 110, 20, 30, player_colour);
				game_double_buffer = GFX_fill_rect_hex_to_ptr8(game_double_buffer, game_width, x + 30, y + 110, 20, 30, player_colour);
				
				game_double_buffer = GFX_fill_rect_hex_to_ptr8(game_double_buffer, game_width, x, y, 50, 20, player_colour);
				game_double_buffer = GFX_fill_rect_hex_to_ptr8(game_double_buffer, game_width, x + 10, y - 30 + (int) game_anim, 30, 30, player_colour);
				
				game_double_buffer = GFX_fill_rect_hex_to_ptr8(game_double_buffer, game_width, x + 15, y - 20 + (int) game_anim, 2, 2, eye_colour);
				game_double_buffer = GFX_fill_rect_hex_to_ptr8(game_double_buffer, game_width, x + 30, y - 20 + (int) game_anim, 2, 2, eye_colour);
				
				break;
				
			} case GAME_MODEL_GRAVE: {
				game_double_buffer = GFX_fill_rect_hex_to_ptr8(game_double_buffer, game_width, x - 25, y + 30, 100, 80, 0x00292929);
				game_double_buffer = GFX_fill_rect_hex_to_ptr8(game_double_buffer, game_width, x - 15, y + 10, 80, 100, 0x00292929);
				
				break;
				
			} case GAME_MODEL_DEBUG:
			default: {
				break;
				
			}
			
		}
		
	}
	
	void game_physics_loop(void) {
		int e;
		for (e = 0; e < game_entity_count; e++) {
			game_entities[e].dy -= game_gravity;
			game_entities[e].y -= game_entities[e].dy;
			
			if (game_entities[e].y < 100) {
				game_entities[e].y = 100;
				game_entities[e].dy = 0;
				
			}
			
		}
		
		game_key_press = poll_key();
		game_key_release = poll_key_release();
		
		if (game_anim_dir) game_anim += speed;
		else game_anim -= speed;
		
		if (game_anim < -10) game_anim_dir = 1;
		if (game_anim > 10) game_anim_dir = 0;
		
		switch (game_key_press) {
			case 75:
			case 30: {
				game_holding_left = TRUE;
				game_striking = TRUE;
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
			flt_player_x -= speed;
			
		} if (game_holding_right) {
			flt_player_x += speed;
			
		} if (game_holding_jump && dy == 0) {
			dy = (int) (speed * game_jump_speed);
			
		}
		
		dy -= game_gravity;
		flt_player_y -= dy;
		
		if (flt_player_y > game_height) {
			flt_player_y = game_height;
			dy = 0;
			
		}
		
		player_x = (int) flt_player_x;
		player_y = (int) flt_player_y;
		
		if (game_striking) {
			if (strike_x < player_x) strike_x += strike_speed;
			if (strike_x > player_x) strike_x -= strike_speed;
			if (strike_y < player_y) strike_y += strike_speed;
			if (strike_y > player_y) strike_y -= strike_speed;
			
			if (strike_x < player_x + strike_speed && strike_y < player_y + strike_speed && strike_x > player_x - strike_speed && strike_y > player_y - strike_speed) {
				speed -= 0.02;
				
				if (speed < 0 && game_striking) {
					game_striking = FALSE;
					speed = 0;
					game_spawn_entity(player_x, -100, 50, GAME_MODEL_GRAVE, 0);
					
				}
				
				game_b = player_colour & 255;
				game_g = (player_colour >> 8) & 255;
				game_r = (player_colour >> 16) & 255;
				
				game_b += 3;
				game_g += 2;
				game_r += 1;
				
				if (game_b > 255) game_b = 255;
				if (game_g > 255) game_g = 255;
				if (game_r > 200) game_r = 200;
				
				player_colour = GFX_compile_rgb_to_hex((uint8) game_r, (uint8) game_g, (uint8) game_b);
				
				game_b = eye_colour & 255;
				game_g = (eye_colour >> 8) & 255;
				game_r = (eye_colour >> 16) & 255;
				
				game_b -= 2;
				game_g -= 1;
				
				if (game_b < 0) game_b = 0;
				if (game_g < 100) game_g = 100;
				
				eye_colour = GFX_compile_rgb_to_hex((uint8) game_r, (uint8) game_g, (uint8) game_b);
				
				game_b = bg_colour & 255;
				game_g = (bg_colour >> 8) & 255;
				game_r = (bg_colour >> 16) & 255;
				
				game_b += 2;
				game_g += 3;
				game_r += 1;
				
				if (game_b > 255) game_b = 255;
				if (game_g > 255) game_g = 255;
				if (game_r > 255) game_r = 255;
				
				bg_colour = GFX_compile_rgb_to_hex((uint8) game_r, (uint8) game_g, (uint8) game_b);
				
			}
			
		}
		
	}
	
	void game_strike(void) {
		game_double_buffer = GFX_draw_line_to_ptr8(game_double_buffer, game_width, 50, 50, strike_x + 25, strike_y - 80, rand_int(0x00000000, 0x00000022));
		game_double_buffer = GFX_draw_line_to_ptr8(game_double_buffer, game_width, 100, 100, strike_x + 25, strike_y - 80, rand_int(0x00000000, 0x00000022));
		game_double_buffer = GFX_draw_line_to_ptr8(game_double_buffer, game_width, 75, 75, strike_x + 25, strike_y - 80, rand_int(0x00000000, 0x00000022));
		
		game_double_buffer = GFX_draw_line_to_ptr8(game_double_buffer, game_width, 100, 50, strike_x + 25, strike_y - 80, rand_int(0x00000000, 0x00000022));
		game_double_buffer = GFX_draw_line_to_ptr8(game_double_buffer, game_width, 50, 100, strike_x + 25, strike_y - 80, rand_int(0x00000000, 0x00000022));
		
	}
	
	void left_game(void) {
		game_entity_count = 0;
		game_striking = FALSE;
		
		game_holding_left = FALSE;
		game_holding_right = FALSE;
		game_holding_jump = FALSE;
		game_holding_sprint = FALSE;
		
		player_x = 100;
		player_y = 100;
		
		player_colour = 0x00000000;
		eye_colour = 0x00FFFFFF;
		bg_colour = 0x00001122;
		
		camera_x = 0;
		camera_y = 0;
		
		strike_x = 0;
		strike_y = 0;
		
		dy = 0;
		speed = 5;
		
		game_width = GFX_get_mode_info('w');
		game_height = GFX_get_mode_info('h');
		
		game_double_buffer = (uint8*) kmalloc(game_width * game_height * 3);
		game_final_buffer = (uint8*) kmalloc(game_width * game_height * 3);
		
		game_fps = 30;
		timer_phase(game_fps);
		game_pid = pit_add_process(game_physics_loop);
		
		game_running = TRUE;
		while (game_running) {
			game_double_buffer = GFX_fill_rect_hex_to_ptr8(game_double_buffer, game_width, 0, 0, game_width, game_height, bg_colour);
			game_double_buffer = GFX_fill_rect_hex_to_ptr8(game_double_buffer, game_width, 50, 50, 50, 50, 0x00FFEECC);
			
			if (game_striking) {
				game_strike();
				
			}
			
			player_y -= 150;
			game_blit_model(GAME_MODEL_PLAYER, player_x, player_y);
			player_y += 150;
			
			int e;
			for (e = 0; e < game_entity_count; e++) {
				game_blit_model(game_entities[e].model, game_entities[e].x, game_entities[e].y);
				
			}
			
			GFX_blit_image(0, 0, game_width, game_height, game_double_buffer);
			
		}
		
		kfree(game_double_buffer, game_width * game_height * 3);
		kfree(game_final_buffer, game_width * game_height * 3);
		
		pit_remove_process(game_pid);
		game_running = FALSE;
		
	}
	
#endif
