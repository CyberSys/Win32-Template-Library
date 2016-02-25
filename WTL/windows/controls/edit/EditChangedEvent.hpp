//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\controls\edut\EditChangedEvent.hpp
//! \brief Encapsulates the WM_COMMAND notification sent when the text of an Edit control changes within the 'Changed' event
//! \date 25 February 2016
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_EDIT_CHANGED_EVENT_HPP
#define WTL_EDIT_CHANGED_EVENT_HPP

#include <wtl/WTL.hpp>
#include <wtl/windows/ControlEventArgs.hpp>             //!< ControlEventArgs
#include <wtl/windows/controls/edit/EditConstants.hpp>  //!< EditNotification

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias EditChangedEventArgs - Defines arguments type for the Edit control 'Changed' Event 
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using EditChangedEventArgs = ControlEventArgs<ENC,WindowMessage::Command,EditNotification,EditNotification::Change>;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias EditChangedEvent - Defines the signature of the Edit control 'Changed' event handler  [Pass by value]
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using EditChangedEvent = Event<LResult, EditChangedEventArgs<ENC>>;
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias EditChangedEventHandler - Defines the delegate type for the Edit control 'Changed' event
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using EditChangedEventHandler = handler_t<EditChangedEvent<ENC>>;

} // namespace wtl

#endif // WTL_EDIT_CHANGED_EVENT_HPP
