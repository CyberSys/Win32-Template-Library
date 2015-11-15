//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\StaticPtrCast.hpp
//! \brief Extends the static_cast to pointers, converting A* to B* iff A is implicitly convertible
//! \brief to B, and A and B are the same size.
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_STATIC_PTR_CAST_HPP
#define WTL_STATIC_PTR_CAST_HPP

#include <wtl/WTL.hpp>
#include <type_traits>                  //!< std::enable_if, std::integral_constant

//! \namespace wtl - Windows template library
namespace wtl
{
  
  
  /////////////////////////////////////////////////////////////////////////////////////////
  // wtl::static_ptr_cast
  //! Prevents static pointer casting between types of different sizes
  //!
  //! \tparam B - Output type
  //! \tparam A - Input type
  //!
  //! \param[in] *a - Pointer to an A
  //! \return B* - Pointer to a B
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename B, typename A, typename std::enable_if_t<sizeof(A) != sizeof(B)>* = nullptr>
  B* static_ptr_cast(A* a)
  {
    static_assert(false, "Cannot static pointer cast between types of different sizes");
  }
  
  /////////////////////////////////////////////////////////////////////////////////////////
  // wtl::static_ptr_cast
  //! Prevent casting where conversion would lose qualifiers
  //!
  //! \tparam B - Output type
  //! \tparam A - Input type
  //!
  //! \param[in] *a - Immutable pointer to an A
  //! \return B* - Mutable pointer to a B
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename B, typename A, typename std::enable_if_t<std::is_const<A>::value    
                                                          && !std::is_const<B>::value>* = nullptr>
  B* static_ptr_cast(const A* a)
  {
    static_assert(false, "Cannot use static pointer cast to remove qualifiers");
  }

  /////////////////////////////////////////////////////////////////////////////////////////
  // wtl::static_ptr_cast
  //! Coerce a mutable pointer of type A*, to a mutable pointer of type B*, iff A and B are the same size
  //! and A is implicitly convertible to B.
  //!
  //! \tparam B - Output type
  //! \tparam A - Input type
  //!
  //! \param[in] *a - Mutable pointer to type A
  //! \return B* - Mutable pointer to type B
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename B, typename A, typename std::enable_if_t<!std::is_const<A>::value
                                                           && std::is_convertible<A,B>::value
                                                           && sizeof(A) == sizeof(B)>* = nullptr>
  std::remove_const_t<B>* static_ptr_cast(A* a)
  {
    // Reinterpret, return
    return reinterpret_cast<std::remove_const_t<B>*>(a);
  }

  /////////////////////////////////////////////////////////////////////////////////////////
  // wtl::static_ptr_cast
  //! Coerce an immutable pointer of type A*, to a similarly qualified pointer of type B*,
  //! iff A and B are the same size, and A is implicitly convertible to B.
  //!
  //! \tparam B - Output type
  //! \tparam A - Input type
  //!
  //! \param[in] const *a - Immutable pointer to type A
  //! \return const B* - Immutable pointer to type B
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename B, typename A, typename std::enable_if_t<std::is_convertible<A,B>::value
                                                           && sizeof(A) == sizeof(B)>* = nullptr>
  const B* static_ptr_cast(const A* a)
  {
    // Reinterpret, return
    return reinterpret_cast<B*>(a);
  }
  
}



#endif // WTL_STATIC_PTR_CAST_HPP


