////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\ControlEvent.hpp
//! \brief Provides argument/delegate/handler types for events from child controls 
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_CONTROL_EVENT_HPP
#define WTL_CONTROL_EVENT_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct ControlEventArgs - Event arguments for win32 messages from controls
  //! 
  //! \tparam ENC - Message character encoding 
  //! \tparam WM - Window message
  //! \tparam EVENT - [optional] Notification type (Default is uint16)
  //! \tparam CODE - [optional] Notification code (Default is zero)
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, WindowMessage WM, typename EVENT = uint16, EVENT CODE = zero<EVENT>::value>
  struct ControlEventArgs;
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \alias ControlEvent - Defines a child control event type 
  //! 
  //! \tparam ENC - Window character encoding
  //! \tparam WM - Window message
  //! \tparam EVENT - [optional] Notification message type (Default is uint16)
  //! \tparam CODE - [optional] Notification message (Default is zero)
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, WindowMessage WM, typename EVENT = uint16, EVENT CODE = zero<EVENT>::value>
  using ControlEvent = Event<LResult, ControlEventArgs<ENC,WM,EVENT,CODE>&>;
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \alias ControlEventHandler - Handlers for event from controls 
  //! 
  //! \tparam ENC - Window character encoding
  //! \tparam WM - Window message
  //! \tparam EVENT - [optional] Notification message type (Default is uint16)
  //! \tparam CODE - [optional] Notification message (Default is zero)
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, WindowMessage WM, typename EVENT = uint16, EVENT CODE = zero<EVENT>::value> 
  using ControlEventHandler = typename ControlEvent<ENC,WM,EVENT,CODE>::delegate_t;


  

}

#endif // WTL_CONTROL_EVENT_HPP
