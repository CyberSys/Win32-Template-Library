//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\toString.hpp
//! \brief Converts any type to a string
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_TO_STRING_HPP
#define WTL_TO_STRING_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  // wtl::toString 
  //! Get the string representation of an enumeration value
  //! 
  //! \tparam ENUM - Enumeration type (for contiguous sets of values)
  //! 
  //! \param[in] e - Enumeration value
  //! \return const char* - String representation
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename ENUM, typename = enable_if_attribute_t<ENUM>> 
  const char* toString(ENUM e)
  {
    CHECKED_INDEX(e, min_value<ENUM>::value, max_value<ENUM>::value+1);

    // Lookup name
    return enum_names<ENUM>::values[static_cast<std::underlying_type_t<ENUM>>(e)
                                  - static_cast<std::underlying_type_t<ENUM>>(min_value<ENUM>::value)];
  }

  
  /////////////////////////////////////////////////////////////////////////////////////////
  // wtl::toString 
  //! Get the string representation of an enumeration value
  //! 
  //! \tparam ENUM - Enumeration type (for non-contiguous sets of values)
  //! 
  //! \param[in] e - Enumeration value
  //! \return const char* - String representation
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename ENUM> 
  const char* toString(ENUM e, std::enable_if_t<!is_attribute<ENUM>::value>* = nullptr)
  {
    CHECKED_INDEX(e, min_value<ENUM>::value, max_value<ENUM>::value+1);

    // Lookup value
    auto first = std::begin(enum_values<ENUM>::values);
    auto last = std::end(enum_values<ENUM>::values);
    auto v = std::find(first, last, e);
     
    // Return associated name
    return v != last ? enum_names<ENUM>::values[v - first] : "Error";
  }
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::parseEnum 
  //! Parses an enumeration value from its string representation 
  //! 
  //! \tparam ENUM - Enumeration type
  //! 
  //! \param[in] const* str - String representation
  //! \param[in,out] &value - Output enumeration value, if parsed successfully
  //! \return bool - True iff parsed successfully
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename ENUM, typename = enable_if_attribute_t<ENUM>> 
  inline bool parseEnum(const char* str, ENUM& value)
  {
    static_assert(std::is_enum<ENUM>::value, "Cannot parse from non-enumeration types");
    static_assert(is_attribute<ENUM>::value, "Cannot parse from non-iteratable enumerations");

    REQUIRED_PARAM(str);
    
    // Linear search, from min to max values
    for (ENUM it = min_value<ENUM>::value; it < max_value<ENUM>::value+1; it = it + 1)
      if (stricmp(str, toString(it)) == 0)
        // Set value and return true
        return (value = it, true);

    // Return false and set value to default
    return (value = default_t<ENUM>(), false);
  }

}



#endif // WTL_TO_STRING_HPP


