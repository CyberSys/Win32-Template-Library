//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\NameValuePair.hpp
//! \brief Defines a name value pair
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_NAME_VALUE_PAIR_HPP
#define WTL_NAME_VALUE_PAIR_HPP

#include "wtl/WTL.hpp"
#include "wtl/io/Console.hpp"       //!< Console
#include "wtl/utils/Point.hpp"      //!< Point

//! \namespace wtl - Windows template library
namespace wtl
{
  
  //////////////////////////////////////////////////////////////////////////////////////////
  //! \struct NameValuePair - Provides an immutable name/value pair
  //! 
  //! \tparam VALUE - Value type
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename VALUE>
  struct NameValuePair
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    // ----------------------------------- REPRESENTATION -----------------------------------
  
    const char*   Name;     //!< Name
    const VALUE&  Value;    //!< Value reference

    // ------------------------------------ CONSTRUCTION ------------------------------------
	
    /////////////////////////////////////////////////////////////////////////////////////////
    // NameValuePair::NameValuePair 
    //! Create a name-value pair
    //! 
    //! \param[in] const* name - Name
    //! \param[in] const& value - Value
    /////////////////////////////////////////////////////////////////////////////////////////
    NameValuePair(const char* name, const VALUE& value) : Name(name), Value(value)
    {}
    
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
    
    ENABLE_COPY(NameValuePair);      //!< Can be deep copied
    ENABLE_MOVE(NameValuePair);      //!< Can be moved 

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    // ----------------------------------- MUTATOR METHODS ----------------------------------
    
  };


  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::name_value_pair 
  //! Creates a name/value pair
  //! 
  //! \tparam VALUE - Value type
  //! 
  //! \param[in] const* name - Name
  //! \param[in] const& value - Value
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename VALUE>
  NameValuePair<VALUE>  name_value_pair(const char* name, const VALUE& value)
  {
    return NameValuePair<VALUE>(name, value);
  }
  

  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator << 
  //! Writes a name-value pair to the debug console, with the name in yellow and value in white
  //!
  //! \tparam VALUE - Value type
  //! 
  //! \param[in,out] &c - Debug console
  //! \param[in] const& value - Name-Value pair representing a named value
  //! \return Console& : Reference to 'c'
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename VALUE>
  enable_if_is_not_t<VALUE,const char*,Console&>  operator << (Console& c, const NameValuePair<VALUE>& value)
  {
    // Write: Name=Value
    return c << Cons::Yellow << value.Name << '='
             << Cons::White  << value.Value;
  }
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator << 
  //! Writes a name-value pair to the debug console, with the name in yellow and value in white
  //!
  //! \tparam VALUE - const char*
  //! 
  //! \param[in,out] &c - Debug console
  //! \param[in] const& value - Name-Value pair representing a named value
  //! \return Console& : Reference to 'c'
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename VALUE>
  Console& operator << (Console& c, const NameValuePair<const char*>& value)
  {
    // Write: Name='Value'
    return c << Cons::Yellow << value.Name << "='"
             << Cons::White  << value.Value 
             << Cons::Yellow << "'";
  }

}



#endif // WTL_NAME_VALUE_PAIR_HPP


