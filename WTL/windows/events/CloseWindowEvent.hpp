//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\events\CloseWindowEvent.hpp
//! \brief Encapsulates the WM_CLOSE message in the 'CloseWindow' event
//! \date 25 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_CLOSE_WINDOW_EVENT_HPP
#define WTL_CLOSE_WINDOW_EVENT_HPP

#include "wtl/WTL.hpp"
#include "wtl/windows/MessageEvent.hpp"            //!< MessageEvent

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias CloseWindowEvent - Defines the signature of the 'CloseWindow' event handler  [No arguments]
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using CloseWindowEvent = Event<LResult>;
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias CloseWindowEventHandler - Defines the delegate type for the 'CloseWindow' event
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using CloseWindowEventHandler = handler_t<CloseWindowEvent<ENC>>;

} // namespace wtl

#endif // WTL_CLOSE_WINDOW_EVENT_HPP
