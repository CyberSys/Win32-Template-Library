////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\MessageHandler.hpp
//! \brief Base class for all Win32 message handlers
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_EVENT_HANDLER_HPP
#define WTL_EVENT_HANDLER_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct IEventHandler - Interface for all Win32 message handlers
  //! 
  //! \tparam ENC - Message character encoding 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct IEventHandler
  {
    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;

    //! \var encoding - Define encoding type
    static constexpr Encoding encoding = ENC;

    ///////////////////////////////////////////////////////////////////////////////
    // IEventHandler::~IEventHandler
    //! Virtual d-tor
    ///////////////////////////////////////////////////////////////////////////////
    virtual ~IEventHandler() 
    {}

    ///////////////////////////////////////////////////////////////////////////////
    // IEventHandler::message const
    //! Query the window message consumed by handler at runtime
    //! 
    //! \return WindowMessage - Message consumed by handler
    ///////////////////////////////////////////////////////////////////////////////
    virtual WindowMessage message() const = 0;

    ///////////////////////////////////////////////////////////////////////////////
    // MessageHandler::invoke
    //! Decodes the message arguments and invokes the handler
    //! 
    //! \param[in] &wnd - Window receiving message
    //! \param[in] m - Window message
    //! \param[in] w - First parameter
    //! \param[in] l - Second parameter
    //! \return LResult - Message result
    ///////////////////////////////////////////////////////////////////////////////
    virtual LResult  invoke(const HWnd& wnd, WindowMessage m, ::WPARAM w, ::LPARAM l) = 0;
  };
  

  ///////////////////////////////////////////////////////////////////////////////
  //! \struct MessageHandler - Encapsulates an event handler for a win32 message
  //! 
  //! \tparam ENC - Message character encoding 
  //! \tparam WM - Window message
  //! \tparam SIG - Delegate signature
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, WindowMessage WM, typename SIG>
  struct MessageHandler : IEventHandler<ENC>
  {
    // ------------------------ TYPES --------------------------
  
    //! \alias base - Define base type
    using base = IEventHandler<ENC>;
    
    //! \alias signature_t - Define signature type
    using signature_t = SIG;

    //! \alias delegate_t - Define delegate type
    using delegate_t = std::function<signature_t>;
    
    //! \alias interface_t - Define interface type
    using interface_t = IEventHandler<ENC>*;

    //! \var message - Define message identifier
    static constexpr WindowMessage message = WM;

    // --------------------- CONSTRUCTION ----------------------

    ///////////////////////////////////////////////////////////////////////////////
    // MessageHandler::MessageHandler
    //! Create an event handler from any callable target
    //! 
    //! \tparam ARGS - Callable target type (Function, lambda expression, bind expression, function object, pointer to member function, pointer to data member)
    //! 
    //! \param[in] &&... args - Arguments passed to delegate constructor
    ///////////////////////////////////////////////////////////////////////////////
    template <typename... ARGS>
    MessageHandler(ARGS&&... args) : Delegate(std::forward<ARGS>(args)...)
    {}
    
    ///////////////////////////////////////////////////////////////////////////////
    // MessageHandler::~MessageHandler
    //! Virtual d-tor
    ///////////////////////////////////////////////////////////////////////////////
    virtual ~MessageHandler()
    {}
    
    // ---------------------- ACCESSORS ------------------------			

    ///////////////////////////////////////////////////////////////////////////////
    // MessageHandler::accept const
    //! Query the whether the handler accepts the message
    //! 
    //! \param[in] &wnd - Window receiving message
    //! \param[in] m - Window message
    //! \param[in] w - First parameter
    //! \param[in] l - Second parameter
    //! \return WindowMessage - Message consumed by handler
    ///////////////////////////////////////////////////////////////////////////////
    virtual bool accept(const HWnd& wnd, WindowMessage m, ::WPARAM w, ::LPARAM l) const 
    {
      // Simply check the message id
      return m == message;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // IEventHandler::message const
    //! Query the window message consumed by handler at runtime
    //! 
    //! \return WindowMessage - Message consumed by handler
    ///////////////////////////////////////////////////////////////////////////////
    virtual WindowMessage message() const 
    {
      return message;
    }
    
    // ----------------------- MUTATORS ------------------------

    ///////////////////////////////////////////////////////////////////////////////
    // MessageHandler::operator interface_t
    //! Implicit user conversion to event handler base interface 
    //!
    //! \return interface_t - Pointer to IEventHandler base interface
    ///////////////////////////////////////////////////////////////////////////////
    operator interface_t ()
    {
      return static_cast<interface_t>(this);
    }

    // -------------------- REPRESENTATION ---------------------
  protected:
    delegate_t  Delegate;    //!< Delegate to handler implementation
  };
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct EventHandler - Encapsulates an event handler for a win32 message
  //! 
  //! \tparam ENC - Message character encoding 
  //! \tparam WM - Window message
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, WindowMessage WM>
  struct EventHandler : MessageHandler<ENC,WM,LResult ()>
  {
    // ------------------------ TYPES --------------------------
  
    //! \alias base - Define base type
    using base = IEventHandler<ENC>;
    
    //! \alias signature_t - Define signature type
    using signature_t = SIG;

    //! \alias delegate_t - Define delegate type
    using delegate_t = std::function<signature_t>;
    
    //! \alias interface_t - Define interface type
    using interface_t = IEventHandler<ENC>*;

    //! \var message - Define message identifier
    static constexpr WindowMessage message = WM;

    // --------------------- CONSTRUCTION ----------------------

    ///////////////////////////////////////////////////////////////////////////////
    // EventHandler::EventHandler
    //! Create an event handler from any callable target
    //! 
    //! \tparam ARGS - Callable target type (Function, lambda expression, bind expression, function object, pointer to member function, pointer to data member)
    //! 
    //! \param[in] &&... args - Arguments passed to delegate constructor
    ///////////////////////////////////////////////////////////////////////////////
    template <typename... ARGS>
    EventHandler(ARGS&&... args) : Delegate(std::forward<ARGS>(args)...)
    {}
    
    // ---------------------- ACCESSORS ------------------------			

    ///////////////////////////////////////////////////////////////////////////////
    // EventHandler::accept const
    //! Query the whether the handler accepts the message
    //! 
    //! \param[in] &wnd - Window receiving message
    //! \param[in] m - Window message
    //! \param[in] w - First parameter
    //! \param[in] l - Second parameter
    //! \return WindowMessage - Message consumed by handler
    ///////////////////////////////////////////////////////////////////////////////
    virtual bool accept(const HWnd& wnd, WindowMessage m, ::WPARAM w, ::LPARAM l) const 
    {
      // Simply check the message id
      return m == message;
    }
    
    // ----------------------- MUTATORS ------------------------

    ///////////////////////////////////////////////////////////////////////////////
    // EventHandler::invoke
    //! Invokes the handler delegate (Decodes and handles/reflects/rejects the message)
    //! 
    //! \param[in] &wnd - Window receiving message
    //! \param[in] m - Window message
    //! \param[in] w - First parameter
    //! \param[in] l - Second parameter
    //! \return LResult - [Unhandled] Handler is incapable or chose not to process this message
    //!                   [Handled] Handler processed this message
    //!                   [Reflected] Handler reflected this message
    ///////////////////////////////////////////////////////////////////////////////
    LResult  invoke(const HWnd& wnd, WindowMessage m, ::WPARAM w, ::LPARAM l) override
    {
      // Invoke delegate 
      return Delegate.invoke(wnd, w, l);
    }
    
    // -------------------- REPRESENTATION ---------------------
  };

  
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct CtrlEventHandler - Encapsulate an handler for win32 messages sent from controls to their parents
  //! 
  //! \tparam ENC - Message character encoding 
  //! \tparam WM - Window message
  //! \tparam EVENTS - Events enumeration 
  //! \tparam CODE - Desired event
  //! \tparam DEL - Delegate type
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, WindowMessage WM, typename EVENTS, EVENTS EVENT, typename DEL, typename = std::enable_if_t<std::is_enum<EVENTS>::value>>
  struct CtrlEventHandler : EventHandler<ENC,WM,DEL>
  {
    // ------------------------ TYPES --------------------------
  
    //! \alias base - Define base type
    using base = IEventHandler<ENC>;
    
    //! \alias event_t - Define notification type
    using event_t = EVENTS;
    
    //! \var event - Define notification identifier
    static constexpr event_t  event = EVENT;
    
    // --------------------- CONSTRUCTION ----------------------

    ///////////////////////////////////////////////////////////////////////////////
    // CtrlEventHandler::CtrlEventHandler
    //! Create an event handler from any callable target
    //! 
    //! \tparam DARGS - Argument types
    //! 
    //! \param[in] &&... args - Delegate constructor arguments 
    ///////////////////////////////////////////////////////////////////////////////
    template <typename... DARGS>
    CtrlEventHandler(DARGS&&... args) : Delegate(std::forward<CARGS>(args)...)
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
    //! \return WindowMessage - Message consumed by handler
    ///////////////////////////////////////////////////////////////////////////////
    bool accept(const HWnd& wnd, WindowMessage m, ::WPARAM w, ::LPARAM l) const override
    {
      switch (m)
      {
      // [COMMAND] Verify from a child control 
      case WindowMessage::COMMAND: return l == 0 && enum_cast<EVENTS>(LOWORD(w)) == EVENT;
      case WindowMessage::NOTIFY:  return opaque_cast<::NMHDR>(l)->code == EVENT;
      }
      return false;
    }
    
    // ----------------------- MUTATORS ------------------------

    // -------------------- REPRESENTATION ---------------------

  };



}

#endif // WTL_EVENT_HANDLER_HPP
