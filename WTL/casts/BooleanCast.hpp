//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\casts\BooleanCast.hpp
//! \brief Converts any type to a Win32 BOOL, and handles conversion between C++/Win32 booleans
//! \date 20 November 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_BOOLEAN_CAST_HPP
#define WTL_BOOLEAN_CAST_HPP

#include <wtl/WTL.hpp>
#include <wtl/utils/Default.hpp>          //!< defvalue

//! \namespace wtl - Windows template library
namespace wtl
{
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::boolean_cast constexpr
  //! Converts any type to BOOL (except BOOL itself) as either TRUE or FALSE
  //!
  //! \tparam T - Any type except BOOL
  //!
  //! \param[in] value - Value
  //! \return BOOL - TRUE (1) if 'value' not equal to value-initialized instance of T, otherwise FALSE (0)
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T> constexpr
  auto  boolean_cast(T value) -> enable_if_not_same_t<T,BOOL,BOOL>
  {
    return value != defvalue<T>() ? True : False;
  }

  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::boolean_cast constexpr
  //! Converts a BOOL to C++ boolean
  //!
  //! \param[in] value - Any BOOL value
  //! \return bool - True iff value is non-zero
  //////////////////////////////////////////////////////////////////////////////////////////
  constexpr
  inline bool boolean_cast(BOOL value) 
  {
    return value != defvalue<BOOL>();
  }

  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::boolean_cast constexpr
  //! Converts any object pointer type to BOOL as TRUE or FALSE
  //!
  //! \tparam T - Object pointer type
  //!
  //! \param[in] value - Any object pointer value
  //! \return BOOL - TRUE if value is non-null, FALSE otherwise
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T> constexpr
  BOOL boolean_cast(T* value)
  {
    return value != nullptr ? True : False;
  }

}



#endif // WTL_BOOLEAN_CAST_HPP


