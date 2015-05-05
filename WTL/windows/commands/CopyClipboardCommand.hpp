//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\commands\CopyClipboardCommand.hpp
//! \brief Encapsulates the 'copy text' Gui Command
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_COPY_CLIPBOARD_HPP
#define WTL_COPY_CLIPBOARD_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct CopyClipboardCommand - Encapsulates copying text from the control with input focus
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct CopyClipboardCommand : Action<ENC>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \alias base - Define base type
    using base = Action<ENC>;

    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;
    
    //! \alias window_t - Define window base type
    using window_t = WindowBase<ENC>;

    //! \var encoding - Define encoding type
    static constexpr Encoding encoding = ENC;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    window_t*  TargetWnd;       //!< Destination window

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // CopyClipboardCommand::CopyClipboardCommand
    //! Create command
    /////////////////////////////////////////////////////////////////////////////////////////
    CopyClipboardCommand()  
      : base(CommandId::Edit_Copy, [this] () { if (TargetWnd = window_t::getFocus())
                                                 TargetWnd->setText(c_arr(L"TODO: Copy text to clipboard")); })
    {}
    
    // -------------------------------------- ACCESSORS --------------------------------------			

    // -------------------------------------- ACCESSORS --------------------------------------			
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // CopyClipboardCommand::clone const
    //! Create a new instance of the command
    //! 
    //! \return type* - New instance of command
    /////////////////////////////////////////////////////////////////////////////////////////
    typename base::type*  clone() const 
    {
      return new CopyClipboardCommand(*this);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // CopyClipboardCommand::state const
    //! Query the current state of the command 
    //! 
    //! \return ActionState - Current state of command
    /////////////////////////////////////////////////////////////////////////////////////////
    ActionState state() const override
    {
      // TODO: Determine whether window has a text selection
      return ActionState::Enabled;
    }
    
    // --------------------------------------- MUTATORS --------------------------------------
    
  };
  

}

#endif // WTL_COPY_CLIPBOARD_HPP
