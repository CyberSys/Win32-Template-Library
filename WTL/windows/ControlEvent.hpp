//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\events\ControlEvent.hpp
//! \brief Provides argument, delegate, and handler types for messages from child controls (WM_COMMAND/WM_NOTIFY)
//! \date 25 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_CONTROL_EVENT_HPP
#define WTL_CONTROL_EVENT_HPP

#include "wtl/WTL.hpp"
#include "wtl/utils/Handle.hpp"                 //!< Handle
#include "wtl/traits/WindowTraits.hpp"          //!< HWnd
#include "wtl/windows/Event.hpp"                //!< Event
#include "wtl/platform/WindowFlags.hpp"         //!< WindowId
#include "wtl/platform/CommonApi.hpp"           //!< send_message

//! \namespace wtl - Windows template library
namespace wtl 
{
  
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
  //! \struct ControlEventArgs<WindowMessage::COMMAND> - Arguments decoder for events from controls passed via 'WM_COMMAND'
  //! 
  //! \tparam ENC - Message character encoding 
  //! \tparam EVENT - Notification message type
  //! \tparam CODE - [optional] Notification message 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, typename EVENT, EVENT CODE>
  struct ControlEventArgs<ENC,WindowMessage::COMMAND,EVENT,CODE> 
  {  
    // ---------------------------------- TYPES & CONSTANTS ----------------------------------
    
    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;
    
    //! \alias event_t - Define notification message type
    using event_t = EVENT;
    
    //! \alias resource_t - Define resource id type
    using resource_t = ResourceId<ENC>;
    
    //! \var encoding - Define message character encoding 
    static constexpr Encoding  encoding = ENC;
    
    //! \var message - Define message identifier
    static constexpr WindowMessage  message = WindowMessage::COMMAND;
    
    //! \var unhandled - Define unhandled result
    static constexpr ::LRESULT  unhandled = unhandled_result<message>::value;

    // ----------------------------------- REPRESENTATION -----------------------------------

    WindowId  Ident;     //!< Originator window id
    HWnd      Sender;    //!< Originator window handle
    event_t   Message;   //!< Notification code

    // ------------------------------------- CONSTRUCTION -----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // ControlEventArgs<COMMAND>::ControlEventArgs
    //! Decode arguments for win32 message 'WM_COMMAND' 
    //! 
    //! \param[in] w - Originator window id in the LO word, NotificationId in the HO word
    //! \param[in] l - Originator window handle
    /////////////////////////////////////////////////////////////////////////////////////////
    ControlEventArgs(::WPARAM w, ::LPARAM l) : Ident(enum_cast<WindowId>(LOWORD(w))), 
                                               Sender(reinterpret_cast<HWND>(l), AllocType::WeakRef), 
                                               Message(static_cast<EVENT>(HIWORD(w)))
    {}
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ControlEventArgs<WindowMessage::NOTIFY>::ControlEventArgs
    //! Create from event arguments of another type
    //! 
    //! \param[in] const& r - Event arguments of another type
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding ALT_ENC, typename ALT_EVENT, ALT_EVENT ALT_CODE>
    ControlEventArgs(const ControlEventArgs<ALT_ENC,WindowMessage::COMMAND,ALT_EVENT,ALT_CODE>& r) : Ident(r.Ident),
                                                                                                     Sender(r.Sender),
                                                                                                     Message(static_cast<event_t>(r.Message))
    {}
    
	  // -------------------------------- COPY, MOVE & DESTROY  -------------------------------
  
    ENABLE_COPY(ControlEventArgs);      //!< Can be shallow copied
    ENABLE_MOVE(ControlEventArgs);      //!< Can be moved
    ENABLE_POLY(ControlEventArgs);      //!< Can be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ControlEventArgs<COMMAND>::reflect const
    //! Reflects the event back to the originator control
    //! 
    //! \return LResult - Message result and routing
    /////////////////////////////////////////////////////////////////////////////////////////
    LResult reflect() const
    {
      // Reflect message
      return send_message<encoding,WindowMessage::REFLECT_COMMAND>(Sender, opaque_cast(Ident,Message), opaque_cast(Sender.get()));
    }
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------

  };


  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct ControlEventArgs<WindowMessage::NOTIFY> - Arguments decoder for events from controls passed via 'WM_NOTIFY'
  //! 
  //! \tparam ENC - Message character encoding 
  //! \tparam EVENT - Notification message type
  //! \tparam CODE - Notification message
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, typename EVENT, EVENT CODE>
  struct ControlEventArgs<ENC,WindowMessage::NOTIFY,EVENT,CODE> 
  {  
    // ---------------------------------- TYPES & CONSTANTS ----------------------------------
    
    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;
    
