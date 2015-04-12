////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\Types.h
//! \brief Defines gdk types
//! \date 6 March 2015
//! \author Nick Crowley
//! Copyright © Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_TYPES_HPP
#define WTL_TYPES_HPP

#include "WTL.hpp"

// Fundamental types
typedef signed char         int8;    
typedef unsigned char       uint8;
typedef signed short        int16;
typedef unsigned short      uint16;
typedef signed int          int32;
typedef unsigned int        uint32;
typedef signed long         long32;
typedef unsigned long       ulong32;
typedef signed long long    int64;
typedef unsigned long long  uint64;
typedef float               float32;
typedef double              float64;

// Aliases
typedef uint8             byte;
typedef uint8             Byte;
typedef ulong32           dword;
typedef uint64            dword64;
typedef uint16            word;

// Handles
typedef UINT_PTR          Socket;
typedef HMODULE           Module;


#endif // WTL_TYPES_HPP
