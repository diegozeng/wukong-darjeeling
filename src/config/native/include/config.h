/*
 * config.h
 * 
 * Copyright (c) 2008-2010 CSIRO, Delft University of Technology.
 * 
 * This file is part of Darjeeling.
 * 
 * Darjeeling is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Darjeeling is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with Darjeeling.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#ifndef __config_h
#define __config_h
#include <sys/types.h>
#include <stdio.h>

// Allocate 4k heap for the VM
#define HEAPSIZE 4096

// 'Time slices' are 32 instructions
#define RUNSIZE 32

// #define PACK_STRUCTS
// #define ALIGN_16

/* Please see common/debug.h */
// #define DARJEELING_DEBUG true
// #define DARJEELING_DEBUG_FRAME
// #define DARJEELING_DEBUG_MEM_TRACE
// #define DARJEELING_DEBUG_TRACE
// #define DARJEELING_DEBUG_CHECK_HEAP_SANITY
// #define DARJEELING_DEBUG_PERFILE
// #define DBG_DARJEELING true
// #define DBG_DARJEELING_GC true
// #define DBG_WKPF true
// #define DBG_WKPFGC true
// #define DBG_WKPFUPDATE true
// #define DBG_WKCOMM true
// #define DBG_WKREPROG true
// #define DBG_WKROUTING true
// #define DBG_ZWAVETRACE true
// #define DBG_WIFI true

#define DARJEELING_PRINTF printf

#define DARJEELING_PGMSPACE_MACRO

// Routing: choose 1
#define ROUTING_USE_NONE
// #define ROUTING_USE_GATEWAY
// #define ROUTING_USE_DSDV
// #define ROUTING_USE_WUKONG

// Radios: choose as many as the routing protocol allows (max 1 for routing_none)
// #define RADIO_USE_ZWAVE
// #define RADIO_USE_XBEE
// #define RADIO_USE WIFI

#endif
