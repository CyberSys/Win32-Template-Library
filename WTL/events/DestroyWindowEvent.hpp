////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\event\DestroyWindowEvent.hpp
//! \brief Encapsulates the WM_DESTROY message in the 'DestroyWindow' event
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_DESTROY_WINDOW_EVENT_HPP
#define WTL_DESTROY_WINDOW_EVENT_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \alias DestroyWindowEventArgs - Arguments for 'DestroyWindow' Event (ie. WM_DESTROY)
  //! 
  //! \tparam ENC - Message character encoding 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using DestroyWindowEventArgs = EventArgs<ENC,WindowMessage::DESTROY>;

  ///////////////////////////////////////////////////////////////////////////////
  //! \alias DestroyWindowEventDelegate - Delegate for 'DestroyWindow' event (ie. WM_DESTROY)
  //! 
  //! \tparam ENC - Window character encoding 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using DestroyWindowEventDelegate = EventDelegate<ENC,WindowMessage::DESTROY>;

  ///////////////////////////////////////////////////////////////////////////////
  //! \alias DestroyWindowEventHandler - Handler for 'DestroyWindow' event (ie. WM_DESTROY)
  //! 
  //! \tparam ENC - Window character encoding 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using DestroyWindowEventHandler = EventHandler<ENC,WindowMessage::DESTROY,DestroyWindowEventDelegate<ENC>>;

}

#endif // WTL_DESTROY_WINDOW_EVENT_HPP
