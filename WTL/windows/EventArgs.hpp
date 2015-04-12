////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\events\EventArgs.hpp
//! \brief Provides decoders for win32 message arguments
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_EVENT_ARGUMENTS_HPP
#define WTL_EVENT_ARGUMENTS_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct EventArgs - Encapsulates decoding win32 message arguments
  //! 
  //! \tparam ENC - Message character encoding 
  //! \tparam WM - Window message
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, WindowMessage WM> 
  struct EventArgs
  {    
    // ------------------- TYPES & CONSTANTS -------------------

    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;
    
    //! \var encoding - Define message character encoding 
    static constexpr Encoding  encoding = ENC;
    
    //! \var message - Define message identifier
    static constexpr WindowMessage  message = WM;

    //! \alias resource_t - Define resource id type
    using resource_t = ResourceId<ENC>;
    
    // --------------------- CONSTRUCTION ----------------------
    
    ///////////////////////////////////////////////////////////////////////////////
    // EventArgs::EventArgs
    //! Create argument decoder for messages with zero arguments
    ///////////////////////////////////////////////////////////////////////////////
    EventArgs()
    {}

    ///////////////////////////////////////////////////////////////////////////////
    // EventArgs::EventArgs
    //! Create argument decoder for messages with zero arguments
    //! 
    //! \param[in] &wnd - Window receiving message
    //! \param[in] w - [Ignored] First message parameter
    //! \param[in] l - [Ignored] Second message parameter
    ///////////////////////////////////////////////////////////////////////////////
    EventArgs(const HWnd& wnd, ::WPARAM w, ::LPARAM l)
    {}

    ///////////////////////////////////////////////////////////////////////////////
    // EventArgs::~EventArgs
    //! Virtual d-tor
    ///////////////////////////////////////////////////////////////////////////////
    virtual ~EventArgs()
    {}

  };

  
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct CtrlEventArgs - Event arguments for win32 messages from controls
  //! 
  //! \tparam ENC - Message character encoding 
  //! \tparam WM - Window message
  //! \tparam EVENT - Notification type
  //! \tparam CODE - Notification code
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, WindowMessage WM, typename EVENT, EVENT CODE>
  struct CtrlEventArgs;


}

#endif // WTL_EVENT_ARGUMENTS_HPP
