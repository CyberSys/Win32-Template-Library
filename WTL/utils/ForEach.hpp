//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\ForEach.hpp
//! \brief Provides a 'for-each' idiom for tuples, executing a unary callable target (which can maintain state) upon each element
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_FOR_EACH_HPP
#define WTL_FOR_EACH_HPP

#include "wtl/WTL.hpp"
#include <tuple>                  //!< std::tuple
#include <type_traits>            //!< std::enable_if_t

//! \namespace wtl - Windows template library
namespace wtl
{
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::for_each
  //! Passes each tuple element to a unary callable target
  //!
  //! \tparam IDX - Zero-based iteration index
  //! \tparam FUNC - Any unary callable target type
  //! \tparam ELEMS... - [optional] Tuple types
  //!
  //! \param[in,out] &t - Any tuple
  //! \param[in] fn - Unary callable target
  //! \return FUNC - Copy of 'fn' on final pass
  //!
  //! \remarks The callable target is not copied and may maintain state
  //////////////////////////////////////////////////////////////////////////////////////////
  template <unsigned IDX, typename FUNC, typename... ELEMS>
  auto for_each(const std::tuple<ELEMS...>& t, FUNC fn) -> std::enable_if_t<(IDX < sizeof...(ELEMS)), FUNC>
  {
    // Call target with current element
    fn(std::get<IDX>(t));

    // Unfurl next element, if any, and return target
    return for_each<IDX+1>(t, fn);
  }

  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::for_each<N>
  //! Stop after final element
  //////////////////////////////////////////////////////////////////////////////////////////
  template <unsigned IDX, typename FUNC, typename... ELEMS>
  auto for_each(const std::tuple<ELEMS...>& t, FUNC fn) -> std::enable_if_t<IDX == sizeof...(ELEMS), FUNC>
  {
    // Return target in final state
    return fn;
  }

}



#endif // WTL_FOR_EACH_HPP


