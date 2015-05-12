//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\events\ActionEvent.hpp
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
#include "wtl/windows/Action.hpp"             //!< ActionSource/ActionState
#include "wtl/windows/MessageEvent.hpp"       //!< MessageEvent

//! \namespace wtl - Windows template library
namespace wtl {
//! \namespace events - WTL Window events
namespace events 
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
    
    // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // EventArgs<WindowMessage::COMMAND>::EventArgs
    //! Decode arguments for win32 message 'WM_COMMAND' 
    //! 
    //! \param[in] w - Originator window id in the LO word, NotificationId in the HO word
    //! \param[in] l - Originator window handle
    /////////////////////////////////////////////////////////////////////////////////////////
    EventArgs(::WPARAM w, ::LPARAM l) : Ident(enum_cast<CommandId>(LOWORD(w))), 
                                        Source(enum_cast<ActionSource>(HIWORD(w)))
    {}
    
    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    // ----------------------------------- REPRESENTATION -----------------------------------

    CommandId      Ident;     //!< Action id 
    ActionSource  Source;     //!< How command was raised
  };
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias ActionEvent - Defines 'Action' event (ie. WM_COMMAND from menus/accelerators/toolbars)
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using ActionEvent = MessageEvent<ENC,WindowMessage::COMMAND>;
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias ActionEventArgs - Arguments for 'Action' Event (ie. WM_COMMAND from menus/accelerators/toolbars)
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using ActionEventArgs = EventArgs<ENC,WindowMessage::COMMAND>;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias ActionEventHandler - Handler for 'Action' event (ie. WM_COMMAND from menus/accelerators/toolbars)
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using ActionEventHandler = MessageEventHandler<ENC,WindowMessage::COMMAND>;

  
} // namespace events
} // namespace wtl

#endif // WTL_COMMAND_EVENT_HPP
