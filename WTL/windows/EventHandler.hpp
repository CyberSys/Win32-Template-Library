////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\events\EventHandler.hpp
//! \brief Provides an event handler for win32 messages
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
  //! \interface IEventHandler - Interface for all Win32 message handlers
  //! 
  //! \tparam ENC - Message character encoding 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct IEventHandler
  {
    // ------------------- TYPES & CONSTANTS -------------------

    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;

    //! \var encoding - Define encoding type
    static constexpr Encoding encoding = ENC;
    
    // --------------------- CONSTRUCTION ----------------------

    ///////////////////////////////////////////////////////////////////////////////
    // IEventHandler::~IEventHandler
    //! Virtual d-tor
    ///////////////////////////////////////////////////////////////////////////////
    virtual ~IEventHandler() 
    {}
    
    // ---------------------- ACCESSORS ------------------------			

    ///////////////////////////////////////////////////////////////////////////////
    // IEventHandler::accept const
    //! Query the whether the handler accepts the message
    //! 
    //! \param[in] &wnd - Window receiving message
    //! \param[in] m - Window message
    //! \param[in] w - First parameter
    //! \param[in] l - Second parameter
    //! \return WindowMessage - Message consumed by handler
    ///////////////////////////////////////////////////////////////////////////////
    virtual bool accept(const HWnd& wnd, WindowMessage m, ::WPARAM w, ::LPARAM l) const = 0;

    ///////////////////////////////////////////////////////////////////////////////
    // IEventHandler::ident const
    //! Query the window message consumed by handler at runtime
    //! 
    //! \return WindowMessage - Message consumed by handler
    ///////////////////////////////////////////////////////////////////////////////
    virtual WindowMessage ident() const = 0;
    
    // ----------------------- MUTATORS ------------------------

    ///////////////////////////////////////////////////////////////////////////////
    // IEventHandler::invoke
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
  //! \struct EventHandler - Encapsulates an event handler for a win32 message
  //! 
  //! \tparam ENC - Message character encoding 
  //! \tparam WM - Window message
  //! \tparam DEL - Delegate type
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, WindowMessage WM, typename DEL>
  struct EventHandler : IEventHandler<ENC>
  {
    // ------------------- TYPES & CONSTANTS -------------------
  
    //! \alias base - Define base type
    using base = IEventHandler<ENC>;
    
    //! \alias delegate_t - Define delegate type
    using delegate_t = DEL;
    
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
    
    ///////////////////////////////////////////////////////////////////////////////
    // EventHandler::~EventHandler
    //! Virtual d-tor
    ///////////////////////////////////////////////////////////////////////////////
    virtual ~EventHandler()
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
      // Query whether delegate accepts message
      return Delegate.accept(wnd,m,w,l);
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // EventHandler::ident const
    //! Query the window message consumed by handler at runtime
    //! 
    //! \return WindowMessage - Message consumed by handler
    ///////////////////////////////////////////////////////////////////////////////
    virtual WindowMessage ident() const 
    {
      return message;
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
    
    ///////////////////////////////////////////////////////////////////////////////
    // EventHandler::operator interface_t
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
  //! \struct EventHanderCollection - Encpasulates a collection of event handlers
  //! 
  //! \tparam ENC - Message character encoding
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct EventHanderCollection 
  {
    // ------------------- TYPES & CONSTANTS -------------------
  
    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;

    //! \alias handler_t - Event handler interface
    using handler_t = IEventHandler<ENC>;

    //! \alias handler_ptr_t - Shared Event handler pointer
    using handler_ptr_t = std::shared_ptr<IEventHandler<ENC>>;
    
    //! \alias collection_t - Define internal storage type (List of shared pointers)
    using collection_t = std::list<handler_ptr_t>;

    //! \alias encoding - Define encoding type
    static constexpr Encoding encoding = ENC;

    //! \alias CreateStruct - Define WM_CREATE/WM_NCCREATE creation data
    //using CreateStruct = getType<char_t,::CREATESTRUCTA,::CREATESTRUCTW>;

    // --------------------- CONSTRUCTION ----------------------

    ///////////////////////////////////////////////////////////////////////////////
    // EventHanderCollection::~EventHanderCollection
    //! Virtual d-tor
    ///////////////////////////////////////////////////////////////////////////////
    virtual ~EventHanderCollection()
    {}

    // ------------------------ STATIC -------------------------

    // ---------------------- ACCESSORS ------------------------			
    
    ///////////////////////////////////////////////////////////////////////////////
    // EventHanderCollection::begin/end const
    //! Exposes immutable element iterators
    //!
    //! \return collection_t::const_iterator - Immutable element iterator
    ///////////////////////////////////////////////////////////////////////////////
    typename collection_t::const_iterator begin() const   { return Handlers.begin(); }
    typename collection_t::const_iterator end() const     { return Handlers.end();   }

    // ----------------------- MUTATORS ------------------------
    
    ///////////////////////////////////////////////////////////////////////////////
    // EventHanderCollection::begin/end
    //! Exposes mutable element iterators
    //!
    //! \return collection_t::iterator - Mutable element iterator
    ///////////////////////////////////////////////////////////////////////////////
    typename collection_t::iterator begin()  { return Handlers.begin(); }
    typename collection_t::iterator end()    { return Handlers.end();   }

    ///////////////////////////////////////////////////////////////////////////////
    // EventHanderCollection::operator +=
    //! Add an event handler
    //!
    //! \param[in] *ptr - Pointer to event handler
    //! \return EventHanderCollection& - Reference to self
    ///////////////////////////////////////////////////////////////////////////////
    EventHanderCollection& operator+= (handler_t* ptr)
    {
      //! \var findByAddress - Find handler by address
      auto findByAddress = [ptr] (const handler_ptr_t& h) -> bool  { return h.get() == ptr; };

      //! \var findByMessage - Find handler by message handled
      auto findByMessage = [ptr] (const handler_ptr_t& h) -> bool  { return h->ident() == ptr->ident(); };

      // Ensure not already present
      if (std::find_if(Handlers.begin(), Handlers.end(), findByAddress) == Handlers.end())
      {
        // Remove handler for similar message, if any
        Handlers.remove_if(findByMessage);

        // [UNIQUE] Encapsulate in shared pointer
        Handlers.emplace_back(ptr);
      }

      return *this;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // EventHanderCollection::operator -=
    //! Remove an event handler
    //!
    //! \param[in] *ptr - Pointer to event handler
    //! \return EventHanderCollection& - Reference to self
    ///////////////////////////////////////////////////////////////////////////////
    EventHanderCollection& operator-= (handler_t* ptr)
    {
      //! \var findByAddress - Find handler by address
      auto findByAddress = [ptr] (const handler_ptr_t& h) -> bool  { return h.get() == ptr; };

      // Remove handler if present
      Handlers.remove_if(findByAddress);
      return *this;
    }

    // -------------------- REPRESENTATION ---------------------
  protected:
    collection_t  Handlers;      //!< Event handler storage
  };



}

#endif // WTL_EVENT_HANDLER_HPP
