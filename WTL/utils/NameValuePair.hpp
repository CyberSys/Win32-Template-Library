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
    /////////////////////////////////////////////////////////////////////////////////////////
    // NameValuePair::NameValuePair 
    //! Create a name-value pair
    //! 
    //! \param[in] const* name - Name
    //! \param[in] const& value - Value
    /////////////////////////////////////////////////////////////////////////////////////////
    NameValuePair(const char* name, const VALUE& value) : Name(name), Value(value)
    {}

    const char*   Name;     //!< Name
    const VALUE&  Value;    //!< Value reference
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


