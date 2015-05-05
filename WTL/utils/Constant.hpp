//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\Constant.hpp
//! \brief Encapsulates a compile-time constant
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_CONSTANT_HPP
#define WTL_CONSTANT_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct complex_constant - Provides a compile-time constant of non-aggregate type
  //! 
  //! \tparam T - Any type  (Aggregates not supported)
  //! \tparam VALUE - Compile-time value
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, T VALUE> 
  struct complex_constant 
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias type - Define own type
    using type = complex_constant<T,VALUE>;

    //! \alias value_type - Define value type
    using value_type = T;

    //! \var value - Define value
    static constexpr value_type value = VALUE;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  
    // ------------------------------------ CONSTRUCTION ------------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // complex_constant::complex_constant constexpr
    //! Default construct
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr 
    complex_constant()
    {}
    
    // --------------------------------------- STATIC ---------------------------------------

    // -------------------------------------- ACCESSORS -------------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // complex_constant::operator() constexpr
    //! Query value
    //! 
    //! \return value_type - Value
    /////////////////////////////////////////////////////////////////////////////////////////
  	constexpr 
    value_type operator ()() const noexcept 
    { 
      return value; 
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // complex_constant::operator value_type constexpr
    //! Implicit user conversion to value type
    //! 
    //! \return value_type - Value
    /////////////////////////////////////////////////////////////////////////////////////////
  	constexpr
    operator value_type() const noexcept 
    { 
      return value; 
    }
    
    // --------------------------------------- MUTATORS -------------------------------------
  };

}



#endif // WTL_CONSTANT_HPP


