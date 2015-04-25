////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\events\CloseWindowEvent.hpp
//! \brief Encapsulates the WM_CLOSE message in the 'CloseWindow' event
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_CLOSE_WINDOW_EVENT_HPP
#define WTL_CLOSE_WINDOW_EVENT_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \alias CloseWindowEvent - Defines 'CloseWindow' event (ie. WM_CLOSE)
  //! 
  //! \tparam ENC - Window character encoding
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using CloseWindowEvent = Event<ENC,LResult>;
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \alias CloseWindowEventHandler - Handler for 'CloseWindow' event (ie. WM_CLOSE)
  //! 
  //! \tparam ENC - Window character encoding
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using CloseWindowEventHandler = typename CloseWindowEvent<ENC>::delegate_t;


}

#endif // WTL_CLOSE_WINDOW_EVENT_HPP
