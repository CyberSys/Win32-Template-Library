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
  
    // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // integral_constant::integral_constant constexpr
    //! Default construct
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr 
    integral_constant()
    {}
    
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
  //! \struct reference_constant - Encapsulates a reference type compile-time constant
  //! 
  //! \tparam T - Integral or enumeration type  
  //! \tparam VALUE - (Compile-time) Value
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, const T& VALUE> 
  struct reference_constant 
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias type - Define own type
    using type = reference_constant<T,VALUE>;

    //! \alias value_type - Define value type
    using value_type = T;

    //! \alias reference_type - Define reference type
    using reference_type = const T&;

//! \if CONSTEXPR_SDMI_CAP - Use static scope if SDMI supported
#ifdef CONSTEXPR_SDMI_CAP
    //! \var value - Define value
    static constexpr value_type value = VALUE;
#endif

    // ----------------------------------- REPRESENTATION -----------------------------------
  
    // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // reference_constant::reference_constant constexpr
    //! Default construct
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr 
    reference_constant()
    {}
    
    // ----------------------------------- STATIC METHODS -----------------------------------

    // -------------------------------------- ACCESSORS -------------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // reference_constant::operator() constexpr
    //! Query value
    //! 
    //! \return reference_type - Reference to value
    /////////////////////////////////////////////////////////////////////////////////////////
  	constexpr 
    reference_type operator ()() const noexcept 
    { 
//! \ifnot CONSTEXPR_SDMI_CAP - Copy-create 
#ifndef CONSTEXPR_SDMI_CAP
      return VALUE;
#else
      return value;
#endif
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // reference_constant::operator reference_type constexpr
    //! Implicit user conversion to reference type
    //! 
    //! \return reference_type - Reference to value
    /////////////////////////////////////////////////////////////////////////////////////////
  	constexpr
    operator reference_type() const noexcept 
    { 
//! \ifnot CONSTEXPR_SDMI_CAP - Copy-create 
#ifndef CONSTEXPR_SDMI_CAP
      return VALUE;
#else
      return value;
#endif
    }
    
    // --------------------------------------- MUTATORS -------------------------------------
  };


  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct literal_constant - Encapsulates a literal/reference compile-time constant 
  //! 
  //! \tparam T - Literal or reference type (NB: Must be default constructible)
  //!
  //! \remarks This is a temporary workaround until we get extended constexpr
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T> 
  struct literal_constant 
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias type - Define own type
    using type = literal_constant<T>;

    //! \alias value_type - Define value type
    using value_type = T;
  
//! \if CONSTEXPR_SDMI_CAP - Use static scope if SDMI supported
#ifdef CONSTEXPR_SDMI_CAP
    //! \var value - Define value
    static constexpr value_type value = value_type();
#endif
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  
    // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // literal_constant::literal_constant constexpr
    //! Default construct
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr 
    literal_constant()
    {}
    
    // ----------------------------------- STATIC METHODS -----------------------------------

    // -------------------------------------- ACCESSORS -------------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // literal_constant::operator() constexpr
    //! Query value
    //! 
    //! \return value_type - Value
    /////////////////////////////////////////////////////////////////////////////////////////
  	constexpr 
    value_type operator ()() const noexcept 
    { 
//! \ifnot CONSTEXPR_SDMI_CAP - Default construct
#ifndef CONSTEXPR_SDMI_CAP
      return value_type();
#else
      return value;
#endif
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // literal_constant::operator value_type constexpr
    //! Implicit user conversion to value type
    //! 
    //! \return value_type - Value
    /////////////////////////////////////////////////////////////////////////////////////////
  	constexpr
    operator value_type() const noexcept 
    { 
//! \ifnot CONSTEXPR_SDMI_CAP - Default construct
#ifndef CONSTEXPR_SDMI_CAP
      return value_type();
#else
      return value;
#endif
    }
    
    // --------------------------------------- MUTATORS -------------------------------------
  };

}



#endif // WTL_CONSTANT_DATA_HPP


