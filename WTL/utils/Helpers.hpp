//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\Helpers.hpp
//! \brief Collection of helper templates 
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_HELPERS_HPP
#define WTL_HELPERS_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  // --------------------------------- AUTO-CAST ---------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct auto_cast - Cast from any type A to any type B  (work in progress)
  //!
  //! \tparam BASE - Base type
  //! \tparam DERIED - Deried type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename BASE, typename DERIVED>
  DERIVED auto_cast(BASE b);


  template <typename DERIVED, typename BASE>
  std::enable_if_t<std::is_base_of<BASE,DERIVED>::value, DERIVED*>
  auto_cast(BASE* b)
  {
    return dynamic_cast<DERIVED*>(b);
  }

  template <typename TO, typename FROM>
  std::enable_if_t<std::is_convertible<FROM,TO>::value, TO&> 
  auto_cast(FROM& f)
  {
    return static_cast<TO&>(f);
  }

  template <typename BASE, typename DERIVED>
  std::enable_if_t<!std::is_base_of<BASE,DERIVED>::value
                && !std::is_convertible<BASE,DERIVED>::value,
                    DERIVED> 
  auto_cast(BASE& x)
  {
    return reinterpret_cast<DERIVED>(x);
  }


  // --------------------------------- CO-VARIANCE ---------------------------------

  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct is_covariant - Query whether a type is co-variant
  //!
  //! \tparam BASE - Base template type
  //! \tparam DERIED - Deried template type
  //! \tparam T - Common enclosed type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <template <typename> typename BASE, 
            template <typename> typename DERIVED, 
            typename T>
  struct is_covariant : std::is_base_of<BASE<T>, DERIVED<T>>
  {};

  
  // --------------------------------- CONSTRUCTION ---------------------------------
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::construct
  //! Constructs any object using any constructor
  //! 
  //! \tparam OBJ - Object type
  //! \tparam ARGS... - [optional] Constructor argument types
  //! 
  //! \param[in] &&... args - [optional] Constructor argument values
  //! \return auto - Object instance
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename OBJ, typename... ARGS>
  auto  construct(ARGS&&... args)
  {
    // Construct object
    return OBJ(std::forward<ARGS>(args)...);
  }



  
}


#endif // WTL_HELPERS_HPP


