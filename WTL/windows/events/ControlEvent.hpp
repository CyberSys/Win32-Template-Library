////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\events\ControlEvent.hpp
//! \brief Provides argument/delegate/handler types for events from controls via WM_COMMAND 
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_CONTROL_EVENT_HPP
#define WTL_CONTROL_EVENT_HPP

#include "wtl/WTL.hpp"
//#include "wtl/windows/ChildControlEvent.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  ///////////////////////////////////////////////////////////////////////////////
  //! \alias ControlEvent - Event from controls raised via WM_COMMAND
  //! 
  //! \tparam ENC - Window character encoding
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC> using ControlEvent = ChildControlEvent<ENC,WindowMessage::COMMAND,std::underlying_type_t<NotifyMessage>>;


  ///////////////////////////////////////////////////////////////////////////////
  //! \alias ControlEventArgs - Arguments for event from controls raised via WM_COMMAND 
  //! 
  //! \tparam ENC - Message character encoding 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC> using ControlEventArgs = typename ControlEvent<ENC>::arguments_t;
  
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \alias ControlEventHandler - Handlers for event from controls raised via WM_COMMAND 
  //! 
  //! \tparam ENC - Message character encoding 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC> using ControlEventHandler = typename ControlEvent<ENC>::delegate_t;
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct ctrl_delegate_signature<WindowMessage::COMMAND> - Defines function signature of handlers for events from controls (raised via WM_COMMAND)
  //! 
  //! \tparam ENC - Window character encoding
  //! \tparam EVENT - Notification message type
  //! \tparam CODE - Notification message 
  ///////////////////////////////////////////////////////////////////////////////
  /*template <Encoding ENC, typename EVENT, EVENT CODE> 
  struct ctrl_delegate_signature<ENC,WindowMessage::COMMAND,EVENT,CODE>  { using type = LResult (ControlEventArgs<ENC>&); };*/
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct ctrl_delegate_signature<WindowMessage::REFLECT_COMMAND> - Defines function signature of handlers for events reflected back to controls (raised via WM_COMMAND)
  //! 
  //! \tparam ENC - Window character encoding
  //! \tparam EVENT - Notification message type
  //! \tparam CODE - Notification message 
  ///////////////////////////////////////////////////////////////////////////////
  /*template <Encoding ENC, typename EVENT, EVENT CODE> 
  struct ctrl_delegate_signature<ENC,WindowMessage::REFLECT_COMMAND,EVENT,CODE>  { using type = LResult (ControlEventArgs<ENC>&); };*/




  
}

#endif // WTL_CONTROL_EVENT_HPP
