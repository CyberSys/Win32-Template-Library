//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\casts\BooleanCast.hpp
//! \brief Converts any type to a Win32 BOOL, and handles conversion between C++/Win32 booleans
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_BOOLEAN_CAST_HPP
#define WTL_BOOLEAN_CAST_HPP

#include "wtl/WTL.hpp"
#include "wtl/utils/Default.hpp"


//! \namespace wtl - Windows template library
namespace wtl
{
  //! \alias enable_if_not_BOOL_t - SFINAE expression requiring other than Win32 Boolean
  template <typename T, typename RET = void>
  using enable_if_not_BOOL_t = std::enable_if_t<!std::is_same<T,BOOL>::value,RET>;

  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::boolean_cast
  //! Converts any non-pointer type (except Win32 BOOL) to Win32 BOOL  (As TRUE (1) or FALSE (0))
  //! 
  //! \tparam T - Value type (exception Win32 BOOL)
  //!
  //! \param[in] value - Value
  //! \return BOOL - TRUE (1) if non-zero, otherwise FALSE (0)
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T>
  enable_if_not_BOOL_t<T,BOOL>  boolean_cast(T value)
  {
    return value != default<T>() ? True : False;
  };
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::boolean_cast
  //! Converts a Win32 BOOL to boolean
  //! 
  //! \param[in] value - Win32 BOOL
  //! \return bool - True iff non-zero
  //////////////////////////////////////////////////////////////////////////////////////////
  inline bool boolean_cast(BOOL value)
  {
    return value != default<BOOL>();
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
    return value != nullptr ? True : False;
  };
  
}



#endif // WTL_BOOLEAN_CAST_HPP


