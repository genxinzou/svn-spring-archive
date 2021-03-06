/**
 * @file SharedLib.cpp
 * @brief shared library loader implementation
 * @author Christopher Han <xiphux@gmail.com>
 *
 * Base shared library loading class implementation
 * Copyright (C) 2005.  Licensed under the terms of the
 * GNU GPL, v2 or later.
 */
#include "StdAfx.h"
#ifdef _WIN32
#include "Win/DllLib.h"
#else
#include "Linux/SoLib.h"
#endif

/**
 * Used to create a platform-specific shared library handler.
 */
SharedLib *SharedLib::Instantiate(const char *filename)
{
#ifdef _WIN32
	return new DllLib(filename);
#else
	return new SoLib(filename);
#endif
}

/**
 * Used to create a platform-specific shared library handler.
 */
SharedLib *SharedLib::Instantiate(std::string filename)
{
	return Instantiate(filename.c_str());
}


const char *SharedLib::GetLibExtension() 
{
#ifdef WIN32
	return "dll";
#elif defined(__APPLE__)
	return "dylib";
#else
	return "so";
#endif
}


SharedLib::~SharedLib() {
}
