////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\ControlEvent.hpp
//! \brief Provides argument/delegate/handler types for events from child controls 
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_CONTROL_EVENT_HPP
#define WTL_CONTROL_EVENT_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
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


  
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct CtrlEventArgs<WindowMessage::COMMAND> - Arguments decoder for events from controls passed via 'WM_COMMAND'
  //! 
  //! \tparam ENC - Message character encoding 
  //! \tparam EVENT - Notification message type
  //! \tparam CODE - [optional] Notification message 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, typename EVENT, EVENT CODE>
  struct CtrlEventArgs<ENC,WindowMessage::COMMAND,EVENT,CODE> 
  {  
    // ------------------- TYPES & CONSTANTS --------------------
    
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

    // -------------------- REPRESENTATION ---------------------

    WindowId  Ident;     //!< Originator window id
    HWnd      Sender;    //!< Originator window handle
    event_t   Message;   //!< Notification code

    // --------------------- CONSTRUCTION ----------------------

    ///////////////////////////////////////////////////////////////////////////////
    // CtrlEventArgs<COMMAND>::CtrlEventArgs
    //! Decode arguments for win32 message 'WM_COMMAND' 
    //! 
    //! \param[in] w - Originator window id in the LO word, NotificationId in the HO word
    //! \param[in] l - Originator window handle
    ///////////////////////////////////////////////////////////////////////////////
    CtrlEventArgs(::WPARAM w, ::LPARAM l) : Ident(enum_cast<WindowId>(LOWORD(w))), 
                                            Sender(reinterpret_cast<HWND>(l), AllocType::WeakRef), 
                                            Message(static_cast<EVENT>(HIWORD(w)))
    {}
    
    ///////////////////////////////////////////////////////////////////////////////
    // CtrlEventArgs<WindowMessage::NOTIFY>::CtrlEventArgs
    //! Create from event arguments of another type
    //! 
    //! \param[in] const& r - Event arguments of another type
    ///////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC, typename EVENT, EVENT CODE>
    CtrlEventArgs(const CtrlEventArgs<ENC,WindowMessage::COMMAND,EVENT,CODE>& r) : Ident(r.Ident),
                                                                                   Sender(r.Sender),
                                                                                   Message(static_cast<event_t>(r.Message))
    {}
    
    
    ///////////////////////////////////////////////////////////////////////////////
    // CtrlEventArgs::~CtrlEventArgs
    //! Virtual d-tor
    ///////////////////////////////////////////////////////////////////////////////
    virtual ~CtrlEventArgs()
    {}

    DEFAULT_COPY(CtrlEventArgs);
    
    // ------------------------ STATIC -------------------------

    // ---------------------- ACCESSORS ------------------------			
    
    ///////////////////////////////////////////////////////////////////////////////
    // CtrlEventArgs<COMMAND>::reflect const
    //! Reflects the event back to the originator control
    //! 
    //! \return LResult - Message result and routing
    ///////////////////////////////////////////////////////////////////////////////
    LResult reflect() const
    {
      // Reflect message
      return send_message<encoding,message+WindowMessage::REFLECT>(Sender, opaque_cast(Ident,Message), opaque_cast(Sender.get()));
    }
    
    // ----------------------- MUTATORS ------------------------

  };


  
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct CtrlEventArgs<WindowMessage::NOTIFY> - Arguments decoder for events from controls passed via 'WM_NOTIFY'
  //! 
  //! \tparam ENC - Message character encoding 
  //! \tparam EVENT - Notification message type
  //! \tparam CODE - Notification message
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, typename EVENT, EVENT CODE>
  struct CtrlEventArgs<ENC,WindowMessage::NOTIFY,EVENT,CODE> 
  {  
    // ------------------- TYPES & CONSTANTS --------------------
    
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

    // -------------------- REPRESENTATION ---------------------
  protected:
    ::NMHDR&  Header;    //!< Message header (NB: Must be initialized before public members)
    
  public:
    WindowId  Ident;     //!< Originator window id
    HWnd      Sender;    //!< Originator window handle
    event_t   Message;   //!< Notification code

    // --------------------- CONSTRUCTION ----------------------
  public:
    ///////////////////////////////////////////////////////////////////////////////
    // CtrlEventArgs<WindowMessage::NOTIFY>::CtrlEventArgs
    //! Decode arguments for win32 message 'WM_NOTIFY' 
    //! 
    //! \param[in] w - Originator window id in the LO word, NotificationId in the HO word
    //! \param[in] l - Originator window handle
    ///////////////////////////////////////////////////////////////////////////////
    CtrlEventArgs(::WPARAM w, ::LPARAM l) : Header(*opaque_cast<::NMHDR>(l)), 
                                            Ident(static_cast<WindowId>(Header.idFrom)), 
                                            Sender(Header.hwndFrom, AllocType::WeakRef), 
                                            Message(static_cast<event_t>(Header.code))
    {}

    
    ///////////////////////////////////////////////////////////////////////////////
    // CtrlEventArgs<WindowMessage::NOTIFY>::CtrlEventArgs
    //! Create from event arguments of another type
    //! 
    //! \param[in] const& r - Event arguments of another type
    ///////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC, typename EVENT, EVENT CODE>
    CtrlEventArgs(const CtrlEventArgs<ENC,WindowMessage::NOTIFY,EVENT,CODE>& r) : Header(r.Header),
                                                                                  Ident(r.Ident),
                                                                                  Sender(r.Sender),
                                                                                  Message(static_cast<event_t>(r.Message))
    {}
    
    
    ///////////////////////////////////////////////////////////////////////////////
    // CtrlEventArgs::~CtrlEventArgs
    //! Virtual d-tor
    ///////////////////////////////////////////////////////////////////////////////
    virtual ~CtrlEventArgs()
    {}

    DEFAULT_COPY(CtrlEventArgs);
    
    // ------------------------ STATIC -------------------------

    // ---------------------- ACCESSORS ------------------------			
    
    ///////////////////////////////////////////////////////////////////////////////
    // CtrlEventArgs<NOTIFY>::reflect const
    //! Reflects the event back to the originator control
    //! 
    //! \return LResult - Message result and routing
    ///////////////////////////////////////////////////////////////////////////////
    LResult reflect() const
    {
      // Reflect message
      return send_message<encoding,message+WindowMessage::REFLECT>(Sender, Ident, opaque_cast(Header));
    }
    
    // ----------------------- MUTATORS ------------------------

  };


  
  ///////////////////////////////////////////////////////////////////////////////
  //! \alias CtrlEvent - Defines a child control event type 
  //! 
  //! \tparam ENC - Window character encoding
  //! \tparam WM - Window message
  //! \tparam EVENT - [optional] Notification message type (Default is uint16)
  //! \tparam CODE - [optional] Notification message (Default is zero)
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, WindowMessage WM, typename EVENT = uint16, EVENT CODE = zero<EVENT>::value>
  using CtrlEvent = Event<ENC, LResult, CtrlEventArgs<ENC,WM,EVENT,CODE>&>;
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \alias CtrlEventHandler - Handlers for event from controls 
  //! 
  //! \tparam ENC - Window character encoding
  //! \tparam WM - Window message
  //! \tparam EVENT - [optional] Notification message type (Default is uint16)
  //! \tparam CODE - [optional] Notification message (Default is zero)
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, WindowMessage WM, typename EVENT = uint16, EVENT CODE = zero<EVENT>::value> 
  using CtrlEventHandler = typename CtrlEvent<ENC,WM,EVENT,CODE>::delegate_t;

  


  ///////////////////////////////////////////////////////////////////////////////
  //! \alias CtrlCommandEvent - Defines an event type for a child control event raised by WM_COMMAND
  //! 
  //! \tparam ENC - Window character encoding
  //! \tparam EVENT - [optional] Notification message type (Default is uint16)
  //! \tparam CODE - [optional] Notification message (Default is zero)
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, typename EVENT = uint16, EVENT CODE = zero<EVENT>::value>
  using CtrlCommandEvent = Event<ENC, LResult, CtrlEventArgs<ENC,WindowMessage::COMMAND,EVENT,CODE>&>;

  ///////////////////////////////////////////////////////////////////////////////
  //! \alias CtrlCommandEventArgs - Defines arguments for event from controls raised via WM_COMMAND 
  //! 
  //! \tparam ENC - Window character encoding
  //! \tparam EVENT - [optional] Notification message type (Default is uint16)
  //! \tparam CODE - [optional] Notification message (Default is zero)
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, typename EVENT = uint16, EVENT CODE = zero<EVENT>::value>
  using CtrlCommandEventArgs = CtrlEventArgs<ENC,WindowMessage::COMMAND,EVENT,CODE>;
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \alias CtrlCommandEventHandler - Defines handler for event from controls raised via WM_COMMAND 
  //! 
  //! \tparam ENC - Window character encoding
  //! \tparam EVENT - [optional] Notification message type (Default is uint16)
  //! \tparam CODE - [optional] Notification message (Default is zero)
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, typename EVENT = uint16, EVENT CODE = zero<EVENT>::value>
  using CtrlCommandEventHandler = typename CtrlCommandEvent<ENC,EVENT,CODE>::delegate_t;



  

  ///////////////////////////////////////////////////////////////////////////////
  //! \alias CtrlNotifyEvent - Defines an event type for a child control event raised by WM_NOTIFY
  //! 
  //! \tparam ENC - Window character encoding
  //! \tparam EVENT - [optional] Notification message type (Default is uint16)
  //! \tparam CODE - [optional] Notification message (Default is zero)
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, typename EVENT = uint16, EVENT CODE = zero<EVENT>::value>
  using CtrlNotifyEvent = Event<ENC, LResult, CtrlEventArgs<ENC,WindowMessage::NOTIFY,EVENT,CODE>&>;

  ///////////////////////////////////////////////////////////////////////////////
  //! \alias CtrlNotifyEventArgs - Defines arguments for event from controls raised via WM_NOTIFY 
  //! 
  //! \tparam ENC - Window character encoding
  //! \tparam EVENT - [optional] Notification message type (Default is uint16)
  //! \tparam CODE - [optional] Notification message (Default is zero)
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, typename EVENT = uint16, EVENT CODE = zero<EVENT>::value>
  using CtrlNotifyEventArgs = CtrlEventArgs<ENC,WindowMessage::NOTIFY,EVENT,CODE>;
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \alias CtrlNotifyEventHandler - Defines handler for event from controls raised via WM_NOTIFY 
  //! 
  //! \tparam ENC - Window character encoding
  //! \tparam EVENT - [optional] Notification message type (Default is uint16)
  //! \tparam CODE - [optional] Notification message (Default is zero)
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, typename EVENT = uint16, EVENT CODE = zero<EVENT>::value>
  using CtrlNotifyEventHandler = typename CtrlNotifyEvent<ENC,EVENT,CODE>::delegate_t;



  

  ///////////////////////////////////////////////////////////////////////////////
  //! \alias CtrlCommandEvent - Defines an event type for a child control event raised by WM_COMMAND
  //! 
  //! \tparam ENC - Window character encoding
  //! \tparam EVENT - [optional] Notification message type (Default is uint16)
  //! \tparam CODE - [optional] Notification message (Default is zero)
  ///////////////////////////////////////////////////////////////////////////////
  /*template <Encoding ENC, typename EVENT = uint16, EVENT CODE = zero<EVENT>::value>
  using ReflectCtrlCommand = Event<ENC, LResult, CtrlEventArgs<ENC,WindowMessage::REFLECT_COMMAND,EVENT,CODE>&>;*/

  ///////////////////////////////////////////////////////////////////////////////
  //! \alias CtrlCommandEventArgs - Defines arguments for event from controls raised via WM_COMMAND 
  //! 
  //! \tparam ENC - Window character encoding
  //! \tparam EVENT - [optional] Notification message type (Default is uint16)
  //! \tparam CODE - [optional] Notification message (Default is zero)
  ///////////////////////////////////////////////////////////////////////////////
  /*template <Encoding ENC, typename EVENT = uint16, EVENT CODE = zero<EVENT>::value>
  using ReflectCtrlCommandEventArgs = CtrlEventArgs<ENC,WindowMessage::REFLECT_COMMAND,EVENT,CODE>;*/
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \alias CtrlCommandEventHandler - Defines handler for event from controls raised via WM_COMMAND 
  //! 
  //! \tparam ENC - Window character encoding
  //! \tparam EVENT - [optional] Notification message type (Default is uint16)
  //! \tparam CODE - [optional] Notification message (Default is zero)
  ///////////////////////////////////////////////////////////////////////////////
  /*template <Encoding ENC, typename EVENT = uint16, EVENT CODE = zero<EVENT>::value>
  using CtrlCommandEventHandler = typename CtrlCommandEvent<ENC,EVENT,CODE>::delegate_t;*/

}

#endif // WTL_CONTROL_EVENT_HPP
