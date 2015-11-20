//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\casts\StaticPtrCast.hpp
//! \brief Extend static_cast to pointer types, converting A* to B* iff A is implicitly convertible to B and A and B are the same size.
//! \date 20 November 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_STATIC_PTR_CAST_HPP
#define WTL_STATIC_PTR_CAST_HPP

#include <wtl/WTL.hpp>
#include <type_traits>                  //!< std::enable_if

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  // wtl::static_ptr_cast constexpr
  //! Convert a mutable pointer to type A to a similarly qualified pointer to type B iff an implicit conversion
  //! exists between type A and type B and if the types are of equal size
  //!
  //! \tparam FROM - Input type (A)
  //! \tparam TO - Output type (B)
  //!
  //! \param[in] *a - Mutable pointer to type A
  //! \return TO* - Mutable pointer to type B
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename FROM, typename TO, typename = enable_if_convertible_t<FROM,TO>, typename = enable_if_sizeof_t<FROM,TO>> constexpr
  std::remove_const_t<TO>*  static_ptr_cast(FROM* a) 
  {
    return reinterpret_cast<std::remove_const_t<TO>*>(a);
  }

  /////////////////////////////////////////////////////////////////////////////////////////
  // wtl::static_ptr_cast constexpr
  //! Convert an immutable pointer to type A to a similarly qualified pointer to type B iff an implicit conversion
  //! exists between type A and type B and if the types are of equal size
  //!
  //! \tparam FROM - Input type (A)
  //! \tparam TO - Output type (B)
  //!
  //! \param[in] const *a - Immutable pointer to type A
  //! \return const TO* - Immutable pointer to type B
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename FROM, typename TO, typename = enable_if_convertible_t<FROM,TO>, typename = enable_if_sizeof_t<FROM,TO>> constexpr
  const TO*  static_ptr_cast(const FROM* a) 
  {
    return reinterpret_cast<const TO*>(a);
  }

  /////////////////////////////////////////////////////////////////////////////////////////
  // wtl::static_ptr_cast constexpr
  //! Prevents static pointer casting between types of different sizes
  //!
  //! \tparam A - Input type 
  //! \tparam B - Output type 
  //!
  //! \param[in] *a - Pointer to an A
  //! \return B* - Pointer to a B
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename A, typename B, typename = enable_if_not_equal_t<sizeof(A), sizeof(B)>> constexpr
  B* static_ptr_cast(A* a)
  {
    static_assert(false, "Cannot static pointer cast between types of different sizes");
  }
  
  /////////////////////////////////////////////////////////////////////////////////////////
  // wtl::static_ptr_cast
  //! Prevent casting where conversion would lose qualifiers
  //!
  //! \tparam A - Input type
  //! \tparam B - Output type
  //!
  //! \param[in] *a - Immutable pointer to an A
  //! \return B* - Mutable pointer to a B
  /////////////////////////////////////////////////////////////////////////////////////////
  /*template <typename A, typename B, typename = std::enable_if_t<std::is_const<A>::value && !std::is_const<B>::value>>
  B* static_ptr_cast(const A* a)
  {
    static_assert(false, "Cannot use static pointer cast to remove qualifiers");
  }*/

  
}



#endif // WTL_STATIC_PTR_CAST_HPP


