//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\events\DestroyWindowEvent.hpp
//! \brief Encapsulates the WM_DESTROY message in the 'DestroyWindow' event
//! \date 25 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_DESTROY_WINDOW_EVENT_HPP
#define WTL_DESTROY_WINDOW_EVENT_HPP

#include "wtl/WTL.hpp"
#include "wtl/windows/MessageEvent.hpp"            //!< Event

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias DestroyWindowEvent - Defines the signature of the 'DestroyWindow' event handler  [No arguments]
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using DestroyWindowEvent = Event<LResult>;
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias DestroyWindowEventHandler - Defines the delegate type for the 'DestroyWindow' event
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using DestroyWindowEventHandler = handler_t<DestroyWindowEvent<ENC>>;

  
} // namespace wtl

#endif // WTL_DESTROY_WINDOW_EVENT_HPP
