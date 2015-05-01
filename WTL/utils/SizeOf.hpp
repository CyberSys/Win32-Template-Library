////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\SizeOf.hpp
//! \brief Provides a generalised alternate size-of 
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_SIZE_OF_HPP
#define WTL_SIZE_OF_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////
  //! \struct size_of - Defines separate size for any type
  //! 
  //! \tparam T - Any type
  /////////////////////////////////////////////////////////////
  
  /*template <typename T> struct size_of { enum { value = sizeof(T) }; };
  template <>           struct size_of< { enum { value = sizeof(T) }; };*/

  /////////////////////////////////////////////////////////////
  //! \typedef size_of_t - Accessor for separate size 
  /////////////////////////////////////////////////////////////
  /*template <typename T>
  using size_of_t = typename size_of<T>::value;*/
  
  /////////////////////////////////////////////////////////////
  // wtd::size_of 
  //! Get separate size for any time
  //! 
  //! \tparam T - Any type
  //! \return int32 - Size of type, in bytes
  /////////////////////////////////////////////////////////////
  template <typename T> 
  constexpr int32 size_of(const T&)
  {
    return sizeof(T);
  };
  
  /////////////////////////////////////////////////////////////
  // wtd::size_of 
  //! Get number of elements in a 1-dimensional statically allocated array
  //! 
  //! \tparam T - Element type
  //! \tparam L - Length of array
  //! \return int32 - Number of elements
  /////////////////////////////////////////////////////////////
  template <typename T, unsigned L> 
  constexpr int32 size_of(const T (&array)[L])
  {
    return L;
  };
  
  /////////////////////////////////////////////////////////////
  // wtd::size_of 
  //! Get number of elements in a 2-dimensional statically allocated array
  //! 
  //! \tparam T - Element type
  //! \tparam X - Rank of first dimension
  //! \tparam Y - Rank of second dimension
  //! \return int32 - Total number of elements in array
  /////////////////////////////////////////////////////////////
  template <typename T, unsigned X, unsigned Y> 
  constexpr int32 size_of(const T (&array)[X][Y])
  {
    return X * Y;
  };

}



#endif // WTL_SIZE_OF_HPP

