////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\events\NotifyEvent.hpp
//! \brief Provides argument/delegate/handler types for events from controls via WM_NOTIFY
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_NOTIFY_EVENT_HPP
#define WTL_NOTIFY_EVENT_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct CtrlEventArgs<WindowMessage::NOTIFY> - Arguments decoder for events from controls passed via 'WM_NOTIFY'
  //! 
  //! \tparam ENC - Message character encoding 
  //! \tparam EVENT - Notification type
  //! \tparam CODE - Notification code
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, typename EVENT, EVENT CODE>
  struct CtrlEventArgs<ENC,WindowMessage::NOTIFY,EVENT,CODE> : EventArgs<ENC,WindowMessage::NOTIFY>
  {  
    // ------------------- TYPES & CONSTANTS --------------------
    
    //! \alias base - Define base type
    using base = EventArgs<ENC,WindowMessage::NOTIFY>;
    
    //! \alias data - Define notification data type
    using data_t = notify_data_t<ENC,EVENT,CODE>;

    //! \alias event_t - Define notification type
    using event_t = EVENT;

    //! \var code - Define notification identifier
    static constexpr EVENT  code = CODE;
    
    // --------------------- CONSTRUCTION ----------------------

    ///////////////////////////////////////////////////////////////////////////////
    // CtrlEventArgs<WindowMessage::NOTIFY>::CtrlEventArgs
    //! Decode arguments for win32 message 'WM_NOTIFY' 
    //! 
    //! \param[in] const& wnd - Window receiving message
    //! \param[in] w - Originator window id in the LO word, NotificationId in the HO word
    //! \param[in] l - Originator window handle
    ///////////////////////////////////////////////////////////////////////////////
    CtrlEventArgs(const HWnd& wnd, ::WPARAM w, ::LPARAM l) : Header(*opaque_cast<::NMHDR>(l)), 
                                                             Data(*opaque_cast<data_t>(l)), 
                                                             Ident(static_cast<WindowId>(Header.idFrom)), 
                                                             Window(Header.hwndFrom, AllocType::WeakRef), 
                                                             Message(static_cast<event_t>(Header.code))
    {}
    
    // ------------------------ STATIC -------------------------

    // ---------------------- ACCESSORS ------------------------			

    // ----------------------- MUTATORS ------------------------

    // -------------------- REPRESENTATION ---------------------
  protected:
    ::NMHDR&  Header;    //!< Message header (NB: Must be initialized before public members)
    data_t&   Data;      //!< Message data   (NB: Must be initialized before public members)
    
  public:
    WindowId  Ident;     //!< Originator window id
    HWnd      Window;    //!< Originator window handle
    EVENT     Message;   //!< Notification code
  };
  

  

  
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct CtrlEventDelegate<WindowMessage::NOTIFY> - Delegates a NOTIFY message to a handler of signature: LResult (sender_t&, arguments_t&)
  //! 
  //! \tparam ENC - Message character encoding 
  //! \tparam SENDER - Originator window type
  //! \tparam EVENTS - Events enumeration 
  //! \tparam CODE - Desired event
  //! \tparam ARGS - Event arguments type
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, typename SENDER, typename EVENTS, EVENTS CODE>
  struct CtrlEventDelegate<ENC,WindowMessage::NOTIFY,SENDER,EVENTS,CODE,void> 
       : MessageDelegate<ENC,WindowMessage::NOTIFY,LResult (SENDER&,CtrlEventArgs<ENC,WindowMessage::NOTIFY,EVENTS,CODE>&)>
  {
    // ------------------- TYPES & CONSTANTS --------------------

    //! \alias arguments_t - Define arguments decoder type
    using arguments_t = CtrlEventArgs<ENC,WindowMessage::NOTIFY,EVENTS,CODE>;

    //! \alias base - Define base type
    using base = MessageDelegate<ENC,WindowMessage::NOTIFY,LResult (SENDER&, arguments_t&)>;
    
    //! \alias event_t - Define notification type
    using event_t = EVENTS;
    
    //! \var event - Define notification identifier
    static constexpr event_t  event = CODE;
    
    //! \alias sender_t - Define originator window type
    using sender_t = SENDER;

    // --------------------- CONSTRUCTION ----------------------

    ///////////////////////////////////////////////////////////////////////////////
    // CtrlEventDelegate::CtrlEventDelegate
    //! Create a single parameter delegate from a member function of a window object
    //! 
    //! \tparam WINDOW - Window object type
    //! \tparam METHOD - Method pointer type
    //!
    //! \param[in] *object - Window object instance
    //! \param[in] method - Member function pointer to event handler method
    ///////////////////////////////////////////////////////////////////////////////
    template <typename WINDOW, typename METHOD>
    CtrlEventDelegate(WINDOW* object, METHOD method) : base(std::bind(method, object, std::placeholders::_1)),
                                                       Receiver(object)
    {}
    
    // ---------------------- ACCESSORS ------------------------			
    
    ///////////////////////////////////////////////////////////////////////////////
    // CtrlEventHandler::accept const
    //! Query the whether the handler accepts the message
    //! 
    //! \param[in] &wnd - Window receiving message
    //! \param[in] m - Window message
    //! \param[in] w - First parameter
    //! \param[in] l - Second parameter
    //! \return bool - True iff accepted by delegate
    ///////////////////////////////////////////////////////////////////////////////
    bool accept(const HWnd& wnd, WindowMessage m, ::WPARAM w, ::LPARAM l) const override
    {
      // [NOTIFY] Verify notification
      return m == WindowMessage::NOTIFY && arguments_t(wnd, w, l).Message == event;     //!< Instantiate arguments only after confirming WM_COMMAND
    }
    
    // ----------------------- MUTATORS ------------------------

    ///////////////////////////////////////////////////////////////////////////////
    // CtrlEventDelegate::invoke
    //! Invokes the delegate, executing the handler for handling, reflecting, or rejecting a message 
    //! 
    //! \param[in] &wnd - Window receiving message
    //! \param[in] w - First parameter
    //! \param[in] l - Second parameter
    //! \return LResult - [Unhandled] Handler is incapable or chose not to process this message
    //!                   [Handled] Handler processed this message
    //!                   [Reflected] Handler reflected this message
    ///////////////////////////////////////////////////////////////////////////////
    LResult invoke(const HWnd& wnd, ::WPARAM w, ::LPARAM l) 
    {
      arguments_t args(wnd, w, l);    //!< Decode arguments, bind to lifetime of handler execution

      // Lookup child window, cast to sender. Invoke handler and pass arguments
      return this->Impl( *reinterpret_cast<sender_t*>(Receiver->find(args.Ident)), args );
    }
    
    // -------------------- REPRESENTATION ---------------------
  protected:
    WindowBase<ENC>*  Receiver;    //!< Receiving window
  };

  
  ///////////////////////////////////////////////////////////////////////////////
  //! \alias NotifyEventHandler - Defines a handler type for events from controls raised via WM_NOTIFY
  //! 
  //! \tparam ENC - Message character encoding 
  //! \tparam SENDER - Originator window type
  //! \tparam EVENT - Notification type
  //! \tparam CODE - Notification code
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, typename SENDER, typename EVENTS, EVENTS CODE>
  using NotifyEventHandler = EventHandler<ENC,WindowMessage::NOTIFY,CtrlEventDelegate<ENC,WindowMessage::NOTIFY,SENDER,EVENTS,CODE>>;

}

#endif // WTL_NOTIFY_EVENT_HPP
