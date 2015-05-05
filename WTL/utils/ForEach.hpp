//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\ForEach.hpp
//! \brief Provides for-each overload for tuple iteration
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_FOR_EACH_HPP
#define WTL_FOR_EACH_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::for_each<0> - Base case
  //////////////////////////////////////////////////////////////////////////////////////////
  template <unsigned IDX = 0U, typename FUNC, typename... TYPES>
  std::enable_if_t<IDX == sizeof...(TYPES)>   //std::enable_if_t<IDX == std::tuple_size<TYPES...>::value>
  for_each(const std::tuple<TYPES...>& t, FUNC fn) 
  { 
    /*no-op*/ 
  }


  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::for_each
  //! Executes a unary function upon each tuple element 
  //! 
  //! \tparam IDX - Zero-based iteration index
  //! \tparam FUNC - Function type
  //! \tparam TYPES - Tuple types
  //!
  //! \param[in] t& - Tuple to iterate over
  //! \param[in] fn - Function to execute
  //////////////////////////////////////////////////////////////////////////////////////////
  template <unsigned IDX = 0U, typename FUNC, typename... TYPES>
  std::enable_if_t<(IDX < sizeof...(TYPES))>
  for_each(const std::tuple<TYPES...>& t, FUNC fn)
  {
    // Execute functor
    fn(std::get<IDX>(t));
    
    // Unfurl next
    for_each<IDX+1U>(t, fn);
  };
  
}



#endif // WTL_FOR_EACH_HPP


