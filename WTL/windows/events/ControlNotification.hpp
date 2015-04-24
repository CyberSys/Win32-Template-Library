////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\events\ControlNotification.hpp
//! \brief Provides argument/delegate/handler types for events from controls via WM_NOTIFY
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_CONTROL_NOTIFICATION_HPP
#define WTL_CONTROL_NOTIFICATION_HPP

#include "wtl/WTL.hpp"
//#include "wtl/windows/ChildControlEvent.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \alias ControlNotification - Generic event from controls raised via WM_NOTIFY
  //! 
  //! \tparam ENC - Window character encoding
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC> using ControlNotification = ChildControlEvent<ENC,WindowMessage::NOTIFY,std::underlying_type_t<NotifyMessage>>;


  ///////////////////////////////////////////////////////////////////////////////
  //! \alias ControlNotificationArgs - Arguments for event from controls raised via WM_NOTIFY 
  //! 
  //! \tparam ENC - Message character encoding 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC> using ControlNotificationArgs = typename ControlNotification<ENC>::arguments_t;
  
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \alias ControlNotificationHandler - Handlers for event from controls raised via WM_NOTIFY
  //! 
  //! \tparam ENC - Message character encoding 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC> using ControlNotificationHandler = typename ControlNotification<ENC>::delegate_t;
  

  ///////////////////////////////////////////////////////////////////////////////
  //! \struct ctrl_delegate_signature<WindowMessage::NOTIFY> - Defines function signature of handlers for events from controls (raised via WM_NOTIFY)
  //! 
  //! \tparam ENC - Window character encoding
  //! \tparam EVENT - Notification message type
  //! \tparam CODE - Notification message 
  ///////////////////////////////////////////////////////////////////////////////
  /*template <Encoding ENC, typename EVENT, EVENT CODE> 
  struct ctrl_delegate_signature<ENC,WindowMessage::NOTIFY,EVENT,CODE>  { using type = LResult (ControlNotificationArgs<ENC>&); };*/
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct ctrl_delegate_signature<WindowMessage::REFLECT_NOTIFY> - Defines function signature of handlers for events reflected back to controls (raised via WM_COMMAND)
  //! 
  //! \tparam ENC - Window character encoding
  //! \tparam EVENT - Notification message type
  //! \tparam CODE - Notification message 
  ///////////////////////////////////////////////////////////////////////////////
  /*template <Encoding ENC, typename EVENT, EVENT CODE> 
  struct ctrl_delegate_signature<ENC,WindowMessage::REFLECT_NOTIFY,EVENT,CODE>  { using type = LResult (ControlNotificationArgs<ENC>&); };*/


  
}

#endif // WTL_CONTROL_NOTIFICATION_HPP
