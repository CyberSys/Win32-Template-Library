//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\events\CtrlCommandEvent.hpp
//! \brief Provides argument/delegate/handler types for WM_COMMAND events from child controls 
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_CONTROL_COMMAND_EVENT_HPP
#define WTL_CONTROL_COMMAND_EVENT_HPP

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
    template <Encoding ENC, typename EVENT, EVENT CODE>
    ControlEventArgs(const ControlEventArgs<ENC,WindowMessage::COMMAND,EVENT,CODE>& r) : Ident(r.Ident),
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
    // ControlEventArgs<COMMAND>::reflect const
    //! Reflects the event back to the originator control
    //! 
    //! \return LResult - Message result and routing
    /////////////////////////////////////////////////////////////////////////////////////////
    LResult reflect() const
    {
      // Reflect message
      return send_message<encoding,message+WindowMessage::REFLECT>(Sender, opaque_cast(Ident,Message), opaque_cast(Sender.get()));
    }
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------

  };

  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias CtrlCommandEvent - Defines an event type for a child control event raised by WM_COMMAND
  //! 
  //! \tparam ENC - Window character encoding
  //! \tparam EVENT - [optional] Notification message type (Default is uint16_t)
  //! \tparam CODE - [optional] Notification message (Default is zero)
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, typename EVENT = uint16_t, EVENT CODE = default<EVENT>()>
  using CtrlCommandEvent = ControlEvent<ENC,WindowMessage::COMMAND,EVENT,CODE>;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias CtrlCommandEventArgs - Defines arguments for event from controls raised via WM_COMMAND 
  //! 
  //! \tparam ENC - Window character encoding
  //! \tparam EVENT - [optional] Notification message type (Default is uint16_t)
  //! \tparam CODE - [optional] Notification message (Default is zero)
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, typename EVENT = uint16_t, EVENT CODE = default<EVENT>()>
  using CtrlCommandEventArgs = ControlEventArgs<ENC,WindowMessage::COMMAND,EVENT,CODE>;
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias CtrlCommandEventHandler - Defines handler for event from controls raised via WM_COMMAND 
  //! 
  //! \tparam ENC - Window character encoding
  //! \tparam EVENT - [optional] Notification message type (Default is uint16_t)
  //! \tparam CODE - [optional] Notification message (Default is zero)
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, typename EVENT = uint16_t, EVENT CODE = default<EVENT>()>
  using CtrlCommandEventHandler = ControlEventHandler<ENC,WindowMessage::COMMAND,EVENT,CODE>;

  
} // namespace wtl

#endif // WTL_CONTROL_COMMAND_EVENT_HPP
