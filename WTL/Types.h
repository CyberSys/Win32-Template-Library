////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\Types.h
//! \brief Defines wtl types
//! \date 6 March 2015
//! \author Nick Crowley
//! Copyright © Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_TYPES_HPP
#define WTL_TYPES_HPP

#include "WTL.hpp"

// Remove 'CHAR' typedef
#ifdef CHAR
  #undef CHAR
#endif

// Fundamental types
using int8    = int8_t;
using uint8   = uint8_t;
using int16   = int16_t;
using uint16  = uint16_t;
using int32   = int32_t;
using uint32  = uint32_t;
using long32  = signed long int;
using ulong32 = unsigned long int;
using int64   = int64_t;
using uint64  = uint64_t;
using float32 = float_t;
using float64 = double_t;

// Integers of data-bus width
//using intptr_t;
//using uintptr_t;

// Convenience aliases
using byte  = uint8;
using word  = uint16;
using dword = ulong32;

// Handles
//using Socket = intptr_t;
//typedef HMODULE           Module;


#endif // WTL_TYPES_HPP
