//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\commands\PasteClipboardCommand.hpp
//! \brief Encapsulates the 'paste text' Gui Command
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_PASTE_CLIPBOARD_HPP
#define WTL_PASTE_CLIPBOARD_HPP

#include "wtl/WTL.hpp"
#include "wtl/windows/Action.hpp"             //!< Action
#include "wtl/windows/WindowBase.hpp"         //!< WindowBase

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct PasteClipboardCommand - Encapsulates pasting text to the control with input focus
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct PasteClipboardCommand : Action<ENC>
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

    // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // PasteClipboardCommand::PasteClipboardCommand
    //! Create command
    /////////////////////////////////////////////////////////////////////////////////////////
    PasteClipboardCommand()  
      : base(CommandId::Edit_Paste, [this] () { if (TargetWnd = window_t::getFocus())
                                                  TargetWnd->setText(c_arr(L"TODO: Paste text from clipboard")); },
                                    [this] () { if (TargetWnd)
                                                  TargetWnd->setText(c_arr(L"TODO: Restore previous text")); })
    {}
    
    // ---------------------------------- ACCESSOR METHODS ----------------------------------			

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // PasteClipboardCommand::clone const
    //! Create a new instance of the command
    //! 
    //! \return type* - New instance of command
    /////////////////////////////////////////////////////////////////////////////////////////
    typename base::type*  clone() const 
    {
      return new PasteClipboardCommand(*this);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // PasteClipboardCommand::state const
    //! Query the current state of the command 
    //! 
    //! \return ActionState - Current state of command
    /////////////////////////////////////////////////////////////////////////////////////////
    ActionState state() const override
    {
      // Always enabled
      return ActionState::Enabled;
    }
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------
    
  };
  

}

#endif // WTL_PASTE_CLIPBOARD_HPP
