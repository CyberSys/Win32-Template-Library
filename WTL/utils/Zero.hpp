//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\Default.hpp
//! \brief Defines the default value for any type
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_ZERO_HPP
#define WTL_ZERO_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct default_t - Defines the default value for any type
  //! 
  //! \tparam T - Any type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T> struct default_t : integral_constant<T, static_cast<T>(0)>
  {
    constexpr 
    default_t()
    {}
  };
  //{
  //  //! \var value - Define default value
  //  static constexpr T value = static_cast<T>(0);

  //  // return stored value
  //	constexpr operator T() const noexcept { return value; }
  //};

  // Define for bool
  template <> struct default_t<bool> : integral_constant<bool, false> 
  {
    constexpr 
    default_t()
    {}
  };
  //{
  //  // return stored value
  //	constexpr operator bool() const noexcept { return this->value; }
  //};

  // Define for floating points 
  template <> struct default_t<double> { static constexpr double value = 0.0; };
  template <> struct default_t<float>  { static constexpr float value = 0.0f; };

  template <typename T> constexpr
  default_t<T> default() noexcept
  {
    return default_t<T>();
  }

}



#endif // WTL_ZERO_HPP


