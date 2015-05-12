//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\LengthOf.hpp
//! \brief Provides the length of any type
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_LENGTH_OF_HPP
#define WTL_LENGTH_OF_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{  
  /////////////////////////////////////////////////////////////////////////////////////////
  // wtd::lengthof 
  //! Get the length of any object
  //! 
  //! \tparam T - Any type
  //! \return int32 - 1
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T> 
  constexpr int32 lengthof(const T&)
  {
    return 1;
  };
  
  /////////////////////////////////////////////////////////////////////////////////////////
  // wtd::lengthof 
  //! Get number of elements in a 1-dimensional array
  //! 
  //! \tparam T - Element type
  //! \tparam L - Length of array
  //! \return int32 - Number of elements
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, unsigned L> 
  constexpr int32 lengthof(const T (&array)[L])
  {
    return L;
  };
  
  /////////////////////////////////////////////////////////////////////////////////////////
  // wtd::lengthof 
  //! Get number of elements in a 2-dimensional array
  //! 
  //! \tparam T - Element type
  //! \tparam X - Rank of first dimension
  //! \tparam Y - Rank of second dimension
  //! \return int32 - Total number of elements in array
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, unsigned X, unsigned Y> 
  constexpr int32 lengthof(const T (&array)[X][Y])
  {
    return X * Y;
  };

}



#endif // WTL_LENGTH_OF_HPP


