//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\events\CommandEvent.hpp
//! \brief Provides argument/delegate/handler types for action events
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_COMMAND_EVENT_HPP
#define WTL_COMMAND_EVENT_HPP

#include "wtl/WTL.hpp"
#include "wtl/casts/EnumCast.hpp"             //!< EnumCast
#include "wtl/platform/CommandId.hpp"         //!< CommandId
#include "wtl/windows/Command.hpp"             //!< CommandSource/CommandState
#include "wtl/windows/MessageEvent.hpp"       //!< MessageEvent

//! \namespace wtl - Windows template library
namespace wtl {

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
    
	  // -------------------------------- COPYING & DESTRUCTION -------------------------------
  
    ENABLE_COPY(EventArgs);      //!< Can be shallow copied
    ENABLE_MOVE(EventArgs);      //!< Can be moved
    ENABLE_POLY(EventArgs);      //!< Can be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    // ----------------------------------- REPRESENTATION -----------------------------------

    CommandId      Ident;     //!< Command id 
    CommandSource  Source;     //!< How command was raised
  };
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias CommandEvent - Defines 'Command' event (ie. WM_COMMAND from menus/accelerators/toolbars)
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using CommandEvent = MessageEvent<ENC,WindowMessage::COMMAND>;
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias CommandEventArgs - Arguments for 'Command' Event (ie. WM_COMMAND from menus/accelerators/toolbars)
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using CommandEventArgs = EventArgs<ENC,WindowMessage::COMMAND>;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias CommandEventHandler - Handler for 'Command' event (ie. WM_COMMAND from menus/accelerators/toolbars)
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using CommandEventHandler = MessageEventHandler<ENC,WindowMessage::COMMAND>;

  
} // namespace wtl

#endif // WTL_COMMAND_EVENT_HPP
