//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\casts\EnumCast.hpp
//! \brief Downcasts an enumeration literal to its underlying type
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_ENUMERATION_CAST_HPP
#define WTL_ENUMERATION_CAST_HPP

#include "wtl/WTL.hpp"
#include "wtl/utils/SFINAE.hpp"         //!< wtl::enable_if_enum_t
#include <type_traits>                  //!< std::enable_if, std::integral_constant

//! \namespace wtl - Windows template library
namespace wtl
{
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::enum_cast
  //! Converts an enumeration to its underlying type
  //! 
  //! \tparam T - Enumeration type
  //!
  //! \param[in] value - Enumeration value
  //! \return std::underlying_type<T> - Value as underlying type
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, typename = enable_if_enum_t<T>>
  std::underlying_type_t<T> enum_cast(T value)
  {
    return static_cast<std::underlying_type_t<T>>(value);
  };
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::enum_cast
  //! Converts an underlying type to an enumeration value
  //! 
  //! \tparam T - Enumeration type
  //!
  //! \param[in] value - Underlying value
  //! \return T - Enumeration value
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, typename = enable_if_enum_t<T>>
  T enum_cast(std::underlying_type_t<T> value)
  {
    return static_cast<T>(value);
  };
  
}



#endif // WTL_ENUMERATION_CAST_HPP


