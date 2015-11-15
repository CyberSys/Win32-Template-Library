//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\events\CommandEvent.hpp
//! \brief Encapsulates the WM_COMMAND message sent by menus, accelerators, and toolbars in the 'Command' event. 
//! \remarks Note that WM_COMMAND messages sent from controls are handled by 'ControlEvent'
//! \date 25 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_COMMAND_EVENT_HPP
#define WTL_COMMAND_EVENT_HPP

#include <wtl/WTL.hpp>
#include <wtl/casts/EnumCast.hpp>             //!< EnumCast
#include <wtl/platform/CommandId.hpp>         //!< CommandId
#include <wtl/windows/Command.hpp>            //!< CommandSource/CommandState
#include <wtl/windows/EventArgs.hpp>          //!< EventArgs

//! \namespace wtl - Windows template library
namespace wtl
{

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct EventArgs<WindowMessage::COMMAND> - Arguments decoder for Gui commands raised via menu or accelerator
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct EventArgs<ENC,WindowMessage::COMMAND> 
  {  
    // ---------------------------------- TYPES & CONSTANTS ----------------------------------
    
    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;
    
    //! \alias resource_t - Define resource id type
    using resource_t = ResourceId<ENC>;
    
    //! \var encoding - Define message character encoding 
    static constexpr Encoding  encoding = ENC;
    
    //! \var message - Define message identifier
    static constexpr WindowMessage  message = WindowMessage::COMMAND;
    
    //! \var unhandled - Define unhandled result
    static constexpr ::LRESULT  unhandled = unhandled_result<message>::value;
    
    // ------------------------------------- CONSTRUCTION -----------------------------------
	
    /////////////////////////////////////////////////////////////////////////////////////////
    // EventArgs<WindowMessage::COMMAND>::EventArgs
    //! Decode arguments for win32 message 'WM_COMMAND' 
    //! 
    //! \param[in] w - Originator window id in the LO word, NotificationId in the HO word
    //! \param[in] l - Originator window handle
    /////////////////////////////////////////////////////////////////////////////////////////
    EventArgs(::WPARAM w, ::LPARAM l) : Ident(enum_cast<CommandId>(LOWORD(w))), 
                                        Source(enum_cast<CommandSource>(HIWORD(w)))
    {}
    
	  // -------------------------------- COPY, MOVE & DESTROY  -------------------------------
  
    ENABLE_COPY(EventArgs);      //!< Can be shallow copied
    ENABLE_MOVE(EventArgs);      //!< Can be moved
    ENABLE_POLY(EventArgs);      //!< Can be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    // ----------------------------------- REPRESENTATION -----------------------------------

    CommandId      Ident;      //!< Command id 
    CommandSource  Source;     //!< How command was raised
  };
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias CommandEventArgs - Defines arguments type for the 'Command' Event 
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using CommandEventArgs = EventArgs<ENC,WindowMessage::COMMAND>;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias CommandEvent - Defines the signature of 'Command' event handlers  [Pass by value]
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using CommandEvent = Event<LResult,CommandEventArgs<ENC>>;
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias CommandEventHandler - Defines the delegate type for the 'Command' event
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using CommandEventHandler = handler_t<CommandEvent<ENC>>;

  
} // namespace wtl

#endif // WTL_COMMAND_EVENT_HPP
