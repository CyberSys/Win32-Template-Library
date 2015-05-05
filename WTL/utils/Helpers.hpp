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

  template <template <typename> class BASE, 
            template <typename> class DERIVED, 
            typename T>
  struct is_covariant : std::is_base_of<BASE<T>, DERIVED<T>>
  {};
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct default_t - Defines the default value for any type
  //! 
  //! \tparam T - Any type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, T VALUE> 
  struct integral_constant 
  {
    using type = integral_constant<T,VALUE>;

    using value_type = T;

    //! \var value - Define default value
    static constexpr value_type value = VALUE;

    constexpr 
    integral_constant()
    {}

    // return stored value
  	constexpr 
    value_type operator ()() const noexcept 
    { 
      return value; 
    }

    // return stored value
  	constexpr
    operator value_type() const noexcept 
    { 
      return value; 
    }
  };

}



#endif // WTL_HELPERS_HPP


