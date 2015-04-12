////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\events\EventDelegate.hpp
//! \brief Provides delegates for encapsulating invokation of handlers for win32 messages
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_EVENT_DELEGATE_HPP
#define WTL_EVENT_DELEGATE_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct delegate_signature - Defines the event delegate signature for each Win32 window message
  //! 
  //! \tparam ENC - Message character encoding 
  //! \tparam WM - Window message
  //! \tparam EVENT - Notification identifier type
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, WindowMessage WM>
  /*[[__depreciated__]]*/ struct delegate_signature { using type = LResult (EventArgs<ENC,WM>&); };
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \alias delegate_signature_t - Defines the event delegate signature for each Win32 window message
  //! 
  //! \tparam ENC - Message character encoding 
  //! \tparam WM - Window message
  //! \tparam EVENT - Notification identifier type
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, WindowMessage WM>
  /*[[__depreciated__]]*/ using delegate_signature_t = typename delegate_signature<ENC,WM>::type; 
  

  
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct MessageDelegate - Encapsulates decoding a Win32 message and invoking an event handler 
  //! 
  //! \tparam ENC - Message character encoding 
  //! \tparam WM - Window message
  //! \tparam SIG - [optional] Handler function signature
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, WindowMessage WM, typename SIG /*= delegate_signature_t<ENC,WM>*/>
  struct MessageDelegate 
  {
    // ------------------- TYPES & CONSTANTS -------------------

    //! \var encoding - Define encoding type
    static constexpr Encoding  encoding = ENC;

    //! \var message - Define message identifier
    static constexpr WindowMessage  message = WM;
    
    //! \alias signature_t - Define handler function signature 
    using signature_t = SIG;

    //! \alias delegate_t - Define delegate functor 
    using delegate_t = std::function<signature_t>;
    
    // --------------------- CONSTRUCTION ----------------------

    ///////////////////////////////////////////////////////////////////////////////
    // MessageDelegate::MessageDelegate
    //! Create a delegate from any callable target
    //! 
    //! \tparam ARGS - Callable target type (Function, lambda expression, bind expression, function object, pointer to member function, pointer to data member)
    //! \param[in] &&... - Event handler implementation (Function, lambda expression, bind expression, function object, pointer to member function, pointer to data member)
    ///////////////////////////////////////////////////////////////////////////////
    template <typename... ARGS>
    MessageDelegate(ARGS&&... args) : Impl(std::forward<ARGS>(args)...)
    {}
    
    // ---------------------- ACCESSORS ------------------------			
    
    ///////////////////////////////////////////////////////////////////////////////
    // MessageDelegate::accept const
    //! Query the whether the delegate accepts the message
    //! 
    //! \param[in] &wnd - Window receiving message
    //! \param[in] m - Window message
    //! \param[in] w - First parameter
    //! \param[in] l - Second parameter
    //! \return bool - True iff accepted by delegate
    ///////////////////////////////////////////////////////////////////////////////
    virtual bool accept(const HWnd& wnd, WindowMessage m, ::WPARAM w, ::LPARAM l) const 
    {
      // Simply check the message id
      return m == message;
    }
    
    // ----------------------- MUTATORS ------------------------
    
    ///////////////////////////////////////////////////////////////////////////////
    // MessageDelegate::invoke
    //! Invokes the delegate, executing the handler for handling, reflecting, or rejecting a message 
    //! 
    //! \param[in] &wnd - Window receiving message
    //! \param[in] w - First parameter
    //! \param[in] l - Second parameter
    //! \return LResult - [Unhandled] Handler is incapable or chose not to process this message
    //!                   [Handled] Handler processed this message
    //!                   [Reflected] Handler reflected this message
    ///////////////////////////////////////////////////////////////////////////////
    virtual LResult invoke(const HWnd& wnd, ::WPARAM w, ::LPARAM l) = 0;

    // -------------------- REPRESENTATION ---------------------
  protected:
    delegate_t  Impl;    //!< Handler routine
  };

  
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct EventDelegate - Calls an event handler of signature: LResult ()
  //! 
  //! \tparam ENC - Message character encoding 
  //! \tparam WM - Window message
  //! \tparam SIG - [optional] Handler function signature
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, WindowMessage WM>
  struct EventDelegate : MessageDelegate<ENC,WM,LResult ()>
  {
    // ------------------- TYPES & CONSTANTS --------------------

    //! \alias base - Define base type
    using base = MessageDelegate<ENC,WM,LResult ()>;
    
    // --------------------- CONSTRUCTION ----------------------

    ///////////////////////////////////////////////////////////////////////////////
    // EventDelegate::EventDelegate
    //! Create a single parameter delegate from a member function of a window object
    //! 
    //! \tparam WINDOW - Window object type
    //! \tparam METHOD - Method pointer type
    //!
    //! \param[in] *object - Window object instance
    //! \param[in] method - Member function pointer to event handler method
    ///////////////////////////////////////////////////////////////////////////////
    template <typename WINDOW, typename METHOD>
    EventDelegate(WINDOW* object, METHOD method) : base(std::bind(method, object))
    {}
    
    // ---------------------- ACCESSORS ------------------------			

    // ----------------------- MUTATORS ------------------------

    ///////////////////////////////////////////////////////////////////////////////
    // EventDelegate::invoke
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
      // Invoke handler
      return this->Impl();
    }
    
    // -------------------- REPRESENTATION ---------------------

  };
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct EventArgsDelegate - Calls a standard event handler of signature: LResult (EventArgs<WM>&)
  //! 
  //! \tparam ENC - Message character encoding 
  //! \tparam WM - Window message
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, WindowMessage WM>
  struct EventArgsDelegate : MessageDelegate<ENC,WM,LResult (EventArgs<ENC,WM>&)>
  {
    // ------------------- TYPES & CONSTANTS --------------------

    //! \alias base - Define base type
    using base = MessageDelegate<ENC,WM,LResult (EventArgs<ENC,WM>&)>;
    
    //! \alias arguments_t - Define message argument decoder type
    using arguments_t = EventArgs<ENC,WM>;
    
    // --------------------- CONSTRUCTION ----------------------

    ///////////////////////////////////////////////////////////////////////////////
    // EventArgsDelegate::EventArgsDelegate
    //! Create a single parameter delegate from a member function of a window object
    //! 
    //! \tparam WINDOW - Window object type
    //! \tparam METHOD - Method pointer type
    //!
    //! \param[in] *object - Window object instance
    //! \param[in] method - Member function pointer to event handler method
    ///////////////////////////////////////////////////////////////////////////////
    template <typename WINDOW, typename METHOD>
    EventArgsDelegate(WINDOW* object, METHOD method) : base(std::bind(method, object, std::placeholders::_1))
    {}
    
    // ---------------------- ACCESSORS ------------------------			

    // ----------------------- MUTATORS ------------------------

    ///////////////////////////////////////////////////////////////////////////////
    // EventArgsDelegate::invoke
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

      // Invoke handler, pass arguments by-ref
      return this->Impl(args);
    }
    
    // -------------------- REPRESENTATION ---------------------

  };


  ///////////////////////////////////////////////////////////////////////////////
  //! \struct CtrlEventDelegate - Delegates to a handler of signature: LResult (sender_t&)
  //! 
  //! \tparam ENC - Message character encoding 
  //! \tparam WM - Window message being handled
  //! \tparam SENDER - Originator window type
  //! \tparam EVENTS - Events enumeration 
  //! \tparam CODE - Desired event
  //! \tparam ARGS - Event arguments type
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, WindowMessage WM, typename SENDER, typename EVENTS, EVENTS EVENT, typename = std::enable_if_t<std::is_enum<EVENTS>::value>>
  struct CtrlEventDelegate;



  
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct ReflectedCtrlEventDelegate - Calls a child control event handler of signature: LResult (sender_t&)
  //! 
  //! \tparam ENC - Message character encoding 
  //! \tparam WM - Window message
  ///////////////////////////////////////////////////////////////////////////////
  //template <Encoding ENC, WindowMessage WM>
  //struct ReflectedCtrlEventDelegate : MessageDelegate<ENC,WM>
  //{
  //  // ------------------- TYPES & CONSTANTS --------------------

  //  //! \alias base - Define base type
  //  using base = MessageDelegate<ENC,WM>;
  //  
  //  //! \alias arguments_t - Define message argument decoder type
  //  using arguments_t = EventArgs<ENC,WM>;
  //  
  //  // --------------------- CONSTRUCTION ----------------------

  //  ///////////////////////////////////////////////////////////////////////////////
  //  // ReflectedCtrlEventDelegate::ReflectedCtrlEventDelegate
  //  //! Create a single parameter delegate from a member function of a window object
  //  //! 
  //  //! \tparam WINDOW - Window object type
  //  //! \tparam METHOD - Method pointer type
  //  //!
  //  //! \param[in] *object - Window object instance
  //  //! \param[in] method - Member function pointer to event handler method
  //  ///////////////////////////////////////////////////////////////////////////////
  //  template <typename WINDOW, typename METHOD>
  //  ReflectedCtrlEventDelegate(WINDOW* object, METHOD method) : base(std::bind(method, object, std::placeholders::_1))
  //  {}
  //  
  //  // ----------------------- MUTATORS ------------------------

  //  ///////////////////////////////////////////////////////////////////////////////
  //  // ReflectedCtrlEventDelegate::invoke
  //  //! Invokes the delegate, executing the handler for handling, reflecting, or rejecting a message 
  //  //! 
  //  //! \param[in] &wnd - Window receiving message
  //  //! \param[in] w - First parameter
  //  //! \param[in] l - Second parameter
  //  //! \return LResult - [Unhandled] Handler is incapable or chose not to process this message
  //  //!                   [Handled] Handler processed this message
  //  //!                   [Reflected] Handler reflected this message
  //  ///////////////////////////////////////////////////////////////////////////////
  //  LResult invoke(const HWnd& wnd, ::WPARAM w, ::LPARAM l) 
  //  {
  //    arguments_t args(wnd, w, l);    //!< Decode arguments, bind to lifetime of handler execution

  //    // Invoke handler, pass arguments by-ref
  //    return this->Impl(args);
  //  }
  //  
  //  // -------------------- REPRESENTATION ---------------------

  //};

  

  
}

#endif // WTL_EVENT_DELEGATE_HPP
