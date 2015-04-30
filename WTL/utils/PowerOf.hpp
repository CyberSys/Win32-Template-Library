////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\PowerOf.hpp
//! \brief Compile-time power-of-N calculator
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_POWER_OF_HPP
#define WTL_POWER_OF_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////
  //! \struct power_of - Compile-time power-of-N calculator
  //! 
  //! \tparam N - Base 
  //! \tparam EXP - Exponent
  /////////////////////////////////////////////////////////////
  template <int32 N, int32 EXP>
  struct power_of
  {
    static_assert(N > 0L, "Invalid base");
    static_assert(EXP > 0L, "Invalid exponent");

    // Sum between 0 and N: N(N-1)
    enum { value = N * power_of<N,EXP-1>::value };
  };

  /////////////////////////////////////////////////////////////
  //! \struct power_of<N,0> - Handle the base case
  /////////////////////////////////////////////////////////////
  template <int32 N>
  struct power_of<N, 0>
  {
    enum { value = 1L };
  };

}



#endif // WTL_POWER_OF_HPP


