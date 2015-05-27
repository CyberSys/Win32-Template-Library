//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\Constant.hpp
//! \brief Encapsulates compile-time constants
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_CONSTANT_DATA_HPP
#define WTL_CONSTANT_DATA_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct integral_constant - Encapsulates an integral/enumeration compile-time constant
  //! 
  //! \tparam T - Integral or enumeration type  
  //! \tparam VALUE - (Compile-time) Value
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, T VALUE> 
  struct integral_constant 
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias type - Define own type
    using type = integral_constant<T,VALUE>;

    //! \alias value_type - Define value type
    using value_type = T;

    //! \var value - Define value
    static constexpr value_type value = VALUE;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  
    // ------------------------------------ CONSTRUCTION ------------------------------------
	
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
    
    DEFAULT_CONSTEXPR(integral_constant);         //!< Can be deep copied at compile-time
    DISABLE_COPY_ASSIGN(integral_constant);       //!< Immutable
    DISABLE_MOVE_ASSIGN(integral_constant);       //!< Immutable
    ENABLE_POLY(integral_constant);               //!< Can be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------

    // -------------------------------------- ACCESSORS -------------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // integral_constant::operator() constexpr
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
    // integral_constant::operator value_type constexpr
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

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct constant - Encapsulates any compile-time constant available via a static accessor
  //! 
  //! \tparam T - Any type
  //! \tparam GET - Any accessor function (with linkage) of signature "T (*)()"
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, T (*GET)()> 
  struct constant 
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias type - Define own type
    using type = constant<T,GET>;

    //! \alias value_type - Define value type
    using value_type = T;

    //! \alias reference_type - Define reference type
    using reference_type = const T&;

//! \if CONSTEXPR_SDMI_CAP - Initialize if SDMI supported
#ifdef CONSTEXPR_SDMI_CAP
    //! \var value - Define value
    static constexpr value_type value = GET();
#else
    //! \var value - Define value
    static const value_type value;
#endif

    // ----------------------------------- REPRESENTATION -----------------------------------
  
    // ------------------------------------ CONSTRUCTION ------------------------------------
	
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
    
    DEFAULT_CONSTEXPR(constant);         //!< Can be deep copied at compile-time
    DISABLE_COPY_ASSIGN(constant);       //!< Immutable
    DISABLE_MOVE_ASSIGN(constant);       //!< Immutable
    ENABLE_POLY(constant);               //!< Can be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------

    // -------------------------------------- ACCESSORS -------------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // constant::operator() constexpr
    //! Query value
    //! 
    //! \return reference_type - Reference to value
    /////////////////////////////////////////////////////////////////////////////////////////
  	constexpr 
    reference_type operator ()() const noexcept 
    { 
      return value;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // constant::operator reference_type constexpr
    //! Implicit user conversion to reference type
    //! 
    //! \return reference_type - Reference to value
    /////////////////////////////////////////////////////////////////////////////////////////
  	constexpr
    operator reference_type() const noexcept 
    { 
      return value;
    }
    
    // --------------------------------------- MUTATORS -------------------------------------
  };

//! \ifnot CONSTEXPR_SDMI_CAP - Initialize at runtime
#ifndef CONSTEXPR_SDMI_CAP
  template <typename T, T (*GET)()> 
  const T  constant<T,GET>::value = GET();
#endif

}



#endif // WTL_CONSTANT_DATA_HPP


