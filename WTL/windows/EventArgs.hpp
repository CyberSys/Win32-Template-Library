//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\EventMessage.hpp
//! \brief Encapsulates handling a window message in an observeable event
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_EVENT_MESSAGE_HPP
#define WTL_EVENT_MESSAGE_HPP

#include "wtl/WTL.hpp"
#include "wtl/windows/Event.hpp"                        //!< Event
#include "wtl/platform/ResourceId.hpp"                  //!< ResourceId
#include "wtl/platform/WindowMessage.hpp"           //!< WindowMessage
#include "wtl/traits/EncodingTraits.hpp"                //!< Encoding

//! \namespace wtl - Windows template library
namespace wtl 
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct EventArgs - Encapsulates decoding win32 message arguments
  //! 
  //! \tparam ENC - Message character encoding 
  //! \tparam WM - Window message
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, WindowMessage WM> 
  struct EventArgs
  {    
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;
    
    //! \alias resource_t - Define resource id type
    using resource_t = ResourceId<ENC>;
    
    //! \var encoding - Define message character encoding 
    static constexpr Encoding  encoding = ENC;
    
    //! \var message - Define message identifier
    static constexpr WindowMessage  message = WM;
    
    //! \var unhandled - Define unhandled result
    static constexpr ::LRESULT  unhandled = unhandled_result<WM>::value;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
    
    // ------------------------------------- CONSTRUCTION -----------------------------------
	
    /////////////////////////////////////////////////////////////////////////////////////////
    // EventArgs::EventArgs
    //! Create argument decoder for messages with zero arguments
    /////////////////////////////////////////////////////////////////////////////////////////
    EventArgs()
    {}

    /////////////////////////////////////////////////////////////////////////////////////////
    // EventArgs::EventArgs
    //! Create from parameters
    //! 
    //! \param[in] w - First message parameter
    //! \param[in] l - Second message parameter
    /////////////////////////////////////////////////////////////////////////////////////////
    EventArgs(::WPARAM w, ::LPARAM l)
    {}
    
	  // -------------------------------- COPYING & DESTRUCTION -------------------------------
  public:
    ENABLE_COPY(EventArgs);      //!< Can be shallow copied
    ENABLE_MOVE(EventArgs);      //!< Can be moved
    ENABLE_POLY(EventArgs);      //!< Can be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------

  };
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct ControlEventArgs - Event arguments for win32 messages from controls
  //! 
  //! \tparam ENC - Message character encoding 
  //! \tparam WM - Window message
  //! \tparam EVENT - [optional] Notification type (Default is uint16_t)
  //! \tparam CODE - [optional] Notification code (Default is zero)
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, WindowMessage WM, typename EVENT = uint16_t, EVENT CODE = defvalue<EVENT>()>
  struct ControlEventArgs;
  /* Undefined */
  

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias MessageEvent - Defines an event encapsulating a window message 
  //! 
  //! \tparam ENC - Window character encoding
  //! \tparam WM - Window message
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, WindowMessage WM>
  using MessageEvent = Event<LResult, EventArgs<ENC,WM>&>;


  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias MessageEventHandler - Defines handler for any 'Message' event 
  //! 
  //! \tparam ENC - Window character encoding
  //! \tparam WM - Window message
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, WindowMessage WM>
  using MessageEventHandler = typename MessageEvent<ENC,WM>::delegate_t;
       
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias ControlEvent - Defines a child control event type 
  //! 
  //! \tparam ENC - Window character encoding
  //! \tparam WM - Window message
  //! \tparam EVENT - [optional] Notification message type (Default is uint16_t)
  //! \tparam CODE - [optional] Notification message (Default is zero)
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, WindowMessage WM, typename EVENT = uint16_t, EVENT CODE = defvalue<EVENT>()>
  using ControlEvent = Event<LResult, ControlEventArgs<ENC,WM,EVENT,CODE>&>;
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias ControlEventHandler - Handlers for event from controls 
  //! 
  //! \tparam ENC - Window character encoding
  //! \tparam WM - Window message
  //! \tparam EVENT - [optional] Notification message type (Default is uint16_t)
  //! \tparam CODE - [optional] Notification message (Default is zero)
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, WindowMessage WM, typename EVENT = uint16_t, EVENT CODE = defvalue<EVENT>()> 
  using ControlEventHandler = typename ControlEvent<ENC,WM,EVENT,CODE>::delegate_t;

} // namespace wtl

#endif // WTL_EVENT_MESSAGE_HPP
