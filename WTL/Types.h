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
#include <cstdint>

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

  //! \alias ulong32_t - Signed 32-bit long integer
  using long32_t  = signed long int;

  //! \alias ulong32_t - Unsigned 32-bit long integer
  using ulong32_t = unsigned long int;

  //! \alias long64_t - Signed 64-bit long long integer
  using long64_t = int64_t;

  //! \alias ulong64_t - Unsigned 64-bit long long integer
  using ulong64_t = uint64_t;

  //! \alias float32_t - 32-bit floating point
  using float32_t = float;

  //! \alias float64_t - 64-bit floating point
  using float64_t = double;
  
  //! \alias float80_t - Implementation dependant extended floating point type (80-bit on x86 as 12/16 bytes)
  using float80_t = long double;

  // ---------------------------------------------------------------------------------------------------------
  // --------------------------------------------- COMMON NAMES ----------------------------------------------
  // ---------------------------------------------------------------------------------------------------------

  //! \alias byte - Unsigned 8-bit integer 
  using byte = uint8_t;

  //! \alias word - Unsigned 16-bit integer 
  using word = uint16_t;

  //! \alias dword - Unsigned 32-bit integer 
  using dword = ulong32_t;

  
  // ---------------------------------------------------------------------------------------------------------
  // ---------------------------------------------- ARRAY TYPES ----------------------------------------------
  // ---------------------------------------------------------------------------------------------------------


  //! \alias array_ref_t - Defines array reference types
  template <typename ELEMENT, uint32_t LENGTH>
  using array_ref_t = ELEMENT (&)[LENGTH];
  
  //! \alias array_ptr_t - Defines array pointer types
  template <typename ELEMENT, uint32_t LENGTH>
  using array_ptr_t = ELEMENT (*)[LENGTH];
}

#endif // WTL_TYPES_HPP
