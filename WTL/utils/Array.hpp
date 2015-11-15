//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\Array.hpp
//! \brief Static array declaration file
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_ARRAY_HPP
#define WTL_ARRAY_HPP

#include <wtl/WTL.hpp>
#include <wtl/utils/Allocator.hpp>          //!< StaticAlloc
#include <wtl/io/Console.hpp>               //!< Debug console
#include <wtl/utils/DebugInfo.hpp>          //!< DebugInfo
#include <wtl/utils/NameValuePair.hpp>      //!< NameValuePair

//! \namespace wtl - Windows template library
namespace wtl
{
  //////////////////////////////////////////////////////////////////////////////////////////
  //! \struct array_traits - Defines array traits
  //! 
  //! \tparam DATA - Type used to specialize traits
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename DATA>
  struct array_traits
  {
    //! \alias alloc_t - Element allocator 
    using alloc_t = static_alloc<DATA>;
  };


  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct Array - Statically allocated, variable length, fixed capacity array. 
  //! This container is designed to be compatible with the standard template library algorithms.
  //! 
  //! \tparam DATA - Type of each element
  //! \tparam LENGTH - Maximum number of elements within the array
  //! \tparam DYNAMIC - Whether array can have a variable number of elements
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename DATA, uint32_t LENGTH, bool DYNAMIC>
  struct Array
  { /*Undefined*/ };

  
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator << 
  //! Writes a DynamicArray (with elements of non-class type) to the debug console
  //!
  //! \tparam E - Array element type
  //! \tparam L - Array capacity
  //!
  //! \param[in,out] &c - Console
  //! \param[in] const &r - Array
  //! \return Console& : Reference to console
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename E, uint32_t L>
  auto operator << (Console& c, const Array<E,L,true>& r) -> enable_if_not_class_t<E,Console&>
  {
    // Number of elements + comma separated values
    return c << object_info("Array", "size", r.size(), 
                                     "values", delimited_range(r, ','));
  }
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator << 
  //! Writes a DynamicArray (with elements of class type) to the debug console
  //!
  //! \tparam E - Array element type
  //! \tparam L - Array capacity
  //!
  //! \param[in,out] &c - Console
  //! \param[in] const &r - Array
  //! \return Console& : Reference to console
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename E, uint32_t L>
  auto operator << (Console& c, const Array<E,L,true>& r) -> enable_if_class_t<E,Console&>
  {
    // Number of elements + values on separate lines
    return c << object_info("Array", "size", r.size(), 
                                     "values", delimited_range(r, '\n'));
  }
  
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator << 
  //! Writes a FixedArray (with elements of non-class type) to the debug console
  //!
  //! \tparam E - Array element type
  //! \tparam L - Array capacity
  //!
  //! \param[in,out] &c - Console
  //! \param[in] const &r - Array
  //! \return Console& : Reference to console
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename E, uint32_t L>
  auto operator << (Console& c, const Array<E,L,false>& r) -> enable_if_not_class_t<E,Console&>
  {
    // Comma separated values
    return c << object_info("Array", "values", delimited_range(r, ','));
  }

  
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator << 
  //! Writes a FixedArray (with elements of class type) to the debug console
  //!
  //! \tparam E - Array element type
  //! \tparam L - Array capacity
  //!
  //! \param[in,out] &c - Console
  //! \param[in] const &r - Array
  //! \return Console& : Reference to console
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename E, uint32_t L>
  auto operator << (Console& c, const Array<E,L,false>& r) -> enable_if_class_t<E,Console&>
  {
    // Write elements on separate lines
    return c << object_info("Array", "values", delimited_range(r, '\n'));
  }
  
  
} // WTL namespace



#endif // WTL_ARRAY_HPP
