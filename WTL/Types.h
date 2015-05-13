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
  // ---------------------------------------------------------------------------------------------------------
  // ----------------------------------------- FUNDEMENTAL TYPES ---------------------------------------------
  // ---------------------------------------------------------------------------------------------------------

  //! \alias int8 - Signed 8-bit integer
  using int8 = int8_t;

  //! \alias uint8 - Unsigned 8-bit integer
  using uint8 = uint8_t;

  //! \alias int16 - Signed 16-bit integer
  using int16 = int16_t;

  //! \alias uint16 - Unsigned 16-bit integer
  using uint16 = uint16_t;

  //! \alias int32 - Signed 32-bit integer
  using int32 = int32_t;

  //! \alias uint32 - Unsigned 32-bit integer
  using uint32  = uint32_t;

  //! \alias long32 - Signed 32-bit long integer
  using long32  = signed long int;

  //! \alias ulong32 - Unsigned 32-bit long integer
  using ulong32 = unsigned long int;

  //! \alias long64 - Signed 64-bit long long integer
  using long64 = int64_t;

  //! \alias uint64 - Unsigned 64-bit long long integer
  using ulong64 = uint64_t;

  //! \alias float32 - 32-bit floating point
  using float32 = float;

  //! \alias float64 - 64-bit floating point
  using float64 = double;

  // ---------------------------------------------------------------------------------------------------------
  // ------------------------------------------- STANDARD TYPES ----------------------------------------------
  // ---------------------------------------------------------------------------------------------------------
  
  //! \alias intptr_t - Signed integer type large enough to hold a pointer
  using intptr_t = int32;

  //! \alias uintptr_t - Unsigned integer type large enough to hold a pointer
  using uintptr_t = uint32;

  //! \alias size_t - Unsigned integer type large enough to represent any memory address
  //using size_t = uint32;

  //! \alias ptrdiff_t - Signed integer type able to represent any memory offset
  using ptrdiff_t = int32;
  
  // ---------------------------------------------------------------------------------------------------------
  // --------------------------------------------- COMMON NAMES ----------------------------------------------
  // ---------------------------------------------------------------------------------------------------------

  //! \alias byte - Unsigned 8-bit integer 
  using byte = uint8;

  //! \alias word - Unsigned 16-bit integer 
  using word = uint16;

  //! \alias dword - Unsigned 32-bit integer 
  using dword = ulong32;

  
  // ---------------------------------------------------------------------------------------------------------
  // ---------------------------------------------- ARRAY TYPES ----------------------------------------------
  // ---------------------------------------------------------------------------------------------------------


  //! \alias array_ref_t - Defines array reference types
  template <typename ELEMENT, uint32 LENGTH>
  using array_ref_t = ELEMENT (&)[LENGTH];
  
  //! \alias array_ptr_t - Defines array pointer types
  template <typename ELEMENT, uint32 LENGTH>
  using array_ptr_t = ELEMENT (*)[LENGTH];
}

#endif // WTL_TYPES_HPP
