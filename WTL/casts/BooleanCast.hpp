//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\casts\BooleanCast.hpp
//! \brief Casts values and pointers to Win32 'BOOL' type (ie. int32)
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_BOOLEAN_CAST_HPP
#define WTL_BOOLEAN_CAST_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::boolean_cast
  //! Converts a value to BOOL as TRUE or FALSE
  //! 
  //! \tparam T - Value type
  //!
  //! \param[in] value - Value
  //! \return BOOL - TRUE if non-zero, otherwise FALSE
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T>
  BOOL boolean_cast(T value)
  {
    return value != default_t<T>::value ? TRUE : FALSE;
  };
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::enum_cast
  //! Converts a pointer to BOOL as TRUE or FALSE
  //! 
  //! \tparam T - Pointer type
  //!
  //! \param[in] value - Pointer value
  //! \return BOOL - TRUE if not nullptr, otherwise FALSE
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T>
  BOOL boolean_cast(T* value)
  {
    return value != nullptr ? TRUE : FALSE;
  };
  
}



#endif // WTL_BOOLEAN_CAST_HPP


