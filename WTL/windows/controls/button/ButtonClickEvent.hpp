//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\events\ButtonClickEvent.hpp
//! \brief Encapsulates the WM_COMMAND message in the 'ButtonClick' event
//! \date 1 December 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_BUTTON_CLICK_EVENT_HPP
#define WTL_BUTTON_CLICK_EVENT_HPP

#include <wtl/WTL.hpp>
#include <wtl/windows/ControlEventArgs.hpp>          //!< EventArgs
#include <wtl/platform/ControlStyles.hpp>            //!< ButtonNotification

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias ButtonClickEventArgs - Defines arguments type for the Button control 'Click' Event 
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using ButtonClickEventArgs = ControlEventArgs<ENC,WindowMessage::Command,ButtonNotification,ButtonNotification::Click>;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias ButtonClickEvent - Defines the signature of the Button control 'Click' event handler  [Pass by value]
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using ButtonClickEvent = Event<LResult, ButtonClickEventArgs<ENC>>;
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias ButtonClickEventHandler - Defines the delegate type for the Button control 'Click' event
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using ButtonClickEventHandler = handler_t<ButtonClickEvent<ENC>>;

} // namespace wtl

#endif // WTL_BUTTON_CLICK_EVENT_HPP
