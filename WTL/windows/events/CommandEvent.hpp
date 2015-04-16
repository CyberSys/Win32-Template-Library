////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\events\CommandEvent.hpp
//! \brief Provides argument/delegate/handler types for events from controls via WM_COMMAND
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_COMMAND_EVENT_HPP
#define WTL_COMMAND_EVENT_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  //! \enum CommandSource - Defines how a Gui command was raised
  enum class CommandSource
  {
    Menu = 0,         //!< Command raised via accelerator
    Accelerator = 1,  //!< Command raised via menu
  };

  //! \enum CommandState - Define states of GUI Commands
  enum class CommandState
  {
    Disabled = 0,     //!< Command should be disabled
    Enabled = 1,      //!< Command should be enabled
    Hidden = 2,       //!< Command should be hidden
  };


  ///////////////////////////////////////////////////////////////////////////////
  //! \struct EventArgs<WindowMessage::COMMAND> - Arguments decoder for Gui commands raised via menu or accelerator
  //! 
  //! \tparam ENC - Message character encoding 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct EventArgs<ENC,WindowMessage::COMMAND> 
  {  
    // ------------------- TYPES & CONSTANTS --------------------
    
    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;
    
    //! \var encoding - Define message character encoding 
    static constexpr Encoding  encoding = ENC;
    
    //! \var message - Define message identifier
    static constexpr WindowMessage  message = WindowMessage::COMMAND;

    //! \alias resource_t - Define resource id type
    using resource_t = ResourceId<ENC>;
    
    // --------------------- CONSTRUCTION ----------------------

    ///////////////////////////////////////////////////////////////////////////////
    // EventArgs<WindowMessage::COMMAND>::EventArgs
    //! Decode arguments for win32 message 'WM_COMMAND' 
    //! 
    //! \param[in] const& wnd - Window receiving message
    //! \param[in] w - Originator window id in the LO word, NotificationId in the HO word
    //! \param[in] l - Originator window handle
    ///////////////////////////////////////////////////////////////////////////////
    EventArgs(const HWnd& wnd, ::WPARAM w, ::LPARAM l) : Ident(enum_cast<CommandId>(LOWORD(w))), 
                                                         Source(enum_cast<CommandSource>(HIWORD(w)))
    {}
    
    // ------------------------ STATIC -------------------------

    // ---------------------- ACCESSORS ------------------------			

    // ----------------------- MUTATORS ------------------------

    // -------------------- REPRESENTATION ---------------------

    CommandId      Ident;     //!< Command id 
    CommandSource  Source;    //!< How command was raised
  };
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \alias GuiCommandArgs - Arguments for 'Command' Event (ie. WM_COMMAND)
  //! 
  //! \tparam ENC - Message character encoding 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using GuiCommandArgs = EventArgs<ENC,WindowMessage::COMMAND>;

  

  ///////////////////////////////////////////////////////////////////////////////
  //! \struct GuiCommandDelegate - Invokes a Gui command (raised via 'WM_COMMAND')
  //! 
  //! \tparam ENC - Message character encoding 
  //! \tparam CMD - Command type
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, typename CMD>
  struct GuiCommandDelegate : MessageDelegate<ENC,WindowMessage::COMMAND,void ()>
  {
    // ------------------- TYPES & CONSTANTS --------------------

    //! \alias base - Define base type
    using base = MessageDelegate<ENC,WindowMessage::COMMAND,void ()>;

    //! \alias arguments_t - Define arguments decoder type
    using arguments_t = EventArgs<ENC,WindowMessage::COMMAND>;
    
    //! \alias command_t - Define command type
    using command_t = CMD;

    //! \var command - Define command id
    static constexpr CommandId  command = command_t::command;

    // --------------------- CONSTRUCTION ----------------------

    ///////////////////////////////////////////////////////////////////////////////
    // GuiCommandDelegate::GuiCommandDelegate
    //! Create a single parameter delegate from a member function of a window object
    //! 
    //! \tparam WINDOW - Window object type
    //! \tparam METHOD - Method pointer type
    //!
    //! \param[in] *object - Window object instance
    //! \param[in] method - Member function pointer to event handler method
    ///////////////////////////////////////////////////////////////////////////////
    template <typename... ARGS>
    GuiCommandDelegate(ARGS&&... args) : Command(std::forward<ARGS>(args)...)
    {}
    
    // ---------------------- ACCESSORS ------------------------			
    
    ///////////////////////////////////////////////////////////////////////////////
    // GuiCommandDelegate::accept const
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
      // [COMMAND] Verify message is not an event from a child control 
      return m == WindowMessage::COMMAND 
          && l == 0
          && arguments_t(wnd, w, l).Ident == command      //!< Instantiate arguments only after confirming WM_COMMAND
          && Command.state() == CommandState::Enabled;    //!< Require gui command be enabled
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // GuiCommandDelegate::ident const
    //! Query the window message consumed by delegate
    //! 
    //! \return HandlerIdent - Message consumed by delegate
    ///////////////////////////////////////////////////////////////////////////////
    HandlerIdent ident() const override
    {
      // Return message and command
      return { message, command };
    }
    
    // ----------------------- MUTATORS ------------------------

    ///////////////////////////////////////////////////////////////////////////////
    // GuiCommandDelegate::invoke
    //! Executes the Gui command associated with the commmand id
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

      // Execute Gui command
      Command.execute(args.Source);

      // Handled
      return 0;
    }
    
    // -------------------- REPRESENTATION ---------------------
  protected:
    command_t    Command;       //!< Gui Command logic
  };
  
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \alias GuiCommandHandler - Handler for gui commands raised by menu or accelerator (ie. WM_COMMAND)
  //! 
  //! \tparam ENC - Window character encoding 
  //! \tparam CMD - Gui command 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, typename CMD>
  using GuiCommandHandler = EventHandler<ENC,WindowMessage::COMMAND,GuiCommandDelegate<ENC,CMD>>;

  
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct EventDelegate<WindowMessage::COMMAND> - Delegate for Gui commands raised via menu or accelerator 
  //! 
  //! \tparam ENC - Message character encoding 
  //! \tparam SENDER - Originator window type
  //! \tparam EVENTS - Events enumeration 
  //! \tparam CODE - Desired event
  ///////////////////////////////////////////////////////////////////////////////
  //template <Encoding ENC>
  //struct EventDelegate<ENC,WindowMessage::COMMAND> : MessageDelegate<ENC,WindowMessage::COMMAND,LResult (CommandSource)>
  //{
  //  // ------------------- TYPES & CONSTANTS --------------------

  //  //! \alias base - Define base type
  //  using base = MessageDelegate<ENC,WindowMessage::COMMAND,LResult (CommandSource)>;
  //  
  //  //! \alias arguments_t - Define arguments decoder type
  //  using arguments_t = EventArgs<ENC,WindowMessage::COMMAND>;
  //  
  //  // --------------------- CONSTRUCTION ----------------------

  //  ///////////////////////////////////////////////////////////////////////////////
  //  // EventDelegate::EventDelegate
  //  //! Create a parameterless delegate from a member function of a window object
  //  //! 
  //  //! \tparam WINDOW - Window object type
  //  //! \tparam METHOD - Method pointer type
  //  //!
  //  //! \param[in] *object - Window object instance
  //  //! \param[in] method - Member function pointer to event handler method
  //  ///////////////////////////////////////////////////////////////////////////////
  //  template <typename WINDOW, typename METHOD>
  //  EventDelegate(WINDOW* object, METHOD method) : base(std::bind(method, object))
  //  {}
  //  
  //  // ---------------------- ACCESSORS ------------------------			
  //  
  //  ///////////////////////////////////////////////////////////////////////////////
  //  // EventDelegate::accept const
  //  //! Query the whether the delegate accepts the message
  //  //! 
  //  //! \param[in] &wnd - Window receiving message
  //  //! \param[in] m - Window message
  //  //! \param[in] w - First parameter
  //  //! \param[in] l - Second parameter
  //  //! \return bool - True iff accepted by delegate
  //  ///////////////////////////////////////////////////////////////////////////////
  //  bool accept(const HWnd& wnd, WindowMessage m, ::WPARAM w, ::LPARAM l) const override
  //  {
  //    // [COMMAND] Verify message is not an event from a child control 
  //    return m == WindowMessage::COMMAND 
  //        && l == 0;
  //  }
  //  
  //  // ----------------------- MUTATORS ------------------------

  //  ///////////////////////////////////////////////////////////////////////////////
  //  // EventDelegate::invoke
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
  //    arguments_t args(wnd, w, l);    //!< Decode arguments

  //    // Invoke handler, pass source
  //    return this->Impl(args.Source);
  //  }
  //  
  //  // -------------------- REPRESENTATION ---------------------
  //};

  ///////////////////////////////////////////////////////////////////////////////
  //! \alias CommandEventDelegate - Delegate for 'Command' event (ie. WM_COMMAND)
  //! 
  //! \tparam ENC - Window character encoding 
  ///////////////////////////////////////////////////////////////////////////////
  /*template <Encoding ENC>
  using CommandEventDelegate = EventDelegate<ENC,WindowMessage::COMMAND>;*/

  
}

#endif // WTL_COMMAND_EVENT_HPP
