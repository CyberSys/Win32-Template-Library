//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\event\CustomDrawEvent.hpp
//! \brief Encapsulates the NM_CUSTOMDRAW notification as the 'CustomDraw' event
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_CUSTOM_DRAW_EVENT_HPP
#define WTL_CUSTOM_DRAW_EVENT_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias StandardNotifyEventHandler - Handler for standard button events sent via COMMAND messages
  //! 
  //! \tparam ENC - Window character encoding type
  //! \tparam CODE - Notification code
  //! \tparam SIG - [optional] Signature of handler delegate 
  /////////////////////////////////////////////////////////////////////////////////////////
  /*template <Encoding ENC, typename SENDER, NotifyMessage CODE, typename SIG = LResult (WindowBase&, NotifyEventArgs<ENC,NotifyMessage,CODE>&)>
  using StandardNotifyEventHandler = NotifyEventHandler<ENC, SENDER, NotifyMessage, CODE, SIG>;*/
  

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias CustomDrawEventHandler - Handler for NM_CUSTOMDRAW notifications
  //! 
  //! \tparam ENC - Window character encoding type
  //! \tparam SENDER - Source window type
  /////////////////////////////////////////////////////////////////////////////////////////
  /*template <Encoding ENC, typename SENDER> using CustomDrawNotifyHandler = StandardNotifyEventHandler<ENC, SENDER, NotifyMessage::CustomDraw>;
  template <Encoding ENC, typename SENDER> using LeftClickNotifyHandler  = StandardNotifyEventHandler<ENC, SENDER, NotifyMessage::Click>;
  template <Encoding ENC, typename SENDER> using RightClickNotifyHandler = StandardNotifyEventHandler<ENC, SENDER, NotifyMessage::RClick>;*/
  
  
} // namespace wtl

#endif // WTL_CUSTOM_DRAW_EVENT_HPP