    //! \alias event_t - Define notification message type
    using event_t = EVENT;
    
    //! \alias resource_t - Define resource id type
    using resource_t = ResourceId<ENC>;
    
    //! \var encoding - Define message character encoding 
    static constexpr Encoding  encoding = ENC;
    
    //! \var message - Define message identifier
    static constexpr WindowMessage  message = WindowMessage::NOTIFY;
    
    //! \var unhandled - Define unhandled result
    static constexpr ::LRESULT  unhandled = unhandled_result<message>::value;

    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    ::NMHDR&  Header;    //!< Message header (NB: Must be initialized before public members)
    
  public:
    WindowId  Ident;     //!< Originator window id
    HWnd      Sender;    //!< Originator window handle
    event_t   Message;   //!< Notification code

    // ------------------------------------- CONSTRUCTION -----------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // ControlEventArgs<WindowMessage::NOTIFY>::ControlEventArgs
    //! Decode arguments for win32 message 'WM_NOTIFY' 
    //! 
    //! \param[in] w - Originator window id in the LO word, NotificationId in the HO word
    //! \param[in] l - Originator window handle
    /////////////////////////////////////////////////////////////////////////////////////////
    ControlEventArgs(::WPARAM w, ::LPARAM l) : Header(*opaque_cast<::NMHDR>(l)), 
                                               Ident(static_cast<WindowId>(Header.idFrom)), 
                                               Sender(Header.hwndFrom, AllocType::WeakRef), 
                                               Message(static_cast<event_t>(Header.code))
    {}

    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ControlEventArgs<WindowMessage::NOTIFY>::ControlEventArgs
    //! Create from event arguments of another type
    //! 
    //! \param[in] const& r - Event arguments of another type
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding ALT_ENC, typename ALT_EVENT, ALT_EVENT ALT_CODE>
    ControlEventArgs(const ControlEventArgs<ALT_ENC,WindowMessage::NOTIFY,ALT_EVENT,ALT_CODE>& r) : Header(r.Header),
                                                                                                    Ident(r.Ident),
                                                                                                    Sender(r.Sender),
                                                                                                    Message(static_cast<event_t>(r.Message))
    {}
    
	  // -------------------------------- COPY, MOVE & DESTROY  -------------------------------
  
    ENABLE_COPY(ControlEventArgs);      //!< Can be shallow copied
    ENABLE_MOVE(ControlEventArgs);      //!< Can be moved
    ENABLE_POLY(ControlEventArgs);      //!< Can be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ControlEventArgs<NOTIFY>::reflect const
    //! Reflects the event back to the originator control
    //! 
    //! \return LResult - Message result and routing
    /////////////////////////////////////////////////////////////////////////////////////////
    LResult reflect() const
    {
      // Reflect message
      return send_message<encoding,WindowMessage::REFLECT_NOTIFY>(Sender, Ident, opaque_cast(Header));
    }
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------

  };
  
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias ControlEvent - Defines a child control event type 
  //! 
  //! \tparam ENC - Window character encoding
  //! \tparam WM - Window message
  //! \tparam EVENT - [optional] Notification message type (Default is uint16_t)
  //! \tparam CODE - [optional] Notification message (Default is zero)
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, WindowMessage WM, typename EVENT = uint16_t, EVENT CODE = defvalue<EVENT>()>
  using ControlEvent = Event<LResult, ControlEventArgs<ENC,WM,EVENT,CODE>&&>;
  
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias ControlEventHandler - Defines a handler type for child control events
  //! 
  //! \tparam ENC - Window character encoding
  //! \tparam WM - Window message
  //! \tparam EVENT - [optional] Notification message type (Default is uint16_t)
  //! \tparam CODE - [optional] Notification message (Default is zero)
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, WindowMessage WM, typename EVENT = uint16_t, EVENT CODE = defvalue<EVENT>()> 
  using ControlEventHandler = typename ControlEvent<ENC,WM,EVENT,CODE>::delegate_t;

  
} // namespace wtl

#endif // WTL_CONTROL_COMMAND_EVENT_HPP
