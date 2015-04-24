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
    
    //! \alias resource_t - Define resource id type
    using resource_t = ResourceId<ENC>;
    
    //! \var encoding - Define message character encoding 
    static constexpr Encoding  encoding = ENC;
    
    //! \var message - Define message identifier
    static constexpr WindowMessage  message = WM;
    
    //! \var unhandled - Define unhandled result
    static constexpr ::LRESULT  unhandled = unhandled_result<WM>::value;
    
    // -------------------- REPRESENTATION ---------------------

    // --------------------- CONSTRUCTION ----------------------
    
    ///////////////////////////////////////////////////////////////////////////////
    // EventArgs::EventArgs
    //! Create argument decoder for messages with zero arguments
    ///////////////////////////////////////////////////////////////////////////////
    EventArgs()
    {}

    ///////////////////////////////////////////////////////////////////////////////
    // EventArgs::EventArgs
    //! Create from parameters
    //! 
    //! \param[in] w - First message parameter
    //! \param[in] l - Second message parameter
    ///////////////////////////////////////////////////////////////////////////////
    EventArgs(::WPARAM w, ::LPARAM l)
    {}

    ///////////////////////////////////////////////////////////////////////////////
    // EventArgs::~EventArgs
    //! Virtual d-tor
    ///////////////////////////////////////////////////////////////////////////////
    virtual ~EventArgs()
    {}
    
    // ------------------------ STATIC -------------------------

    // ---------------------- ACCESSORS ------------------------			
    
    // ----------------------- MUTATORS ------------------------

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
    // CtrlEventArgs::~CtrlEventArgs
    //! Virtual d-tor
    ///////////////////////////////////////////////////////////////////////////////
    virtual ~CtrlEventArgs()
    {}
    
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
    // CtrlEventArgs::~CtrlEventArgs
    //! Virtual d-tor
    ///////////////////////////////////////////////////////////////////////////////
    virtual ~CtrlEventArgs()
    {}
    
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
}

#endif // WTL_EVENT_ARGUMENTS_HPP
