/*
 * ac97.h
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


#ifndef CODEC_AC97_H
	#define CODEC_AC97_H
	
	#include "../types.h"
	#include "../pci/pci.h"
	
	#define N_ELEMENTS(arr) (sizeof(arr) / sizeof((arr)[0]))
	
	#define AC97_NAMBAR 0x10
	#define AC97_NABMBAR 0x14
	
	#define AC97_PO_BDBAR 0x10
	#define AC97_PO_CIV 0x14
	#define AC97_PO_LVI 0x15
	#define AC97_PO_SR 0x16
	#define AC97_PO_PICB 0x18
	#define AC97_PO_CR 0x1B

	#define AC97_BDL_LEN 32
	#define AC97_BDL_BUFFER_LEN 0x1000
	#define AC97_CL_GET_LENGTH(cl) ((cl) & 0xFFFF)
	#define AC97_CL_SET_LENGTH(cl, v) ((cl) = (v) & 0xFFFF)
	#define AC97_CL_BUP ((uint32) 1 << 30)
	#define AC97_CL_IOC ((uint32) 1 << 31)
	
	#define AC97_X_CR_RPBM (1 << 0)
	#define AC97_X_CR_RR (1 << 1)
	#define AC97_X_CR_LVBIE (1 << 2)
	#define AC97_X_CR_FEIE (1 << 3)
	#define AC97_X_CR_IOCE (1 << 4)
	
	#define AC97_X_SR_DCH (1 << 0)
	#define AC97_X_SR_CELV (1 << 1)
	#define AC97_X_SR_LVBCI (1 << 2)
	#define AC97_X_SR_BCIS (1 << 3)
	#define AC97_X_SR_FIFOE (1 << 3)
	
	#define AC97_RESET 0x00
	#define AC97_MASTER_VOLUME 0x02
	#define AC97_AUX_OUT_VOLUME 0x04
	#define AC97_MONO_VOLUME 0x06
	#define AC97_PCM_OUT_VOLUME 0x18
	
	#define AC97_SND_NAME "Intel AC'97"
	#define AC97_PLAYBACK_SPEED 48000
	#define AC97_PLAYBACK_FORMAT SND_FORMAT_L16SLE
	
	#define AC97_KNOB_PCM_OUT (SND_KNOB_VENDOR + 0
	
	typedef struct {
		uint32 pointer;
		uint32 cl;
		
	} ac97_bdl_entry;
	
	typedef struct {
		uint32 pci_device;
		uint16 nabmbar;
		uint16 nambar;
		
		uint32 irq;
		uint8 lvi;
		uint8 bits;
		
		ac97_bdl_entry* bdl;
		uint16* buffers[AC97_BDL_LEN]
		
		uint32 bdl_p;
		uint32 mask;
		
	} ac97_device;
	
#endif
