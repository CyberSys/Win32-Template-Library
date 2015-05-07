//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\casts\ConstExprCast.hpp
//! \brief Forces compile-time evaluation of an expression
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_CONSTEXPR_CAST_HPP
#define WTL_CONSTEXPR_CAST_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  // wtl::constexpr_cast constexpr
  //! Forces compile-time evaluation of an expression
  //! 
  //! \param[in] result - Input
  //! \return T - Returns copy of input
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T> constexpr 
  T  constexpr_cast(T result)
  {
    constexpr T var = result;    //!< Force evaluation

    return var;
  };
}



#endif // WTL_CONSTEXPR_CAST_HPP


