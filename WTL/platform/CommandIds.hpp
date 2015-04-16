////////////////////////////////////////////////////////////////////////////////
//! \file wtl\platform\CommandIds.hpp
//! \brief Defines WTL Command Ids
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_COMMAND_IDS_HPP
#define WTL_COMMAND_IDS_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  //! \enum CommandId - Defines GUI Command Ids
  enum class CommandId : uint16
  {
    FILE_EXIT,            //!< Exit program
    // TODO: Import ID_FILE_OPEN, ... etc. from Win32 definitions if possible, otherwise MFC or WTL
  };
  
  
  //! Define traits: Non-contiguous Enumeration
  template <> struct is_attribute<CommandId>  : std::false_type  {};
  template <> struct is_contiguous<CommandId> : std::false_type  {};

  //! Define limits traits
  /*template <> struct max_value<CommandId>     : std::integral_constant<CommandId,CommandId::Invalid>   {};
  template <> struct min_value<CommandId>     : std::integral_constant<CommandId,CommandId::Black>     {};*/
  

  ///////////////////////////////////////////////////////////////////////////////
  //! wtl::command_id
  //! Creates a strongly typed command id from any integral or enumeration type
  //!
  //! \tparam TYPE - Integral or enumeration type
  //! 
  //! \param[in] id - Value representing command id
  //! \return CommandId - CommandId representation of 'id'
  ///////////////////////////////////////////////////////////////////////////////
  template <typename VALUE, typename = std::enable_if_t<std::is_integral<VALUE>::value || std::is_enum<VALUE>::value>>
  CommandId  command_id(VALUE id)
  {
    // Convert into underlying type then cast to enumeration
    return enum_cast<CommandId>( static_cast<std::underlying_type_t<CommandId>>(id) );
  }



}

#endif  // WTL_COMMAND_IDS_HPP
