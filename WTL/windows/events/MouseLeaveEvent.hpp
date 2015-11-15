//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\events\MouseLeaveEvent.hpp
//! \brief Encapsulates the WM_MOUSELEAVE message in the 'MouseLeave' event
//! \date 15 November 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_MOUSELEAVE_EVENT_HPP
#define WTL_MOUSELEAVE_EVENT_HPP

#include <wtl/WTL.hpp>
#include <wtl/windows/EventArgs.hpp>          //!< EventArgs

//! \namespace wtl - Windows template library
namespace wtl
{

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias MouseLeaveEventArgs - Defines arguments type for the 'MouseLeave' Event 
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using MouseLeaveEventArgs = EventArgs<ENC,WindowMessage::MouseLeave>;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias MouseLeaveEvent - Defines the signature of 'MouseLeave' event handlers  [Pass by value]
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using MouseLeaveEvent = Event<LResult,MouseLeaveEventArgs<ENC>>;
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias MouseLeaveEventHandler - Defines the delegate type for the 'MouseLeave' event
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using MouseLeaveEventHandler = handler_t<MouseLeaveEvent<ENC>>;

  
} // namespace wtl

#endif // WTL_MOUSELEAVE_EVENT_HPP
