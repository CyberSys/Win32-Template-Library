//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\controls\edut\EditUpdatedEvent.hpp
//! \brief Encapsulates the WM_COMMAND notification sent when the text of an Edit control changes within the 'Updated' event
//! \date 25 February 2016
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_EDIT_UPDATED_EVENT_HPP
#define WTL_EDIT_UPDATED_EVENT_HPP

#include <wtl/WTL.hpp>
#include <wtl/windows/ControlEventArgs.hpp>             //!< ControlEventArgs
#include <wtl/windows/controls/edit/EditConstants.hpp>  //!< EditNotification

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias EditUpdatedEventArgs - Defines arguments type for the Edit control 'Updated' Event 
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using EditUpdatedEventArgs = ControlEventArgs<ENC,WindowMessage::Command,EditNotification,EditNotification::Update>;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias EditUpdatedEvent - Defines the signature of the Edit control 'Updated' event handler  [Pass by value]
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using EditUpdatedEvent = Event<LResult, EditUpdatedEventArgs<ENC>>;
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias EditUpdatedEventHandler - Defines the delegate type for the Edit control 'Updated' event
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using EditUpdatedEventHandler = handler_t<EditUpdatedEvent<ENC>>;

} // namespace wtl

#endif // WTL_EDIT_UPDATED_EVENT_HPP
