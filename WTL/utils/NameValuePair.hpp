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
  
}



#endif // WTL_NAME_VALUE_PAIR_HPP


