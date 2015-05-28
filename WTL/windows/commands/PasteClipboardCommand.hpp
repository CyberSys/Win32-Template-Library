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
    
    //! \alias type - Define own type
    using type = PasteClipboardCommand<ENC>;
  
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

    // ------------------------------------- CONSTRUCTION -----------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // PasteClipboardCommand::PasteClipboardCommand
    //! Create command
    /////////////////////////////////////////////////////////////////////////////////////////
    PasteClipboardCommand()  
      : base(ActionId::Edit_Paste, [this] () { if (TargetWnd = window_t::getFocus())
                                                  TargetWnd->Text = c_str(L"TODO: Paste text from clipboard"); },
                                    [this] () { if (TargetWnd)
                                                  TargetWnd->Text = c_str(L"TODO: Restore previous text"); })
    {}
    
	  // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    ENABLE_COPY(PasteClipboardCommand);      //!< Can be shallow copied
    ENABLE_MOVE(PasteClipboardCommand);      //!< Can be moved 
    ENABLE_POLY(PasteClipboardCommand);      //!< Can be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // PasteClipboardCommand::clone const
    //! Create a new instance of the command
    //! 
    //! \return type* - New instance of command
    /////////////////////////////////////////////////////////////////////////////////////////
    type*  clone() const 
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
