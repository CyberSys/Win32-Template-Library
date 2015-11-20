//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\casts\BaseCast.hpp
//! \brief Downcasts an object to its immediate base type
//! \date 20 November 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_BASE_CAST_HPP
#define WTL_BASE_CAST_HPP

#include <wtl/WTL.hpp>
#include <wtl/utils/Exception.hpp>          //!< wtl::exception

//! \namespace wtl - Windows template library
namespace wtl
{
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::base_cast constexpr
  //! Downcasts a type defining its own base class
  //!
  //! \tparam T - Any type with a public 'base' member type
  //!
  //! \param[in] &obj - Mutable object reference
  //! \return T::base& - Mutable reference to 'obj' base
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, typename BASE = typename T::base> constexpr
  BASE& base_cast(T& obj) 
  {
    return static_cast<BASE&>(obj);
  }

  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::base_cast constexpr
  //! Downcasts a type defining its own base class
  //!
  //! \tparam T - Any type with a public 'base' member type
  //!
  //! \param[in] const& obj - Immutable object reference
  //! \return const T::base& - Immutable reference to 'obj' base
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, typename BASE = typename T::base> constexpr
  const BASE&  base_cast(const T& obj)
  {
    return static_cast<const BASE&>(obj);
  }

  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::base_cast 
  //! Downcasts a type defining its own base class
  //!
  //! \tparam T - Any type with a public 'base' member type
  //!
  //! \param[in] *obj - Mutable object pointer
  //! \return T::base* - Mutable pointer to 'obj' base
  //!
  //! \throw wtl::invalid_argument - [Debug only] Missing argument
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, typename BASE = typename T::base> 
  BASE* base_cast(T* obj) noexcept(!CHECKED_ARGUMENTS)
  {
    REQUIRED_PARAM(obj);

    return static_cast<BASE*>(obj);
  }

  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::base_cast 
  //! Downcasts a type defining its own base class
  //!
  //! \tparam T - Any type with a public 'base' member type
  //!
  //! \param[in] const* obj - Object pointer
  //! \return const T::base* - Pointer to 'obj' base
  //!
  //! \throw wtl::invalid_argument - [Debug only] Missing argument
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, typename BASE = typename T::base>
  const BASE* base_cast(const T* obj) noexcept(!CHECKED_ARGUMENTS)
  {
    REQUIRED_PARAM(obj);

    return static_cast<const BASE*>(obj);
  }
}



#endif // WTL_BASE_CAST_HPP


