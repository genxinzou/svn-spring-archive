/*
	Copyright (c) 2008 Robin Vobruba <hoijui.quaero@gmail.com>

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _EXPORTDEFINES_H
#define _EXPORTDEFINES_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// extern declaration that will work across
// different platforms and compilers
#ifndef SHARED_EXPORT
	#ifdef __cplusplus
		#ifdef _WIN32
			#define SHARED_EXPORT extern "C" __declspec(dllexport)
			#define SPRING_API
		#elif __GNUC__ >= 4
		 // Support for '-fvisibility=hidden'.
			#define SHARED_EXPORT extern "C" __attribute__ ((visibility("default")))
			#define SPRING_API __attribute__ ((visibility("default")))
		#else
		 // Older versions of gcc have everything visible; no need for fancy stuff.
			#define SHARED_EXPORT extern "C"
			#define SPRING_API
		#endif
	#else // __cplusplus
	 // we dont have t oexport if we are in C already
		#define SHARED_EXPORT
	#endif // __cplusplus
#endif // SHARED_EXPORT

// calling convention declaration that will work across
// different and compilers
// for all 64bit platforms, we do not specify a calling convetion,
// as they all support only a single one anyway
// for windows 32bit, we use stdcall
// for non-windows 32bit, we use cdecl

#ifndef SPRING_CALLING_CONVENTION
	#if defined _WIN32
		#define SPRING_CALLING_CONVENTION stdcall
		#define SPRING_CALLING_CONVENTION_2 __stdcall
	#else // defined _WIN32
		#define SPRING_CALLING_CONVENTION cdecl
		#define SPRING_CALLING_CONVENTION_2 __cdecl
	#endif // defined _WIN32
#endif // SPRING_CALLING_CONVENTION

#ifndef CALLING_CONV
	#if defined _WIN64 || defined __LP64__ || defined __ppc64__ || defined __ILP64__ || defined __SILP64__ || defined __LLP64__
		#define CALLING_CONV
	#elif defined _WIN32
		#define CALLING_CONV SPRING_CALLING_CONVENTION_2
	#elif __GNUC__
		#define CALLING_CONV __attribute__ ((SPRING_CALLING_CONVENTION))
	#elif __INTEL_COMPILER
		#define CALLING_CONV __attribute__ ((SPRING_CALLING_CONVENTION))
	#else
		#define CALLING_CONV SPRING_CALLING_CONVENTION_2
	#endif // _WIN32
#endif // CALLING_CONV

// Intel Compiler compatibility fix
// Is used when assigning function pointers, for example in:
// ExternalAI/AIInterfaceLibrary.cpp
#ifndef CALLING_CONV_FUNC_POINTER
	#ifdef __INTEL_COMPILER
		#define CALLING_CONV_FUNC_POINTER
	#else
		#define CALLING_CONV_FUNC_POINTER CALLING_CONV
	#endif
#endif // CALLING_CONV_FUNC_POINTER


#define EXPORT(type) SHARED_EXPORT type CALLING_CONV


#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif // _EXPORTDEFINES_H
