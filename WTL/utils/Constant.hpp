//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\Constant.hpp
//! \brief Encapsulates a compile-time constant
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_CONSTANT_HPP
#define WTL_CONSTANT_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct complex_constant - Provides a compile-time constant of non-aggregate type
  //! 
  //! \tparam T - Any type  (Aggregates not supported)
  //! \tparam VALUE - Compile-time value
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, T VALUE> 
  struct complex_constant 
  {
    //! \alias type - Define own type
    using type = complex_constant<T,VALUE>;

    //! \alias value_type - Define value type
    using value_type = T;

    //! \var value - Define default value
    static constexpr value_type value = VALUE;

    constexpr 
    complex_constant()
    {}

    // return stored value
  	constexpr 
    value_type operator ()() const noexcept 
    { 
      return value; 
    }

    // return stored value
  	constexpr
    operator value_type() const noexcept 
    { 
      return value; 
    }
  };

}



#endif // WTL_CONSTANT_HPP


