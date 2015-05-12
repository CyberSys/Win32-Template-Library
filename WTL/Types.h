//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\Types.h
//! \brief Defines fundemental wtl types
//! \date 6 March 2015
//! \author Nick Crowley
//! Copyright © Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_TYPES_HPP
#define WTL_TYPES_HPP

#include "WTL.hpp"
#include <stdint.h>

// Remove 'CHAR' typedef
#ifdef CHAR
  #undef CHAR
#endif

//! \namespace wtl - Windows template library
namespace wtl
{

  // Fundemental types
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
  using float32 = float;
  using float64 = double;

  
  // Standard types
  using intptr_t = int32;
  using uintptr_t = uint32;
  // using size_t = int32;
  using ptrdiff_t = int32;
  

  // Common names
  using byte  = uint8;
  using word  = uint16;
  using dword = ulong32;



  //! \alias array_ref_t - Defines array reference types
  template <typename ELEMENT, uint32 LENGTH>
  using array_ref_t = ELEMENT (&)[LENGTH];
  
  //! \alias array_ptr_t - Defines array pointer types
  template <typename ELEMENT, uint32 LENGTH>
  using array_ptr_t = ELEMENT (*)[LENGTH];
}

#endif // WTL_TYPES_HPP
