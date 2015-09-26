//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\events\CtrlNotifyEvent.hpp
//! \brief Provides argument/delegate/handler types for WM_NOTIFY events from child controls 
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_CONTROL_NOTIFY_EVENT_HPP
#define WTL_CONTROL_NOTIFY_EVENT_HPP

#include "wtl/WTL.hpp"
#include "wtl/utils/Handle.hpp"                 //!< Handle
#include "wtl/traits/WindowTraits.hpp"          //!< HWnd
#include "wtl/windows/ControlEvent.hpp"         //!< ControlEventArgs
#include "wtl/platform/WindowFlags.hpp"         //!< WindowId
#include "wtl/platform/CommonApi.hpp"           //!< send_message

//! \namespace wtl - Windows template library
namespace wtl 
{
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
    template <Encoding ENC, typename EVENT, EVENT CODE>
    ControlEventArgs(const ControlEventArgs<ENC,WindowMessage::NOTIFY,EVENT,CODE>& r) : Header(r.Header),
                                                                                  Ident(r.Ident),
                                                                                  Sender(r.Sender),
                                                                                  Message(static_cast<event_t>(r.Message))
    {}
    
	  // -------------------------------- COPYING & DESTRUCTION -------------------------------
  
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
      return send_message<encoding,message+WindowMessage::REFLECT>(Sender, Ident, opaque_cast(Header));
    }
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------

  };


  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias CtrlNotifyEvent - Defines an event type for a child control event raised by WM_NOTIFY
  //! 
  //! \tparam ENC - Window character encoding
  //! \tparam EVENT - [optional] Notification message type (Default is uint16_t)
  //! \tparam CODE - [optional] Notification message (Default is zero)
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, typename EVENT = uint16_t, EVENT CODE = defvalue<EVENT>()>
  using CtrlNotifyEvent = ControlEvent<ENC,WindowMessage::NOTIFY,EVENT,CODE>;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias CtrlNotifyEventArgs - Defines arguments for event from controls raised via WM_NOTIFY 
  //! 
  //! \tparam ENC - Window character encoding
  //! \tparam EVENT - [optional] Notification message type (Default is uint16_t)
  //! \tparam CODE - [optional] Notification message (Default is zero)
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, typename EVENT = uint16_t, EVENT CODE = defvalue<EVENT>()>
  using CtrlNotifyEventArgs = ControlEventArgs<ENC,WindowMessage::NOTIFY,EVENT,CODE>;
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias CtrlNotifyEventHandler - Defines handler for event from controls raised via WM_NOTIFY 
  //! 
  //! \tparam ENC - Window character encoding
  //! \tparam EVENT - [optional] Notification message type (Default is uint16_t)
  //! \tparam CODE - [optional] Notification message (Default is zero)
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, typename EVENT = uint16_t, EVENT CODE = defvalue<EVENT>()>
  using CtrlNotifyEventHandler = ControlEventHandler<ENC,WindowMessage::NOTIFY,EVENT,CODE>;
  
} // namespace wtl

#endif // WTL_CONTROL_NOTIFY_EVENT_HPP
