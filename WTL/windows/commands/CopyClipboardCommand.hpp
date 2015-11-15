//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\commands\CopyClipboardCommand.hpp
//! \brief Encapsulates the 'copy text' Gui Command
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_COPY_CLIPBOARD_HPP
#define WTL_COPY_CLIPBOARD_HPP

#include <wtl/WTL.hpp>
#include <wtl/windows/Command.hpp>             //!< Command
#include <wtl/windows/WindowBase.hpp>         //!< WindowBase

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct CopyClipboardCommand - Encapsulates copying text from the control with input focus
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct CopyClipboardCommand : Command<ENC>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \alias type - Define own type
    using type = CopyClipboardCommand<ENC>;
  
    //! \alias base - Define base type
    using base = Command<ENC>;

    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;
    
    //! \alias window_t - Define window base type
    using window_t = WindowBase<ENC>;

    //! \var encoding - Define encoding type
    static constexpr Encoding encoding = ENC;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    window_t*  TargetWnd;       //!< Destination window

    // ------------------------------------- CONSTRUCTION -----------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // CopyClipboardCommand::CopyClipboardCommand
    //! Create command
    /////////////////////////////////////////////////////////////////////////////////////////
    CopyClipboardCommand() : base(CommandId::Edit_Copy, [this] () { if ((TargetWnd = window_t::getFocus()))
                                                                     TargetWnd->Text = "TODO: Copy text to clipboard"; })
    {}
    
	  // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    ENABLE_COPY(CopyClipboardCommand);      //!< Can be shallow copied
    ENABLE_MOVE(CopyClipboardCommand);      //!< Can be moved 
    ENABLE_POLY(CopyClipboardCommand);      //!< Can be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // CopyClipboardCommand::clone const
    //! Create a new instance of the command
    //! 
    //! \return type* - New instance of command
    /////////////////////////////////////////////////////////////////////////////////////////
    type*  clone() const override
    {
      return new CopyClipboardCommand(*this);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // CopyClipboardCommand::state const
    //! Query the current state of the command 
    //! 
    //! \return CommandState - Current state of command
    /////////////////////////////////////////////////////////////////////////////////////////
    CommandState state() const override
    {
      // TODO: Determine whether window has a text selection
      return CommandState::Enabled;
    }
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------
    
  };
  

}

#endif // WTL_COPY_CLIPBOARD_HPP
