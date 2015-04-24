////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\EventMessage.hpp
//! \brief Encapsulates handling a window message in an observeable event
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_EVENT_MESSAGE_HPP
#define WTL_EVENT_MESSAGE_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  

  ///////////////////////////////////////////////////////////////////////////////
  //! \struct delegate_signature - Defines the signature of window message event delegate
  //! 
  //! \tparam ENC - Message character encoding 
  //! \tparam WM - Window message
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, WindowMessage WM>
  struct delegate_signature { using type = LResult (EventArgs<ENC,WM>&); };

  ///////////////////////////////////////////////////////////////////////////////
  //! \alias delegate_signature_t - Type accessor window message event delegates
  //! 
  //! \tparam ENC - Message character encoding 
  //! \tparam WM - Window message
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, WindowMessage WM>
  using delegate_signature_t = typename delegate_signature<ENC,WM>::type; 

  


  ///////////////////////////////////////////////////////////////////////////////
  //! \struct ctrl_delegate_signature - Defines the signature of window message event delegate
  //! 
  //! \tparam ENC - Message character encoding 
  //! \tparam WM - Window message
  //! \tparam EVENT - Notification message type
  //! \tparam CODE - Notification message 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, WindowMessage WM, typename EVENT, EVENT CODE>
  struct ctrl_delegate_signature { using type = LResult (CtrlEventArgs<ENC,WM,EVENT,CODE>&); };

  ///////////////////////////////////////////////////////////////////////////////
  //! \alias ctrl_delegate_signature_t - Type accessor window message event delegates
  //! 
  //! \tparam ENC - Message character encoding 
  //! \tparam WM - Window message
  //! \tparam EVENT - Notification message type
  //! \tparam CODE - [optional] Notification message 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, WindowMessage WM, typename EVENT, EVENT CODE = zero<EVENT>::value>
  using ctrl_delegate_signature_t = typename ctrl_delegate_signature<ENC,WM,EVENT,CODE>::type; 



  ///////////////////////////////////////////////////////////////////////////////
  //! \struct MessageEvent - Encapsulates raising an event to handle a window message
  //! 
  //! \tparam ENC - Character encoding 
  //! \tparam WM - Window message
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, WindowMessage WM>
  struct MessageEvent : Event<ENC,delegate_signature_t<ENC,WM>>
  {
    // ------------------- TYPES & CONSTANTS -------------------
  
    //! \alias arguments_t - Define message argument decoder type
    using arguments_t = EventArgs<ENC,WM>;
    
    //! \alias base - Define base type
    using base = Event<ENC,delegate_signature_t<ENC,WM>>;
    
    // -------------------- REPRESENTATION ---------------------
  
    // --------------------- CONSTRUCTION ----------------------

    // ------------------------ STATIC -------------------------

    // ---------------------- ACCESSORS ------------------------			

    // ----------------------- MUTATORS ------------------------

  };



  ///////////////////////////////////////////////////////////////////////////////
  //! \struct ChildControlEvent - Encapsulates raising an event to handle a child control notification 
  //! 
  //! \tparam ENC - Character encoding 
  //! \tparam WM - Window message
  //! \tparam EVENT - Notification message type
  //! \tparam CODE - [optional] Notification message 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, WindowMessage WM, typename EVENT, EVENT CODE = zero<EVENT>::value>
  struct ChildControlEvent : Event<ENC,ctrl_delegate_signature_t<ENC,WM,EVENT,CODE>>
  {
    // ------------------- TYPES & CONSTANTS -------------------
  
    //! \alias base - Define base type
    using base = Event<ENC,ctrl_delegate_signature_t<ENC,WM,EVENT,CODE>>;
    
    //! \alias arguments_t - Define message argument decoder type
    using arguments_t = CtrlEventArgs<ENC,WM,EVENT,CODE>;

    //! \alias event_t - Define notification message type
    using event_t = EVENT;
    
    // -------------------- REPRESENTATION ---------------------
  
    // --------------------- CONSTRUCTION ----------------------

    // ------------------------ STATIC -------------------------

    // ---------------------- ACCESSORS ------------------------			

    // ----------------------- MUTATORS ------------------------

  };
  
  
}

#endif // WTL_EVENT_MESSAGE_HPP
