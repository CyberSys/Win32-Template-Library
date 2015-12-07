//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\traits\MessageTraits.hpp
//! \brief Provides traits for individual window message types
//! \date 1 December 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_MESSAGE_TRAITS_HPP
#define WTL_MESSAGE_TRAITS_HPP

#include <wtl/WTL.hpp>
#include <wtl/platform/WindowMessage.hpp>     //!< WindowMessage
#include <wtl/platform/MsgResult.hpp>         //!< MsgRoute

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct message_traits - Provides traits for individual window message types
  //! 
  //! \tparam T - Window message type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T>
  struct message_traits;

  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct message_traits<WindowMessages> - Provides traits for window messages
  /////////////////////////////////////////////////////////////////////////////////////////
  template <>
  struct message_traits<WindowMessage>
  {
    /////////////////////////////////////////////////////////////////////////////////////////
    // wtl::routing 
    //! Determine whether a message was handled from its result
    //!
    //! \param[in] message - Window message 
    //! \param[in] res - Message result 
    //! \return bool - True iff result indicates message was unhandled
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename RESULT> 
    static MsgRoute routing(WindowMessage msg, RESULT res) 
    {
      switch (msg)
      {
      case WindowMessage::DrawItem:       return res == True ? MsgRoute::Handled : MsgRoute::Unhandled;
      case WindowMessage::GetText:        
      case WindowMessage::GetTextLength:  return MsgRoute::Handled;
      default:                            return res == 0    ? MsgRoute::Handled : MsgRoute::Unhandled;
      }
    }
  };
  

} //namespace wtl

#endif // WTL_MESSAGE_TRAITS_HPP

