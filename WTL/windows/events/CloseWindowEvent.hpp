//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\events\CloseWindowEvent.hpp
//! \brief Encapsulates the WM_CLOSE message in the 'CloseWindow' event
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_CLOSE_WINDOW_EVENT_HPP
#define WTL_CLOSE_WINDOW_EVENT_HPP

#include "wtl/WTL.hpp"
#include "wtl/windows/MessageEvent.hpp"            //!< Event

//! \namespace wtl - Windows template library
namespace wtl {
//! \namespace events - WTL Window events
namespace events 
{

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias CloseWindowEvent - Defines 'CloseWindow' event (ie. WM_CLOSE)
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using CloseWindowEvent = Event<LResult>;
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias CloseWindowEventHandler - Handler for 'CloseWindow' event (ie. WM_CLOSE)
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using CloseWindowEventHandler = typename CloseWindowEvent<ENC>::delegate_t;

} // namespace events
} // namespace wtl

#endif // WTL_CLOSE_WINDOW_EVENT_HPP
