//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\casts\NativeCast.hpp
//! \brief Provides a programmable meta-function for converting from WTL to native types
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_NATIVE_CAST_HPP
#define WTL_NATIVE_CAST_HPP

#include "wtl/WTL.hpp"
#include "wtl/utils/SFINAE.hpp"         //!< wtl::enable_if_enum_t
#include <type_traits>                  //!< std::enable_if, std::integral_constant

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct native_conversion - Defines a set of meta-functions, each defining a conversion to a native type
  //! 
  //! \tparam T - Any type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, typename = void> 
  struct native_conversion;  /* Undefined */


  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::native_cast
  //! Converts any type to an equivalent native type iff a native conversion is defined
  //! 
  //! \tparam T - Input type
  //!
  //! \param[in] &obj - Mutable object reference
  //! \return native_conversion<T,U>::result_t& - Mutable reference to 'obj' as 'result_t'
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T>
  typename native_conversion<T>::result_t&  native_cast(T& obj)
  {
    return *reinterpret_cast<typename native_conversion<T>::result_t*>(&obj);
  };
  

  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::native_cast const
  //! Converts any type to an equivalent native type iff a native conversion is defined
  //! 
  //! \tparam T - Input type
  //!
  //! \param[in] const &obj - Immutable object reference
  //! \return const native_conversion<T,U>::result_t& - Immutable reference to 'obj' as 'result_t'
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T>
  const typename native_conversion<T>::result_t&  native_cast(const T& obj)
  {
    return *reinterpret_cast<const typename native_conversion<T>::result_t*>(&obj);
  };
}



#endif // WTL_NATIVE_CAST_HPP


