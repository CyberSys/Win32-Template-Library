//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\controls\edut\EditMaxTextEvent.hpp
//! \brief Encapsulates the WM_COMMAND notification sent when the text of an Edit control changes within the 'MaxText' event
//! \date 25 February 2016
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_EDIT_MAXTEXT_EVENT_HPP
#define WTL_EDIT_MAXTEXT_EVENT_HPP

#include <wtl/WTL.hpp>
#include <wtl/windows/ControlEventArgs.hpp>             //!< ControlEventArgs
#include <wtl/windows/controls/edit/EditConstants.hpp>  //!< EditNotification

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias EditMaxTextEventArgs - Defines arguments type for the Edit control 'MaxText' Event 
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using EditMaxTextEventArgs = ControlEventArgs<ENC,WindowMessage::Command,EditNotification,EditNotification::MaxText>;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias EditMaxTextEvent - Defines the signature of the Edit control 'MaxText' event handler  [Pass by value]
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using EditMaxTextEvent = Event<LResult, EditMaxTextEventArgs<ENC>>;
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias EditMaxTextEventHandler - Defines the delegate type for the Edit control 'MaxText' event
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using EditMaxTextEventHandler = handler_t<EditMaxTextEvent<ENC>>;

} // namespace wtl

#endif // WTL_EDIT_MAXTEXT_EVENT_HPP
