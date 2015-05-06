//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\casts\BaseCast.hpp
//! \brief Downcasts an object to its immediate base type
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_BASE_CAST_HPP
#define WTL_BASE_CAST_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::base_cast
  //! Downcasts a type defining its own base class
  //! 
  //! \tparam T - Any type with a public 'base' member type
  //!
  //! \param[in] &obj - Object reference
  //! \return T::base& - Reference to 'obj' base 
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, typename BASE = typename T::base>
  BASE& base_cast(T& obj)
  {
    return static_cast<BASE&>(obj);
  };
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::base_cast
  //! Downcasts a type defining its own base class
  //! 
  //! \tparam T - Any type with a public 'base' member type
  //!
  //! \param[in] *obj - Object pointer
  //! \return T::base* - Pointer to 'obj' base 
  //! 
  //! \throw wtl::invalid_argument - [Debug only] Missing argument
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, typename BASE = typename T::base>
  BASE* base_cast(T* obj)
  {
    REQUIRED_PARAM(obj);

    return static_cast<BASE*>(obj);
  };
  
}



#endif // WTL_BASE_CAST_HPP


