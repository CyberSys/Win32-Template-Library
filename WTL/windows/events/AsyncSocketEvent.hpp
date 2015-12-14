//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\events\AsyncSocketEvent.hpp
//! \brief Encapsulates the messages sent by winsock in the 'AsyncSocket' event
//! \date 4 December 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_ASYNC_SOCKET_EVENT_HPP
#define WTL_ASYNC_SOCKET_EVENT_HPP

#include <wtl/WTL.hpp>
#include <wtl/casts/OpaqueCast.hpp>           //!< opaque_cast
#include <wtl/windows/EventArgs.hpp>          //!< EventArgs
#include <wtl/platform/SocketFlags.hpp>       //!< SocketEvent
//#include <wtl/utils/Handle.hpp>               //!< Handle
//#include <wtl/traits/SocketTraits.hpp>        //!< HSocket

//! \namespace wtl - Windows template library
namespace wtl
{

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct EventArgs<WindowMessage::AsyncSocket> - Arguments decoder for socket window notifications
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct EventArgs<ENC,WindowMessage::Socket> 
  {  
    // ---------------------------------- TYPES & CONSTANTS ----------------------------------
    
    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;
    
    //! \var encoding - Define message character encoding 
    static constexpr Encoding  encoding = ENC;
    
    //! \var message - Define message identifier
    static constexpr WindowMessage  message = WindowMessage::Socket;
    
    // ----------------------------------- REPRESENTATION -----------------------------------

    SocketEvent  Event;         //!< Notification code
    long         Result;        //!< Result/error code
    ::SOCKET     Socket;        //!< Socket handle [weak reference]
    
    // ------------------------------------- CONSTRUCTION -----------------------------------
	
    /////////////////////////////////////////////////////////////////////////////////////////
    // EventArgs<WindowMessage::AsyncSocket>::EventArgs
    //! Decode arguments for custom message sent by winsock
    //! 
    //! \param[in] w - Socket handle
    //! \param[in] l - Notification and error code packed into the LO and HO words respectively
    /////////////////////////////////////////////////////////////////////////////////////////
    EventArgs(::WPARAM w, ::LPARAM l) : Event(enum_cast<SocketEvent>(WSAGETSELECTEVENT(l))),
                                        Result(WSAGETSELECTERROR(l)),
                                        Socket(static_cast<::SOCKET>(w))
    {}
    
	  // -------------------------------- COPY, MOVE & DESTROY --------------------------------
  
    ENABLE_COPY(EventArgs);      //!< Can be shallow copied
    ENABLE_MOVE(EventArgs);      //!< Can be moved
    ENABLE_POLY(EventArgs);      //!< Can be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			

    // ----------------------------------- MUTATOR METHODS ----------------------------------

  };
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias AsyncSocketEventArgs - Defines arguments type for the 'AsyncSocket' Event 
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using AsyncSocketEventArgs = EventArgs<ENC,WindowMessage::Socket>;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias AsyncSocketEvent - Defines the signature of 'AsyncSocket' event handlers  [Pass by value]
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using AsyncSocketEvent = Event<LResult,AsyncSocketEventArgs<ENC>>;
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias AsyncSocketEventHandler - Defines the delegate type for the 'AsyncSocket' event
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using AsyncSocketEventHandler = handler_t<AsyncSocketEvent<ENC>>;

  
} // namespace wtl

#endif // WTL_ASYNC_SOCKET_EVENT_HPP
