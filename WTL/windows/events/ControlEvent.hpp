////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\events\ControlEvent.hpp
//! \brief Provides argument/delegate/handler types for events from controls via WM_COMMAND
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
  //! \struct CtrlEventArgs<WindowMessage::COMMAND> - Arguments decoder for events from controls passed via 'WM_COMMAND'
  //! 
  //! \tparam ENC - Message character encoding 
  //! \tparam EVENT - Notification type
  //! \tparam CODE - Notification code
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, typename EVENT, EVENT CODE>
  struct CtrlEventArgs<ENC,WindowMessage::COMMAND,EVENT,CODE> //: EventArgs<ENC,WindowMessage::COMMAND>
  {  
    // ------------------- TYPES & CONSTANTS --------------------
    
    //! \alias base - Define base type
    //using base = EventArgs<ENC,WindowMessage::COMMAND>;

    //! \alias event_t - Define notification type
    using event_t = EVENT;
    
    //! \var code - Define notification identifier
    static constexpr EVENT  code = CODE;

    // --------------------- CONSTRUCTION ----------------------

    ///////////////////////////////////////////////////////////////////////////////
    // CtrlEventArgs<WindowMessage::COMMAND>::CtrlEventArgs
    //! Decode arguments for win32 message 'WM_COMMAND' 
    //! 
    //! \param[in] const& wnd - Window receiving message
    //! \param[in] w - Originator window id in the LO word, NotificationId in the HO word
    //! \param[in] l - Originator window handle
    ///////////////////////////////////////////////////////////////////////////////
    CtrlEventArgs(const HWnd& wnd, ::WPARAM w, ::LPARAM l) : Ident(enum_cast<WindowId>(LOWORD(w))), 
                                                             Window(reinterpret_cast<HWND>(l), AllocType::WeakRef), 
                                                             Message(static_cast<EVENT>(HIWORD(w)))
    {}
    
    // ------------------------ STATIC -------------------------

    // ---------------------- ACCESSORS ------------------------			

    // ----------------------- MUTATORS ------------------------

    // -------------------- REPRESENTATION ---------------------

    WindowId  Ident;     //!< Originator window id
    HWnd      Window;    //!< Originator window handle
    EVENT     Message;   //!< Notification code
  };
  

  
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct CtrlEventDelegate<WindowMessage::COMMAND> - Delegates a COMMAND message to a handler of signature: LResult (sender_t&)
  //! 
  //! \tparam ENC - Message character encoding 
  //! \tparam SENDER - Originator window type
  //! \tparam EVENTS - Events enumeration 
  //! \tparam CODE - Desired event
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, typename SENDER, typename EVENTS, EVENTS EVENT>
  struct CtrlEventDelegate<ENC,WindowMessage::COMMAND,SENDER,EVENTS,EVENT,void> : MessageDelegate<ENC,WindowMessage::COMMAND,LResult (SENDER&)>
  {
    // ------------------- TYPES & CONSTANTS --------------------

    //! \alias base - Define base type
    using base = MessageDelegate<ENC,WindowMessage::COMMAND,LResult (SENDER&)>;
    
    //! \alias arguments_t - Define arguments decoder type
    using arguments_t = CtrlEventArgs<ENC,WindowMessage::COMMAND,EVENTS,EVENT>;
    
    //! \alias event_t - Define notification type
    using event_t = EVENTS;
    
    //! \var event - Define notification identifier
    static constexpr event_t  event = EVENT;
    
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
    // CtrlEventDelegate::accept const
    //! Query the whether the delegate accepts the message
    //! 
    //! \param[in] &wnd - Window receiving message
    //! \param[in] m - Window message
    //! \param[in] w - First parameter
    //! \param[in] l - Second parameter
    //! \return bool - True iff accepted by delegate
    ///////////////////////////////////////////////////////////////////////////////
    bool accept(const HWnd& wnd, WindowMessage m, ::WPARAM w, ::LPARAM l) const override
    {
      // [COMMAND] Verify from a child control and verify notification
      return m == WindowMessage::COMMAND 
          && l != 0 
          && arguments_t(wnd, w, l).Message == event;    //!< Instantiate arguments only after confirming WM_COMMAND
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // CtrlEventDelegate::ident const
    //! Query the window message consumed by delegate
    //! 
    //! \return HandlerIdent - Message consumed by delegate
    ///////////////////////////////////////////////////////////////////////////////
    HandlerIdent ident() const override
    {
      // Return message and event
      return { message, event };
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

      // Lookup child window and invoke handler
      return this->Impl( Receiver->find<sender_t>(args.Ident) );
    }
    
    // -------------------- REPRESENTATION ---------------------
  protected:
    WindowBase<ENC>*  Receiver;    //!< Receiving window
  };

  
  ///////////////////////////////////////////////////////////////////////////////
  //! \alias CtrlEventHandler - Defines a handler type for events from controls raised via WM_COMMAND
  //! 
  //! \tparam ENC - Message character encoding 
  //! \tparam EVENT - Notification type
  //! \tparam CODE - Notification code
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, typename SENDER, typename EVENTS, EVENTS CODE>
  using CtrlEventHandler = EventHandler<ENC,WindowMessage::COMMAND,CtrlEventDelegate<ENC,WindowMessage::COMMAND,SENDER,EVENTS,CODE>>;


  
}

#endif // WTL_CONTROL_EVENT_HPP
