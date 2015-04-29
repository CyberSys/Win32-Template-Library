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
    MenuItem = 0,     //!< Command raised via menu item
    Accelerator = 1,  //!< Command raised via accelerator
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
    
    //! \alias resource_t - Define resource id type
    using resource_t = ResourceId<ENC>;
    
    //! \var encoding - Define message character encoding 
    static constexpr Encoding  encoding = ENC;
    
    //! \var message - Define message identifier
    static constexpr WindowMessage  message = WindowMessage::COMMAND;
    
    //! \var unhandled - Define unhandled result
    static constexpr ::LRESULT  unhandled = unhandled_result<message>::value;
    
    // --------------------- CONSTRUCTION ----------------------

    ///////////////////////////////////////////////////////////////////////////////
    // EventArgs<WindowMessage::COMMAND>::EventArgs
    //! Decode arguments for win32 message 'WM_COMMAND' 
    //! 
    //! \param[in] w - Originator window id in the LO word, NotificationId in the HO word
    //! \param[in] l - Originator window handle
    ///////////////////////////////////////////////////////////////////////////////
    EventArgs(::WPARAM w, ::LPARAM l) : Ident(enum_cast<CommandId>(LOWORD(w))), 
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
  //! \alias GuiCommandEvent - Defines 'Command' event (ie. WM_COMMAND)
  //! 
  //! \tparam ENC - Window character encoding
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using GuiCommandEvent = MessageEvent<ENC,WindowMessage::COMMAND>;
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \alias GuiCommandEventArgs - Arguments for 'Command' Event (ie. WM_COMMAND)
  //! 
  //! \tparam ENC - Message character encoding 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using GuiCommandEventArgs = EventArgs<ENC,WindowMessage::COMMAND>;

  ///////////////////////////////////////////////////////////////////////////////
  //! \alias GuiCommandEventHandler - Handler for 'Command' event (ie. WM_COMMAND)
  //! 
  //! \tparam ENC - Window character encoding
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using GuiCommandEventHandler = typename GuiCommandEvent<ENC>::delegate_t;


}

#endif // WTL_COMMAND_EVENT_HPP
