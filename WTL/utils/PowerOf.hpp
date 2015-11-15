//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\PowerOf.hpp
//! \brief Compile-time power-of-N calculator
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_POWER_OF_HPP
#define WTL_POWER_OF_HPP

#include <wtl/WTL.hpp>

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct power_of - Compile-time power-of-N calculator
  //! 
  //! \tparam N - Base 
  //! \tparam EXP - Exponent
  /////////////////////////////////////////////////////////////////////////////////////////
  template <int32_t N, int32_t EXP>
  struct power_of
  {
    static_assert(N > 0, "Base cannot be negative");
    static_assert(EXP > 0, "Exponent cannot be negative");

    // Multiply N by N, EXP times
    static constexpr int32_t value = N * power_of<N,EXP-1>::value;
  };

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct power_of<0> - Handle the base case
  /////////////////////////////////////////////////////////////////////////////////////////
  template <int32_t N>
  struct power_of<N, 0>
  {
    //! \var value = Define base case
    static constexpr int32_t value = 1;
  };

}



#endif // WTL_POWER_OF_HPP


