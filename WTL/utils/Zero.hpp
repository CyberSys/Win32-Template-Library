//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\Zero.hpp
//! \brief Defines zero for any type
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
  //! \struct zero - Defines zero for any type
  //! 
  //! \tparam T - Any type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T> struct zero : std::integral_constant<T, static_cast<T>(0)>
  {};

  // Define for bool
  template <> struct zero<bool>   : std::integral_constant<bool, false> {};

  // Define for floating points 
  template <> struct zero<double> { static constexpr double value = 0.0; };
  template <> struct zero<float>  { static constexpr float value = 0.0f; };


  /////////////////////////////////////////////////////////////////////////////////////////
  //! \typedef zero_t - Zero type accessor 
  //! 
  //! \tparam T - Any type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T>
  using zero_t = typename zero<std::remove_const<T>>;

}



#endif // WTL_ZERO_HPP


