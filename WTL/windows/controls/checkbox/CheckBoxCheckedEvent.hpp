//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\controls\checkbox\CheckBoxCheckedEvent.hpp
//! \brief Encapsulates the WM_COMMAND message sent when a checkbox is ticked in the 'Checked' event
//! \date 1 December 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_CHECKBOX_CHANGED_EVENT_HPP
#define WTL_CHECKBOX_CHANGED_EVENT_HPP

#include <wtl/WTL.hpp>
#include <wtl/windows/ControlEventArgs.hpp>          //!< EventArgs
#include <wtl/platform/ControlStyles.hpp>            //!< CheckBoxNotification

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias CheckBoxCheckedEventArgs - Defines arguments type for the CheckBox control 'Checked' Event 
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using CheckBoxCheckedEventArgs = ControlEventArgs<ENC,WindowMessage::Command,ButtonNotification,ButtonNotification::Click>;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias CheckBoxCheckedEvent - Defines the signature of the CheckBox control 'Checked' event handler  [Pass by value]
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using CheckBoxCheckedEvent = Event<LResult, CheckBoxCheckedEventArgs<ENC>>;
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias CheckBoxCheckedEventHandler - Defines the delegate type for the CheckBox control 'Checked' event
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using CheckBoxCheckedEventHandler = handler_t<CheckBoxCheckedEvent<ENC>>;

} // namespace wtl

#endif // WTL_CHECKBOX_CHANGED_EVENT_HPP
