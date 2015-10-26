//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\toString.hpp
//! \brief Converts any type to a string
//! \date 26 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_TO_STRING_HPP
#define WTL_TO_STRING_HPP

#include "wtl/WTL.hpp"
#include "wtl/traits/EnumTraits.hpp"        //!< enum_names, enum_values
#include "wtl/utils/LengthOf.hpp"           //!< lengthof
#include "wtl/utils/SFINAE.hpp"             //!< enable_if_enum_t

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  // wtl::to_string 
  //! Get the string representation of an enumeration value
  //! 
  //! \tparam ENUM - Enumeration type which specializes the 'enum_names' and 'enum_values' traits
  //! 
  //! \param[in] e - Enumeration value
  //! \return const char* - String representation if found, otherwise <Unrecognised>
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename ENUM>
  auto to_string(ENUM e) -> enable_if_enum_t<ENUM, const char*>
  {
    // Linear search for value
    for (int32_t i = 0; i < lengthof(enum_values<ENUM>::values); ++i)
      if (enum_values<ENUM>::values[i] == e)
        // [FOUND] Cross-reference with 'enum_names'
        return enum_names<ENUM>::values[i];

    // [NOT-FOUND] Return sentinel
    return "<Unrecognised>";
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
  template <typename ENUM> 
  auto parseEnum(const char* str, ENUM& value) -> enable_if_enum_t<ENUM, bool>
  {
    // Linear search for value
    for (int32_t i = 0; i < lengthof(enum_values<ENUM>::values); ++i)
      if (!stricmp(str, enum_names<ENUM>::values[i]))
      {
        // [FOUND] Cross-reference with 'enum_values'
        value = enum_values<ENUM>::values[i];
        return true;
      }

    // [NOT-FOUND] Return false
    value = defvalue<ENUM>();
    return false;
  }

}



#endif // WTL_TO_STRING_HPP


